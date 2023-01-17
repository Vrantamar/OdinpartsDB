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
#define CONN_STRING "172.17.0.2:1521/XEPDB1"    // this mf took 2h of research to find

static dpiErrorInfo error_info;

//prints errors (duh)
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
//REMEMBER TO FREE THE MEMORY
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

int conn_setup(dpiConn *conn, dpiContext *context){
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
    
    return 0;
}

int create_table(dpiConn *conn){
    int choice=-1;
    fprintf(stdout,"____________________\n");
    fprintf(stdout,"|| Table creation ||\n");
    fprintf(stdout,"‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
    fprintf(stdout,"Do you wish to write SQL code now (1) or fetch from file (2)? [Enter 0 to abort].\n");
    fflush(stdout);
    fscanf(stdin,"%d", &choice);

    if(choice==1){
        char buffer[64], *user_statement=malloc(sizeof(char));
        user_statement[0]='\0';
        memset(buffer,48,sizeof(buffer));
        bool FLAG=TRUE;
        int size=0;
        
        fprintf(stdout,"Insert your SQL statement. Enter no characters to end the input.\n");
        fflush(stdout);
        //consuming buffer newline, because fscanf and fgets differences.
        while ((getchar()) != '\n');
        while(FLAG){
            if(fgets(buffer,(int)sizeof(buffer),stdin)==NULL){
                perror("Input Error\n");
                break;;
            }
            else{
                if(buffer[0]=='\n'){
                    FLAG=FALSE;
                }
                else{
                    size=size+strlen(buffer)+1;
                    user_statement=realloc(user_statement,size*sizeof(char));
                    strcat(user_statement,buffer);
                }
            }
        }

        fprintf(stdout,"You have written the following:\n\n%s",user_statement);

    }
    else if(choice==2){
    }
    else{
        fprintf(stdout,"[ABORTING]...");
        return 1;
    }
    return 0;
}

int main(int argc, char** argv){
    int choice =-1;
    dpiStmt *statement;
    dpiConn *conn;
    dpiContext *context=NULL;
    //dpiErrorInfo errorInfo;
    
    if(conn_setup(conn, context)!=0){
        perror("Errors occurred while connecting to DB instance.\n");
        return EXIT_FAILURE;
    }

    while(choice!=0){
        fprintf(stdout,"___________________________________\n");
        fprintf(stdout,"|| Select a task to be executed: ||\n");
        fprintf(stdout,"‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
        fprintf(stdout,"\t1. Create table\n");
        fprintf(stdout,"\t2. Insert rows\n");
        fprintf(stdout,"\t3. Select some rows\n");
        fprintf(stdout,"\t4. Scroll rows from table\n");
        fprintf(stdout,"\t0. Exit\n");
        fprintf(stdout,"~: ");
        fflush(stdout);

        fscanf(stdin,"%d",&choice);
        if(choice==1){
            if(create_table(conn)){
                perror("Unable to Create Table.");
            }
        }

        if(choice==2){}
        if(choice==3){}
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
