/*************************************************************************
	> File Name: client.c
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年06月20日 星期四 21时57分10秒
 ************************************************************************/

#include<stdio.h>
#include <stdio.h>
#define MAX_SIZE 1024
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<pwd.h>
#include<unistd.h>
#include<netinet/in.h>

int socket_connect(int port, char * host) {
    int socket_fd;
    struct sockaddr_in dest_addr;
    if ((socket_fd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket() error");
        return -1;
    }
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(host);
    if (connect(socket_fd, (struct sockaddr *)&dest_addr,sizeof(dest_addr)) < 0) {
        perror("connect() error");
        return -1;
    }
    return socket_fd;
}


void sendfile(int sockfd, char *path) {
    FILE *fd = fopen(path, "rb");
    int num_read;
    char data[MAX_SIZE]  = {0};
    while (1) {
        memset(data, 0, MAX_SIZE);
        num_read = fread(data, 1 , MAX_SIZE, fd);
        if (send(sockfd, data, strlen(data), 0) < 0){
            printf("发送失败");   
            fclose(fd);
            return ;
        }
        if (num_read == 0) break;
    }
    fclose(fd);
}


int main(int argc, char **argv) {
    int sockfd, port, sock_file;
    char filename[50] = {0};
    char *p;
    FILE *fp;
    if (argc != 4) {
        printf("输入正确的参数");
        exit(1);
    }

    char *path = argv[3];
    port = atoi(argv[2]);  // 传进来的是字符数组，转为整型存储。
    char *ip = argv[1];

    if (access(path, R_OK) != 0) {  // access() 用于检测文件权限
        printf("文件不可读\n");
        exit(2);
    }
    strcpy(filename, (p = strrchr(path, '/')) ? p + 1 : path);  // strrchr(char * s, int c) 用于找打文件名，即使问价带有路径，第二个参数要注意是 一个字符的整形 表示
    if ((sockfd = socket_connect(port, ip)) < 0) {
        printf("链接失败\n");
        exit(1);
    }
    if ((send(sockfd, filename, strlen(filename), 0)) < 0) {
        printf("send()失败");
    }
    int ack;
    if (recv(sockfd, &ack, sizeof(ack), 0) <= 0 ) {
        perror("recv ack");
    }
    if (ack == 1) {
        if ((sock_file = socket_connect(8090, ip)) < 0) {
            perror("sock_file connect");
            exit(1);
        }
        printf("链接成功\n");
        sendfile(sock_file, path);
        close(sock_file);
    }


    close(sockfd);
    return 0;
}
