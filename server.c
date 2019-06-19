/*************************************************************************
	> File Name: fuwu.c
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年06月18日 星期二 19时40分27秒
 ************************************************************************/

#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<pwd.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<netinet/in.h>
#include<sys/ipc.h>

#define MYPORT 8888
#define MYKEY 12345
#define SIZE 10240


int main() {
    char buf[100];
    memset(buf,0,100);
    int server_sockfd,client_sockfd;
    socklen_t server_len, client_len;
    struct sockaddr_in server_sockaddr,client_sockaddr;

    server_sockfd = socket(AF_INET, SOCK_STREAM,0);
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_len = sizeof(server_sockaddr);

//    int on = 1;
//    setsockopt (server_sockfd, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(on));


    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,server_len) == -1)
    {
        perror("bind");
        exit(1);
    }

    if(listen(server_sockfd, 10) < 0) {
        perror("listen");
        exit(1);
    }
    pid_t  pid;
    while((client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_sockaddr,&server_len)) != -1) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            close(server_sockfd);
            char buf[SIZE] = {0};
            char *ip = inet_ntoa(client_sockaddr.sin_addr); // 为啥不用成员
            recv (client_sockfd, buf,sizeof(buf),0);
            printf("%s:%s\n",buf, ip);
            close(client_sockfd);
            exit(0);
        } else {
            close(client_sockfd);
        }
    }

        close(server_sockfd);
        return 0;

    }


/*
    printf("\n%s:%d Login server ! \n\n", inet_ntoa(client_sockaddr.sin_addr), ntohs(client_sockaddr.sin_port));

  pid_t ppid = fork();
    if (ppid == -1) {
        printf("fock 1 failed:");
    }
    else if(ppid == 0) {
        int recvbytes;
       pid_t  pid = fork();

        if (pid == -1) {
            printf("fork 2 failed:");
            exit(1);
        }
        else if(pid == 0) {
            while(1) {
                bzero(buf,100);
                if ((recvbytes = recv(client_sockfd,buf,100,0)) == -1) {
                    perror("read client_sockfd failed:");
                }
                else if (recvbytes != 0) {
                    buf[recvbytes] = '\0';
                    usleep(10000);
                    printf("%s:%d said:%s\n",inet_ntoa(client_sockaddr.sin_addr), ntohs(client_sockaddr.sin_port),buf);

                    if(send(client_sockfd,buf,recvbytes,0) == -1) {
                        perror("send error");
                        break;
                    }
                }
            }
        }
        else if(pid > 0) {

        }
    }
    else if (ppid > 0) {
     close(client_sockfd);
    }
    
   return 0;
}*/
