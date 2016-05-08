/*
 * main.cpp
 *
 *  Created on: 2016Äê5ÔÂ8ÈÕ
 *      Author: LeoBrilliant
 */

#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "ConnectRetry.h"

#define MAXADDRLEN 256
#define BUFLEN 128

#define SERVER_PORT 20000

extern int connect_retry(int, const struct sockaddr *, socklen_t);

void print_uptime(int sockfd)
{
	int n;
	char buf[BUFLEN];

	while((n = recv(sockfd, buf, BUFLEN, 0)) > 0)
	{
		write(STDOUT_FILENO, buf, n);
	}

	if(n < 0)
	{
		perror("recv error");
	}
}

int main()
{
	//struct addrinfo * ailist, *aip;
	//struct addrinfo hint;
	struct sockaddr_in server_addr;
	socklen_t socklen = sizeof(server_addr);
	int sockfd, err;
	char server_ip[] = "192.168.31.134";

//	hint.ai_flags = 0;
//	hint.ai_family = 0;
//	hint.ai_socktype = SOCK_STREAM;
//	hint.ai_addrlen = 0;
//	hint.ai_canonname = NULL;
//	hint.ai_addr = NULL;
//	hint.ai_next = NULL;

	//if((err = getaddrinfo()))

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		err = errno;
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	inet_aton(server_ip, &server_addr.sin_addr);
	server_addr.sin_port = htons(SERVER_PORT);
	if(connect_retry(sockfd, (const sockaddr*)&server_addr, socklen) < 0)
		err = errno;
	else
	{
		print_uptime(sockfd);
	}

	return 0;
}


