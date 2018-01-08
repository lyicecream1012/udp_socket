    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <errno.h>
    #include <string.h>

    #define MYPORT 8887
    #define BUFFER_SIZE 1024*16
    char* SERVERIP = "127.0.0.1";

    #define ERR_EXIT(m) \
        do \
    { \
        perror(m); \
        exit(EXIT_FAILURE); \
        } while(0)

    void echo_cli(int sock)
    {
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(MYPORT);
        servaddr.sin_addr.s_addr = inet_addr(SERVERIP);

        int ret;
        char sendbuf[BUFFER_SIZE] = {0};
        char recvbuf[BUFFER_SIZE] = {0};
        while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
        {

            printf("Send to server：%s\n",sendbuf);
            sendto(sock, sendbuf, strlen(sendbuf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

            ret = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
            if (ret == -1)
            {
                if (errno == EINTR)
                    continue;
                ERR_EXIT("recvfrom");
            }
            printf("Receive from server：%s\n",recvbuf);

            memset(sendbuf, 0, sizeof(sendbuf));
            memset(recvbuf, 0, sizeof(recvbuf));
        }

        close(sock);


    }

    int main(void)
    {
        int sock;
        if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
            ERR_EXIT("socket");

        echo_cli(sock);

        return 0;
    }
