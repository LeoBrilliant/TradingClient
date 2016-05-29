/*
 * TradingClient.h
 *
 *  Created on: 2016Äê5ÔÂ29ÈÕ
 *      Author: LeoBrilliant
 */

#ifndef TRADINGCLIENT_H_
#define TRADINGCLIENT_H_

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

#define MAXADDRLEN 256
#define BUFLEN 128

#define SERVER_PORT 20000

int connect_retry(int, const struct sockaddr *, socklen_t);

int TradingClient();

#endif /* TRADINGCLIENT_H_ */
