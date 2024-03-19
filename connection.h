#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/select.h>
#include <time.h>

#define SERVER_PORT 8008
#define WAIT_QUEUE_SIZE 10
#define MAX_BUF_SIZE 1024

#define FAIL -1
#define SUCCESS 0
#define CLIENT 0
#define SERVER 1

#define PRINT_INFO printf("INFO: ");printf
#define PRINT_ERR printf("ERROR: ");printf
#define CHECK_API(api_call, fmt) \
                        do {\
                            if (api_call <= -1)\
                                {\
                                    printf("ERROR: ");\
                                    printf(fmt);\
                                    printf("\n");\
									exit(0);\
                                }\
                            } while(0);

typedef void* (*callbackHandler) (void* );
typedef struct {
    /* host related info structure */
    int host_type; /* SERVER or CLIENT */
    struct sockaddr_in host_addr;
    socklen_t addr_size;
    int host_socket;
    pthread_t thread_id;
    /* remote client related info structure */
    struct sockaddr_in remote_addr;
    int remote_socket;
    int conn_id;
    callbackHandler server_type_cb_handler;
} server_params_t;

typedef struct {
    int host_type;
    struct sockaddr_in host_addr;
    socklen_t addr_size;
    int host_socket;
    pthread_t thread_id;
} host_params_t;

extern int API_openSocket(void *param, int port);
extern int API_handleNewConnection(server_params_t *srv);
extern int API_handleNewConnection2(server_params_t *srv);

#endif //__CONNECTION_H__
