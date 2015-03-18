#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void save_file(char *text, long length, const char *filename)
{
    char filepath[100];
    sprintf(filepath, "downloads/%s", filename);
    FILE *file = fopen(filepath, "w");
    fwrite(text, length, 1, file);
    fclose(file);
}

int main()
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char filename[255] = "dog.txt", *filecontents;
    long filesize;
    int filename_length;

    portno = 2027;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname("127.0.0.1");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server -> h_addr, (char *)&serv_addr.sin_addr.s_addr, server -> h_length);
    serv_addr.sin_port = htons(portno);
    connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));

    n = write(sockfd, filename, 255);
    n = read(sockfd, &filesize, sizeof(long));
    printf("Size: %ld", filesize);
    filecontents = malloc(filesize);
    n = read(sockfd, filecontents, filesize);
    close(sockfd);

    save_file(filecontents, filesize, filename);
    return 0;
}
