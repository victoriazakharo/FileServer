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

int main(int argc, char **argv)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char filename[256], ip_addr[256],*filecontents;
    long filesize;
    int filename_length;

    if (argc > 2) {
        strcpy(filename, argv[1]);
        strcpy(ip_addr, argv[2]);
    } else {
        printf("Arguments are incorrect\n");
    }

    portno = 2027;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname(ip_addr);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server -> h_addr, (char *)&serv_addr.sin_addr.s_addr, server -> h_length);
    serv_addr.sin_port = htons(portno);
    connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));

    n = write(sockfd, filename, 255);
    n = read(sockfd, &filesize, sizeof(long));
    if (filesize < 0) {
        printf("File not found\n");
        close(sockfd);
        return;
    }
    long fullsize = filesize;
    filecontents = malloc(filesize);
    while(filesize > 0) {
        long bytes_read = read(sockfd, filecontents, filesize);
        filecontents += bytes_read;
        filesize -= bytes_read;
        printf("%ld\n", bytes_read);
    }
    filecontents -= fullsize;

    FILE *file = fopen("csuper.txt", "w");
    fwrite(filecontents + sizeof(char)*(fullsize-100), sizeof(char), 100, file);
    fclose(file);
    close(sockfd);

    save_file(filecontents, fullsize, filename);
    return 0;
}
