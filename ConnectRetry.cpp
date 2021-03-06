/*
 * ConnectRetry.h
 *
 *  Created on: 2016��5��8��
 *      Author: LeoBrilliant
 */

#ifndef CONNECTRETRY_CPP_
#define CONNECTRETRY_CPP_

#include <sys/socket.h>
#include "TradingClient.h"

#define MAXSLEEP 128

int connect_retry(int sockfd, const struct sockaddr * addr, socklen_t alen)
{
	int nsec;

	for(nsec = 1; nsec <= MAXSLEEP; nsec <<= 1)
	{
		if(connect(sockfd, addr, alen) == 0)
			return 0;

		if(nsec <= MAXSLEEP / 2)
			sleep(nsec);
	}

	return -1;
}



#endif /* CONNECTRETRY_CPP_ */
