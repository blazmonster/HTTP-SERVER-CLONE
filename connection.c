#include "connection.h"

int API_openSocket(void *param, int port)
{
    host_params_t *host = (host_params_t *) param;
    int ret = -1;
    ret = socket(AF_INET, SOCK_STREAM, 0);
    if (ret < 0)
        return FAIL;
    host->host_socket = ret;
    // Setting up server address struct
    host->host_addr.sin_family = AF_INET;
    host->host_addr.sin_port = htons(port);
    host->addr_size = sizeof(struct sockaddr_in);
    if (host->host_type == SERVER) {
        host->host_addr.sin_addr.s_addr = INADDR_ANY;
        ret = bind(host->host_socket,
            (const struct sockaddr *)&host->host_addr,
                host->addr_size);
        if (ret < 0)
            return FAIL;
        ret = listen(host->host_socket, WAIT_QUEUE_SIZE);
        if (ret < 0)
            return FAIL;
    } else if (host->host_type == CLIENT) {
        host->host_addr.sin_addr.s_addr = inet_addr("192.168.1.2");
    }
    return host->host_socket;
}

int API_handleNewConnection(server_params_t *srv)
{
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int ret = -1;
    server_params_t *p_srv = NULL;

	//PRINT_INFO("Waiting for new connections .......\n");
	ret = accept(srv->host_socket, (struct sockaddr *)&srv->remote_addr, &addr_size);
	if (ret < 0) {
		return FAIL;
	}
	srv->remote_socket = ret;
    //srv->conn_id = getConnectionId();
	PRINT_INFO("Server accepts client connection @socket: %d @client IP: %s\n",
			srv->remote_socket, inet_ntoa(srv->remote_addr.sin_addr));
	//spawn thread for handling a new connection
	p_srv = (server_params_t *)malloc(sizeof(server_params_t));
	memcpy(p_srv, srv, sizeof(server_params_t));
	ret = pthread_create(&srv->thread_id, NULL, srv->server_type_cb_handler, (void *)p_srv);
	if (ret != 0) {
		printf("\n\n####pthread_create() failed#####\n\n");
		return FAIL;
	}
	return SUCCESS;
}

int API_handleNewConnection2(server_params_t *srv)
{
    socklen_t addr_size = sizeof(struct sockaddr_in);

    fd_set readfds;
    FD_ZERO(&readfds); // FD_ZERO() clears a set and FD_CLR() removes a fd
    FD_SET(srv->host_socket, &readfds);

    printf("Waiting for client connections @select().......\n");
    select(FD_SETSIZE, &readfds, NULL, NULL, NULL);

    if (FD_ISSET(srv->host_socket, &readfds)) {
        srv->remote_socket =
            accept(srv->host_socket, (struct sockaddr *)&srv->remote_addr, &addr_size);
        printf("Server accepts client connection @socket: %d @client IP: %s .......\n",
            srv->remote_socket, inet_ntoa(srv->remote_addr.sin_addr));

        //spawn thread for handling a new connection
		//TODO: Need to create heap memory for passing thread arg
        pthread_create(&srv->thread_id, NULL, srv->server_type_cb_handler, (void *)srv);
    }
    return srv->remote_socket;
    //close(srv->srv_socket);
}
