/*
 *    Akvarion, Jan 2023
*/

#include "../odpi-main/include/dpi.h" //PATH TO ODPI-C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define FALSE 0
#define TRUE 1
#define DEBUG_USER_INPUT 0
#define CONN_STRING "172.17.0.2:1521/XEPDB1"    // this 

static dpiErrorInfo error_info;
static dpiContext *context=NULL;


//prints errors
void printError(dpiContext *context, dpiErrorInfo *ErrorInfo){
    dpiContext_getError(context, ErrorInfo);
    fprintf(stderr,"-----------------------------------------\n");
    fprintf(stderr, "[FAILED]\n");
    fprintf(stderr, "\tFN: %s\n", ErrorInfo->fnName);
    fprintf(stderr, "\tACTION: %s\n", ErrorInfo->action);
    fprintf(stderr, "\tMSG: %.*s\n", ErrorInfo->messageLength, ErrorInfo->message);
    fflush(stderr);
}

//Not used.take a string out from the buffer and copy it in a string.
int buff_out(char *dest, char *buffer, size_t *dest_size){
    int i;
    bool FLAG=FALSE;
    //foreach charcter in the string
    for(i=0;i<strlen(buffer)&&!FLAG;i++){
        //check if this is the terminator and set the FLAG
        if(buffer[i]=='\0'){
           FLAG=TRUE;
        }
        //if the size of destination is smaller than what we need to copy, reallocate memory
        if(*dest_size==(size_t)i){
            dest=(char*)realloc(dest,(*dest_size+1)*sizeof(char));
        }
        dest[i]=buffer[i];
    }
    if(dest[i]=='\0'){
        fprintf(stdout,"string has a null terminator");
        return 0;
    }
    else {
        fprintf(stdout, "string does not have a null terminator\n");
        return -1;
    }

}

//User authentication dummy routine
//not used. also maybe memory leaks
void get_user_info(char *username, char *password){
    size_t size=0;
    char buffer[32];
    memset(buffer,48,sizeof(buffer));
    fprintf(stdout,"-----------------------------------------\n");
    fprintf(stdout,"Username: ");
    fflush(stdout);

    if ( fgets(buffer, sizeof(buffer), stdin) != NULL ){
        char *newline = strchr(buffer, '\n'); /* search for newline character */
        if ( newline != NULL ){
            *newline = '\0'; /* overwrite trailing newline */
        }
    }
    username=(char*)malloc((strlen(buffer)+1)*sizeof(char));
    strcpy(username,buffer);
    //size++;
    //buff_out(username,buffer,&size);
    memset(buffer,48,sizeof(buffer));

    fprintf(stdout,"\nPassword: ");
    fflush(stdout);

    if (fgets(buffer, sizeof(buffer), stdin) != NULL ){
        char *newline = strchr(buffer, '\n'); /* search for newline character */
        if ( newline != NULL ){
            *newline = '\0'; /* overwrite trailing newline */
        }
    }

    password=(char*)malloc((strlen(buffer)+1)*sizeof(char));
    strcpy(password,buffer);
    //size++;
    //buff_out(password,buffer,&size);
    

    fflush(stdout);
    fflush(stdin);

}

int statement_creation(dpiConn *conn, dpiContext *context){
    int choice=-1;
    char buffer[64], *user_SQL=malloc(sizeof(char));
    user_SQL[0]='\0';
    memset(buffer,48,sizeof(buffer));
    bool FLAG=TRUE, FLAG_1=TRUE;
    int size=0;
    const char *string;
    dpiStmt *statement;

    fprintf(stdout,"________________________\n");
    fprintf(stdout,"|| Statement creation ||\n");
    fprintf(stdout,"‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
    fprintf(stdout,"Do you wish to continue? [Enter 0 to return to main menu].\n~: ");
    fflush(stdout);
    fscanf(stdin,"%d", &choice);

    if(choice==0) return 1;
    if(choice==1){
        string="INSERT INTO UTENTE VALUES ('abdiel.koepp','Enrico','Lindgren','llockman@example.net','a26c4bbbfbabb03ba220','0.00','0.00','0.00','0.00','0','2008-09-07')";
        FLAG_1=FALSE;
    }

    fprintf(stdout,"Insert your SQL statement. Enter no characters to end the input.\n");
    fflush(stdout);
    //consuming buffer newline, because fscanf and fgets differences.
    //AGAIN USER INPUTS CREATES ISSUES WITH odpi-c -.-'
    while ((getchar()) != '\n');
    while(FLAG && FLAG_1){
        if(fgets(buffer,(int)sizeof(buffer),stdin)==NULL){
            perror("Input Error\n");
            break;;
        }
        else{
            if(buffer[0]=='\n'){
                FLAG=FALSE;
            }
            else{
                size=size+strlen(buffer);
                user_SQL=realloc(user_SQL,size*sizeof(char));
                strcat(user_SQL,buffer);
            }
        }
    }

    //CHECK IF CONNECTION IS HEALTHY
    if(dpiConn_ping(conn)==DPI_FAILURE){
        fprintf(stderr,"[Connection is not healthy]\n");
    }
    else fprintf(stdout,"[HEALTHY CONNECTION]\n");
    
    if(FLAG_1){
        fprintf(stdout,"You have inserted the following:\n\n%s",user_SQL);
        if (dpiConn_prepareStmt(conn, 0, user_SQL, strlen(user_SQL), NULL, 0, &statement)!=DPI_SUCCESS){
            printError(context,&error_info);
            return -1;
        }
        else fprintf(stdout,"[STATEMENT SUCCESSFULLY PREPARED]\n");

    }
    else{
        if (dpiConn_prepareStmt(conn, 0, string, strlen(string), NULL, 0, &statement)!=DPI_SUCCESS){
            printError(context,&error_info);
            return -1;
        }
        else fprintf(stdout,"[STATEMENT SUCCESSFULLY PREPARED]\n");
    }
    
    fflush(stdout);

    return 0;
}

int main(int argc, char** argv){
    int choice =-1;
    dpiConn *conn;
    dpiStmt *statement;
    //dpiErrorInfo errorInfo;
    char *userName=NULL;
    char *password=NULL;

    if(DEBUG_USER_INPUT){
        //Doesn't work, don't know why, throws segmentation fault. Maybe strings format or encoding?
        get_user_info(userName,password);
    }
    else{
        userName="ODINPARTS";
        password="odinparts";
    }

    if(dpiContext_createWithParams(DPI_MAJOR_VERSION, DPI_MINOR_VERSION, NULL,&context,&error_info)!=DPI_SUCCESS){
        printError(context,&error_info);
        return EXIT_FAILURE;
    }
    printf("[CONTEXT CREATED SUCCESSFULLY]\n");

    //CONNECTION CREATION
    if(dpiConn_create(context, userName, strlen(userName), password, strlen(password), CONN_STRING, strlen(CONN_STRING),NULL,NULL,&conn)!=DPI_SUCCESS){
        printError(context,&error_info);
        return EXIT_FAILURE;
    }
    printf("[CONNECTION CREATED SUCCESSFULLY]\n");

    if(dpiConn_ping(conn)==DPI_FAILURE){
        fprintf(stderr,"[Connection is not healthy]\n");
    }
    else fprintf(stdout,"[HEALTHY CONNECTION]\n");

    while(choice!=0){
        fprintf(stdout,"___________________________________\n");
        fprintf(stdout,"|| Select a task to be executed: ||\n");
        fprintf(stdout,"‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
        fprintf(stdout,"\t1. Create statement\n");
        fprintf(stdout,"\t2. Import statement\n");
        fprintf(stdout,"\t3. [DEBUG] Check connection health\n");
        fprintf(stdout,"\t0. Exit\n");
        fprintf(stdout,"~: ");
        fflush(stdout);

        fscanf(stdin,"%d",&choice);
        if(choice==1){
            if(statement_creation(conn,context)==-1){
                printf("Unable to execute statement.\n");
            }
        }

        if(choice==2){}
        if(choice==3){
            if(dpiConn_ping(conn)==DPI_FAILURE){
                fprintf(stderr,"[Connection is not healthy]\n");
            }
            else fprintf(stdout,"[HEALTHY CONNECTION]\n");
            
        }
        if(choice==4){}

    }


    printf("[CLEANING UP...]\n");
    //CLOSING CONNECTION
    dpiConn_release(conn);
    //DESTROYING CONTEXT
    dpiContext_destroy(context);

    printf("[DONE]\n");
    return 0;
}
