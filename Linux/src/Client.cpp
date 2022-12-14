#include "Client.h"

#include <iostream>

#include <cstdio>
#include <cstring>

#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int sockfd;
char ip[255];

int client_start() {

    setup_client_socket();

    char str[1024];
    int nbytes = ask_string(str);
    if (nbytes <= 1) {
        send_requests("Hello server!\n", 14);
    }
    else {
        send_requests(str, nbytes);
    }

    close(sockfd);

    return 0;
}

int ask_string(char *str) {
    printf("Enter request (up to 1024 characters):\n");
    fgets(str, sizeof(str), stdin);

    int i = 0;
    while (str[i] != '\n' && str[i] != '\0') i++;
    return ++i;
}

void setup_client_socket() {

    int len;
    struct sockaddr_in address;
    int result;

//2. Создайте сокет для клиента:

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

//3. Назовите сокет по согласованию с сервером:

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(get_ip());
    address.sin_port = htons(9734);
    len = sizeof(address);

//4. Соедините ваш сокет с сокетом сервера:

    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1) {
        perror("Error while connecting to server");
        exit(1);
    }
}

void send_requests(char *send_str, int nbytes) {
    char ch;

//5. Теперь вы можете читать и писать через sockfd:

    write(sockfd, send_str, nbytes);
    while (read(sockfd, &ch, 1)) {
        printf("%c", ch);
    }
}

char *get_ip() {
    int fd;
    if((fd = open("ip.txt", O_RDONLY | O_CREAT, 0666)) < 0){
        printf("Can\'t open file\n");
        exit(-1);
    }

    int nbytes = read(fd, &ip, 255);

    ip[nbytes-1] = '\0';

    close(fd);

    return ip;
}
