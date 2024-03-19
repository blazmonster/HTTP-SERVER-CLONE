/*
    This needs to be compiled in following way only -
    ==> gcc -o createhtml.cgi createhtml.c
    We require createhtml.cgi binary to serve CGI script
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define CGI_GENERATED_HTML "./custom.html"
#define RANDOM(min, max) (min + rand() % (max - min + 1))

int main()
{
    FILE* fd = NULL;

    srand(time(NULL));
    //printf("\n\nCGI code HIT!!!!!! %u\n\n", RANDOM(10000, 20000));
    fd = fopen(CGI_GENERATED_HTML, "w");
    dup2(fileno(fd), STDOUT_FILENO);

    printf("<html>\n");
    printf("\t<head>\n");
    printf("\t\t<title>cgi</title>\n");
    printf("\t</head>\n");
    printf("\t<body>\n");
    printf("\t\t<h1>CGI Web Page</h1>\n");
    printf("\t\t<p>web server's CGI served this page!</p>\n");
    printf("\t\t<p>This page has unique id: %d</p>\n", RANDOM(10000, 20000));
    printf("\t</body>\n");
    printf("</html>\n");
    
    fclose(fd);
    return 0;
}
