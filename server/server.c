#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

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

void *serve_client(void* pnewsockfd)
{
    int newsockfd = (int)pnewsockfd;
    char filename[256], *filecontents;
    long filesize;
    FILE *file = fopen("log.txt", "w");
    fprintf(file, "Socket: %d\n", newsockfd);

    read(newsockfd, filename, 255);

    filesize = read_file(&filecontents, filename);

    fprintf(file, "Filename: %s\n", filename);
    fprintf(file, "Filesize: %ld\n", filesize);

    fclose(file);

    write(newsockfd, &filesize, sizeof(long));
    write(newsockfd, filecontents, filesize);

    close(newsockfd);
}

#ifdef MULTITHREADED
void serve_client_proxy(int newsockfd)
{
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, &serve_client, (void*)newsockfd);
}
#endif

#ifdef MULTIPROCESS
void serve_client_proxy(int newsockfd)
{
    int pid = fork();
    if(pid == 0) {
        serve_client(newsockfd);
    }
}
#endif

int main()
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 2027;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        serve_client_proxy(newsockfd);
    }

    close(sockfd);
    return 0;
}
