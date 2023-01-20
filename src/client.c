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

/*
 *      Remove given section from string. Negative len means remove
 *      everything up to the end. Credits to M Ohem @ stackoverflow
 *      
 */
int str_cut(char *str, int begin, int len)
{
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return len;
}

int examining_data(dpiStmt *statement, dpiConn *conn, dpiContext *context, dpiData *col_value, dpiVar *var){
    dpiQueryInfo *info;
    int column=1;
    
    if(dpiStmt_define(statement,column,var)){
        printError(context,&error_info);
        return -1;
    }
    
    //populate metadata struct
    if(dpiStmt_getQueryInfo(statement,column,info)!=DPI_SUCCESS){
        printError(context,&error_info);
        return -1;
    }
    //Sorting data
    if(dpiConn_newVar(conn,info->typeInfo.oracleTypeNum,info->typeInfo.defaultNativeTypeNum,1,info->typeInfo.clientSizeInBytes,1,0,info->typeInfo.objectType,&var,&col_value)!=DPI_SUCCESS){
        printError(context,&error_info);
        return -1;
    }
    
    return 0;
}

//Routine to execute the SQL_string query
int statement_exec_routine(dpiConn *conn, dpiContext *context, char *SQL_string){
    dpiStmt *statement;
    dpiData *col_value;
    dpiVar *var;
    uint32_t bufferRowIndex;
    dpiNativeTypeNum nativeTypeNum;
    int found;


    //CHECK IF CONNECTION IS HEALTHY
    if(dpiConn_ping(conn)==DPI_FAILURE){
        fprintf(stderr,"[Connection is not healthy]\n");
        return -1;
    }
    else fprintf(stdout,"[HEALTHY CONNECTION]\n");

    //PREPARING STATEMENT
    if (dpiConn_prepareStmt(conn, 0, SQL_string, strlen(SQL_string), NULL, 0, &statement)!=DPI_SUCCESS){

        }
    else fprintf(stdout,"[STATEMENT SUCCESSFULLY PREPARED]\n");
    fflush(stdout);
    
    //EXECUTING STATEMENT
    if (dpiStmt_execute(statement, DPI_MODE_EXEC_DEFAULT, NULL) != DPI_SUCCESS){
        printError(context,&error_info);
        return -1;
    }
    else fprintf(stdout,"[STATEMENT SUCCESSFULLY EXECUTED]\n");
    fflush(stdout);
    
    //To do: loop to display db stdout response messages and rows.
    while (0) {
        int counter=1;
        if (dpiStmt_fetch(statement, &found, &bufferRowIndex) < 0){
            printError(context,&error_info);
            return -1;
            }
        if (!found)    break;
        if(examining_data(statement, conn, context, col_value,var)<0){
            printf("%.s",col_value->value.asBytes.length,col_value->value.asBytes.ptr);
        }
            
    }
    
    
    //COMMITTING CHANGES
    if (dpiConn_commit(conn) < 0){
        printError(context,&error_info);
        return -1;
    }
    else fprintf(stdout,"[COMMIT WAS SUCCESSFUL]\n");
    
    //CLEANING UP 
    dpiStmt_release(statement);


    return 0;
}

int import_from_file(dpiConn *conn, dpiContext *context){
    FILE *fp;
    char path[32], buffer[64], **SQL_string=malloc(sizeof(char));
    int choice=-1,size=0, statement_counter=0;
    bool FLAG=TRUE, FLAG_1=FALSE;

    SQL_string[statement_counter]=malloc(sizeof(char));

    fprintf(stdout,"________________________\n");
    fprintf(stdout,"|| Import statements  ||\n");
    fprintf(stdout,"‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
    fprintf(stdout,"Do you wish to continue? [Enter 0 to return to main menu].\n~: ");

    fscanf(stdin,"%d", &choice);
    while ((getchar()) != '\n');

    if(choice==0) return 1;
    
    fprintf(stdout,"Insert the path to your file containing your SQL queries.\nIf the file is within the local directory, provide the name file only.\n~:");

    fscanf(stdin,"%s",path);
    while ((getchar()) != '\n');
    
    if((fp=fopen(path,"r"))==NULL){
        printf("[ERROR]\nFile does not exists/Wrong path.\n");
        return -1;
    }
    
    //consuming buffer newline, because fscanf and fgets differences.
    //while ((getchar()) != '\n');

    while(fgets(buffer,(int)sizeof(buffer),fp)!=NULL){
        int i,j=0;
        //checking if this is the last line of a SQL statement
        for(i=0;i<strlen(buffer);i++){
            //have i reached the end of the SQL query? If so, set the flag.
            if(buffer[i]==';'){
                j=i;
                FLAG_1=TRUE;
            }
            //CHECK IF THE ";" IS IN A STRING->this means cheching it the occurrences of \' before it are odd or even. 
            if(FLAG_1){
                int occurrences=0;
                for(i=0;i<j;i++){
                    if(buffer[i]=='\''){
                        occurrences++;
                    }
                }
                if(occurrences%2!=0){
                    //; is inside a string-> fake positive
                    FLAG_1=FALSE;
                }
            }
        }

        size=size+strlen(buffer);
        if(FLAG_1){
            //reached the end of the query, need to remove the ";"
            //size of what to append and allocate changes accordingly
            size--;
            str_cut(buffer,j,1);
        }

        SQL_string[statement_counter]=realloc(SQL_string[statement_counter],size*sizeof(char));
        strcat(SQL_string[statement_counter],buffer);

        if(FLAG_1){
            //changing memory pointer, allocating new element...
            statement_counter++;
            SQL_string[statement_counter]=malloc(sizeof(char));
            //RESET SIGNAL
            FLAG_1=FALSE;
        }
    }
    //I have all the statements, it's time to run them all.

    return 0;
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

    fprintf(stdout,"Insert your SQL statement. Enter no characters to end the input.\n\n");
    fflush(stdout);

    //consuming buffer newline, because fscanf and fgets differences.
    while ((getchar()) != '\n');

    //Getting user_input SQL queries
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

    if(statement_exec_routine(conn,context,user_SQL)<0){
        printf("Unable to perform statement execution routine.\n");
        return -1;
    }
    
    free(user_SQL);
    
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
        if(choice==2){
            if(import_from_file(conn,context)==-1){
                printf("Unable to import statement.\n");
            }
        }
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