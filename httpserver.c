/*
    Author: Jeeban Sethi
    Exeperience: 3+ yrs
    Designation: Senior Software Engineer

    compilation steps => gcc -o http httpserver.c connection.c helper.c
*/

#include "connection.h"
#include "helper.h"

#define SERVER_RUNNING 1

typedef struct {
    server_params_t server_params;
    http_request_t client_request;
    http_response_t srv_response;
} server_info_t;

static inline void handleCGIRequest()
{
    pid_t cid = fork();
    if (cid == 0) {
        execve("./createhtml.cgi", NULL, NULL);
    }
}

static inline void* handleHTTPConnection(void *arg)
{
    int remote_socket = ((server_params_t *)arg)->remote_socket;
    char req_buffer[MAX_BUF_SIZE] = {0};
    char resp_buffer[MAX_BUF_SIZE] = {0};
    http_request_t client_request;
    http_response_t srv_response;
    int bytes=0;
    int ret = 0;

    struct timespec start, end;
    double time_taken = 0;

    free(arg);
    clock_gettime(CLOCK_MONOTONIC, &start);
    ret = read(remote_socket, req_buffer, MAX_BUF_SIZE);
    if (ret == -1)
        printf("\n\n#####read from buffer failed\n\n");
    if (IS_CGI_REQUEST(parseHTTPRequest(req_buffer, &client_request))) {
       handleCGIRequest();
    }
    //send response
    createHTTPResponse(&client_request, &srv_response, resp_buffer, 0);
    ret = write(remote_socket, srv_response.response_buffer, MAX_BUF_SIZE);
    if (ret == -1)
        printf("\n\n#####write to buffer failed\n\n");
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    PRINT_INFO("Request from client for method: GET, path: %s\n", client_request.dir);
    PRINT_INFO("Response sent to client for method: GET, path: %s\n"
            "(time taken %.9f sec)\n\n", client_request.dir, time_taken);
    close(remote_socket);
    pthread_exit(NULL);
}

static inline int startHTTPServer(server_params_t *srv)
{
    int ret = -1;
    ret = API_openSocket(srv, SERVER_PORT);
    if (ret < 0) {
        PRINT_ERR("Opening socket failed.\n");
        return FAIL;
    }
    PRINT_INFO("Socket opened @fd: %d and server started.\n\n", srv->host_socket);
    while(SERVER_RUNNING) {
        ret = API_handleNewConnection(srv); 
        if (ret < 0) {
            PRINT_ERR("Handle new connection failed.\n");
            return FAIL;
        }
    }
    return 0;
}

int main()
{
    server_params_t srv;
    memset(&srv, 0, sizeof(server_params_t));
    srv.host_type = SERVER;
    srv.server_type_cb_handler = handleHTTPConnection;

    startHTTPServer(&srv);
    
    return 0;
}
