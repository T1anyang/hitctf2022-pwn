#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int count = 0;
    printf("Status: 404\r\n");
    printf("Content-type: text/html\r\n");
    printf("\r\n");
    printf("Hello world!<br>");
    printf("Request number %d.<br>", 123456);
    printf("%s\n", getenv("REQUEST_URI"));
    return 404;
}