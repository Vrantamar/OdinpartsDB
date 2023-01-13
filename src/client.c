/*
 *    Akvarion, Jan 2023
*/

// #include "../odpi-main/include/dpi.h" //PATH TO ODPI-C COMPILED FOLDER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONN_STRING "172.17.0.2:1521/XEPDB1"

//Funzione per mostrare a video errori nell'istanziazione del contesto
void printError(dpiContext *context, dpiErrorInfo *ErrorInfo){
    dpiContext_getError(context, ErrorInfo);
    fprintf(stderr,"-----------------------------------------\n");
    fprintf(stderr, "[FAILED]\n");
    fprintf(stderr, "\tFN: %s\n", ErrorInfo->fnName);
    fprintf(stderr, "\tACTION: %s\n", ErrorInfo->action);
    fprintf(stderr, "\tMSG: %.*s\n", ErrorInfo->messageLength, ErrorInfo->message);
    fflush(stderr);
}


void get_user_info(const char *username, const char *password){
    printf("-----------------------------------------\n");
    printf("Username: ");
    scanf("%s", username);
    printf("\nPassword: ");
    scanf("%s", password);
}

int main(int argc, char** argv){
    dpiData intColValue, stringColValue;
    uint32_t rowidStrLength;
    const char *rowidStr;
    uint64_t rowCount;
    dpiRowid *rowid;
    dpiStmt *statement;
    dpiConn *conn;
    dpiContext *context=NULL;
    dpiErrorInfo errorInfo;
    uint32_t userNameLength;
    const char *userName;
    const char *password;

    //Creazione contesto per la connessione
    if(dpiContext_createWithParams(DPI_MAJOR_VERSION, DPI_MINOR_VERSION, NULL,&context,&errorInfo)!=DPI_SUCCESS){
        printError(context,&errorInfo);
        return EXIT_FAILURE;
    }

    printf("[CONTEXT CREATED SUCCESSFULLY]\n");

    //GET USER CREDENTIALS, TODO
    //get_user_info(userName,password);
    userName="ODINPARTS";
    password="odinparts";
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

    printf("[DONE]");
    return 0;
}