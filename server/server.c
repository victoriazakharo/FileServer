#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

long read_file(char **file_contents, const char *filename)
{
    char filepath[100];
    sprintf(filepath, "files/%s", filename);
    FILE *file = fopen(filepath, "r");

    if (file == NULL)
    return -1;

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    *file_contents = malloc(fsize);
    fread(*file_contents, fsize, 1, file);
    fclose(file);
    return fsize;
}

int main()
{
    int sockfd, newsockfd, portno, n;
    socklen_t clilen;
    char filename[256];
    char *filecontents;
    struct sockaddr_in serv_addr, cli_addr;
    long filesize;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 2027;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    n = read(newsockfd, filename, 255);

    FILE *file = fopen("log.txt", "w");
    fprintf(file, "Filename: %s\n", filename);

    filesize = read_file(&filecontents, filename);

    fprintf(file, "File size: %ld\n", filesize);

    n = write(newsockfd, &filesize, sizeof(long));
    n = write(newsockfd, filecontents, filesize);

    fclose(file);
    close(newsockfd);
    close(sockfd);

    return 0;
}
