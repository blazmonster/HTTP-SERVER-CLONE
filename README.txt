compilation ==> gcc -o http httpserver.c connection.c helper.c

connection library ==> for any server connection (http, ftp, dns etc.)
helper library(http server) ==> for parsing client request, send response etc
------------------
http server ==> write protocol compliant server 



1. connection.h  ==> connection.o
-----------------
    i. createSocket()
    ii. handleNewConnection()
2. helper.h ==> helper.o
--------------
    i. request/response structure
    ii. parseRequest()


3. httpserver.c
---------------
launchServer(HTTP, 10.29.49.122, 8080);
    a. socket()
    b. bind()
    c. listen()
    d. accept()

===========
Challenges :-
===========
1. Concurrent fulfilment of requests coming from clients is obtained by 
    the use of pthread to handle new connection in server.
2. accept is blocking call, so select() sys call is used to obtain synchronous I/O.
    select() is more preferable where many I/O operations happen. poll() can be considered.
3. Response buffer needs to be scaled since earlier it was hardcoded for something like
    {HTTP/1.1 200 OK\r\n\r\n[body]} . Hence to accomodate all the headers we need some machanism
    to scale this.