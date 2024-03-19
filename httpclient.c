/*
    Author: Jeeban Sethi
    Exeperience: 3+ yrs
    Designation: Senior Software Engineer

    compilation steps => gcc -o httpclient httpclient.c connection.c
*/

#include "connection.h"
#include <string.h>
#include <unistd.h>

int main()
{
    host_params_t host;
    char buffer[MAX_BUF_SIZE];
    int ret = 0;
    memset(&host, 0, sizeof(host_params_t));
    memset(buffer, 0, MAX_BUF_SIZE);

    host.host_type = CLIENT;
    API_openSocket(&host, SERVER_PORT); //socket created
    ret = connect(host.host_socket, (struct sockaddr *)&host.host_addr, host.addr_size);
    if (ret != 0)
        printf("Failed to connect to server!!!\n");

    sprintf(buffer, "%s", "GET / HTTP/1.1");

    write(host.host_socket, buffer, MAX_BUF_SIZE);
    memset(buffer, 0, MAX_BUF_SIZE);
    read(host.host_socket, buffer, MAX_BUF_SIZE);

    printf("%s\n", buffer);

    return 0;
}