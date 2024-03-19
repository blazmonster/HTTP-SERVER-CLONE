#include "helper.h"

#define LANDING_PAGE "./index.html"
#define ERROR_PAGE "./error.html"
#define CGI_GENERATED_HTML "./custom.html"

char *HTTPStatusCodeToStr(int code)
{
    switch (code)
    {
    case CODE_200:
        return "200 OK";
    case CODE_400:
    default:
        return "400 Bad Request";
    }
}

int parseHTTPRequest(char *buf, http_request_t *req)
{
    char *method = NULL;
    char *path = NULL;
    char *protocol = NULL;
    int ret = 0;

    method = strtok(buf, " ");
    path = strtok(NULL, " ");
    protocol = strtok(NULL, "\n");

    strcpy(req->dir, path);
    if (!strcmp(path, "/?")) {
        ret = CGI_REQ;
    }
    if (!strcmp(method, "GET")) {
        req->method = GET;
    }
    if (!strcmp(protocol, "HTTP/1.1")) {
        req->protocol = 0;
    }
    return ret;
}

int fileRead(char *file_path, char *msg)
{
    FILE *fd = NULL;
    int ret = 0;

    fd = fopen(file_path, "r");
    ret = fread(msg, sizeof(char), MAX_BUF_SIZE, fd);
    //printf("Number of items read=%u\n\n", ret);
    fclose(fd);
    return 0;
}

void createHTTPResponse(http_request_t *req, http_response_t *resp, char *msg, unsigned long tid)
{
    char *content_type = "text/html";
    char *file_path = NULL;
    int is_cgi_req = 0;
    int offset = 0;

    memset(msg, 0, MAX_BUF_SIZE);
    memset(resp, 0, RESPONSE_SIZE);
    if (!strcmp(req->dir, "/?")) {
        is_cgi_req = 1;
        sleep(2);
        resp->status_code = CODE_200;
        file_path = CGI_GENERATED_HTML;
    } else if (!strcmp(req->dir, "/") ||
                !strcmp(req->dir, "/favicon.ico")) {
        resp->status_code = CODE_200;
        file_path = LANDING_PAGE;
    } else {
        resp->status_code = CODE_400;
        file_path = ERROR_PAGE;
    }
    fileRead(file_path, msg);

    /*
    ----------------------
        HTTP/1.1 200 OK
        Server-type:
        Content-type:
        Content-length:
        .
        .
        .

        {response body}
    ----------------------
        i.e HTTP/1.1 200 OK\r\n\r\n{msg}
    */
    //TODO: scale below response buffer to accomodate response headers as well
    offset += sprintf(resp->response_buffer, "HTTP/1.1 %s\r\n",
                            HTTPStatusCodeToStr(resp->status_code));
    offset += sprintf(resp->response_buffer + offset, "Content-Type:%s", content_type);
    offset += sprintf(resp->response_buffer + offset, "%s", "\r\n\r\n");
    offset += sprintf(resp->response_buffer + offset, "%s", msg);

    if (is_cgi_req) {
        remove(file_path);
    }
}