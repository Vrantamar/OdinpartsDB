/*
 *    Akvarion, Jan 2023
*/

#include "../odpi-main/include/dpi.h" //PATH TO ODPI-C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FALSE 0
#define TRUE 1
#define DEBUG_USER_INPUT 0
#define CONN_STRING "172.17.0.2:1521/XEPDB1"    // this mf took 2h of research to find

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

//take a string out from the buffer and copy it in a string.
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
void get_user_info(char *username, char *password){
    size_t size=0;
    char buffer[32];
    fprintf(stdout,"-----------------------------------------\n");
    fprintf(stdout,"Username: ");

    size=(size_t)fscanf(stdin,"%s", buffer);
    username=(char*)malloc((size+1)*sizeof(char));
    //strcpy(username,buffer);
    size++;
    buff_out(username,buffer,&size);
    
    fprintf(stdout,"\nPassword: ");
    
    size=(size_t)fscanf(stdin,"%s", buffer);
    password=(char*)malloc((size+1)*sizeof(char));
    //strcpy(password,buffer);
    size++;
    buff_out(password,buffer,&size);
    

    fflush(stdout);
    fflush(stdin);
}

int main(int argc, char** argv){
    char *userName=NULL;
    char *password=NULL;
    dpiStmt *statement;
    dpiConn *conn;
    dpiContext *context=NULL;
    dpiErrorInfo errorInfo;

    //GET USER CREDENTIALS
    if(DEBUG_USER_INPUT){
        //strings obtained in this way do not work, although they are nul terminated strings, so... uhm... skip.
        get_user_info(userName,password);
    }
    else{
        userName="ODINPARTS";
        password="odinparts";
    }

    //Creazione contesto per la connessione
    if(dpiContext_createWithParams(DPI_MAJOR_VERSION, DPI_MINOR_VERSION, NULL,&context,&errorInfo)!=DPI_SUCCESS){
        printError(context,&errorInfo);
        return EXIT_FAILURE;
    }

    printf("[CONTEXT CREATED SUCCESSFULLY]\n");

    //CONNECTION CREATION
    if(dpiConn_create(context, userName, strlen(userName), password, strlen(password), CONN_STRING, strlen(CONN_STRING),NULL,NULL,&conn)!=DPI_SUCCESS){
        printError(context,&errorInfo);
        return EXIT_FAILURE;
    }
    printf("[CONNECTION CREATED SUCCESSFULLY]\n");


    printf("[CLEANING UP..]\n");
    //CLOSING CONNECTION
    dpiConn_release(conn);
    //DESTROYING CONTEXT
    dpiContext_destroy(context);

    printf("[DONE]\n");
    return 0;
}
