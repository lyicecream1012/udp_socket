#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>


#define MYPORT 8887
#define BUFFER_SIZE 1024*16

#define ERR_EXIT(m) \
    do { \
    perror(m); \
    exit(EXIT_FAILURE); \
    } while (0)

char* strjoin(char *str1, char *str2);

void echo_ser(int sock)
{
    char recvbuf[BUFFER_SIZE] = {0};
    struct sockaddr_in peeraddr;
    socklen_t peerlen;
    int n;
    int fd;
    char *filepath = "../log/";
    char *filename = "../log/test.log";

    while (1)
    {
        peerlen = sizeof(peeraddr);
        memset(recvbuf, 0, sizeof(recvbuf));
        n = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&peeraddr, &peerlen);
        if (n <= 0)
        {
            if (errno == EINTR)
            continue;

            ERR_EXIT("recvfrom error");
        }
        else if(n > 0)
        {
            if (-1 == access(filepath, F_OK))
            {
                mkdir(filepath,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
            }
                
            fd=open(filename,O_RDWR|O_CREAT|O_APPEND,S_IREAD|S_IWRITE);
            if (-1 != fd)
            {
                write(fd,recvbuf,n);
            }
            /*close(fd);*/
            /*printf("Received data：%s\n",recvbuf);*/
            sendto(sock, recvbuf, n, 0, (struct sockaddr *)&peeraddr, peerlen);
            /*printf("Sent back data：%s\n",recvbuf);*/
        }
    }
    close(sock);
}

int main(void)
{
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        ERR_EXIT("socket error");

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("Listening port:%d\n",MYPORT);
    if (bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("bind error");

    echo_ser(sock);

    return 0;
}
