#ifndef __HELPER_H__
#define __HELPER_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define REQUEST_SIZE sizeof(http_request_t)
#define RESPONSE_SIZE sizeof(http_response_t)

#define MAX_PATH_SIZE 32
#define NAME_LEN 16
#define MAX_BUF_SIZE 1024

#define CGI_REQ 1
#define IS_CGI_REQUEST(ret) (ret? 1 : 0)

enum {
    GET = 0,
    POST,
    PUT
};

enum {
    CODE_200 = 200,
    CODE_400 = 400
};

typedef struct {
    int protocol;
    int method;
    char dir[MAX_PATH_SIZE];
} http_request_t;

typedef struct {
    //char protocol_name[NAME_LEN];
    int status_code;
    char response_buffer[MAX_BUF_SIZE];
} http_response_t;


extern int parseHTTPRequest(char *buf, http_request_t *req);
extern int fileRead(char *file_path, char *msg);
extern void createHTTPResponse(http_request_t *req, http_response_t *resp, char *msg, unsigned long tid);
extern char *HTTPStatusCodeToStr(int code);

#endif //__HELPER_H__