/*
 * main.cpp
 *
 *  Created on: 2016Äê5ÔÂ8ÈÕ
 *      Author: LeoBrilliant
 */

#include "TradingClient.h"
#include "UnitTest/TestDataFactory.h"
#include "Serialization.h"

using namespace std;

int TradingClient()
{
	struct sockaddr_in server_addr;
	socklen_t socklen = sizeof(server_addr);
	int sockfd, err;
	char server_ip[] = "192.168.31.134";
	int ret;
	vector<int> vsock;
	const int counter = 10;

	char buf[1024];
	int len = 0;

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
		Order * op = GenOrder();
		op->SetDirection(DirectionType::SELL);
		StringType strOrder = SerializeOrder(op);
		sprintf(buf, "%s: %d", strOrder.c_str(), vsock[i]);
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


