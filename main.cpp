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
#include <vector>
#include <iostream>
#include "ConnectRetry.h"

#define MAXADDRLEN 256
#define BUFLEN 128

#define SERVER_PORT 20000

extern int connect_retry(int, const struct sockaddr *, socklen_t);

using namespace std;

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
	int ret;
	vector<int> vsock;
	const int counter = 10;

	char buf[1024];
	int len = 0;
//	hint.ai_flags = 0;
//	hint.ai_family = 0;
//	hint.ai_socktype = SOCK_STREAM;
//	hint.ai_addrlen = 0;
//	hint.ai_canonname = NULL;
//	hint.ai_addr = NULL;
//	hint.ai_next = NULL;

	//if((err = getaddrinfo()))

	for(int i = 0; i < counter; ++i)
	{
		sockfd = -1;
		if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			err = errno;
		}

		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		inet_aton(server_ip, &server_addr.sin_addr);
		server_addr.sin_port = htons(SERVER_PORT);

		ret = connect_retry(sockfd, (const sockaddr*)&server_addr, socklen);
		if(ret != 0)
			err = errno;
		else
		{
			vsock.push_back(sockfd);
			//print_uptime(sockfd);
		}
	}

	for(int i = 0; i < counter; ++i)
	{
		sprintf(buf, "%s: %d", server_ip, vsock[i]);
		len = strlen(buf);
		write(vsock[i], buf, len);
		printf("write: %s\n", buf);
	}

	printf("write finished, sleep 1s\n");
	sleep(1);

	for(int i = 0; i < counter; ++i)
	{
		close(vsock[i]);
	}

	return err;
}


