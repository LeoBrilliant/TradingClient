/*
 * main.cpp
 *
 *  Created on: 2016Äê5ÔÂ8ÈÕ
 *      Author: LeoBrilliant
 */

#include "TradingClient.h"
#include "UnitTest/TestDataFactory.h"
#include "Serialization.h"
#include "zmq.hpp"

using namespace std;

int TradingClient()
{
	struct sockaddr_in server_addr;
	socklen_t socklen = sizeof(server_addr);
	int sockfd, err;
	char server_ip[] = "192.168.31.134";
	int ret;
	vector<int> vsock;
	const int counter = 1;

	char buf[1024];
	int len = 0;

	cout << "Connecting to OpenTS" << endl;
	//sockets to OpenTS
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

	// socket to receive orders
	//  Prepare our context and socket
	zmq::context_t context (1);
	zmq::socket_t socket (context, ZMQ_REP);
	socket.bind ("tcp://*:5556");

	cout << "Waiting for New Orders\n" << endl ;
	int msgCounter = 0;
	while (true) {
		zmq::message_t request;

		memset(buf, 0, 1024);

		//  Wait for next request from client
		socket.recv (&request);
		std::cout << "Received Data: " << (char*)(request.data()) << std::endl;

		//  Do some 'work'
		//Order * op = GenOrder();
		//op->SetDirection(DirectionType::SELL);
		//StringType strOrder = SerializeOrder(op);
		//sprintf(buf, "%d\t%s: %d", ++msgCounter, strOrder.c_str(), vsock[0]);
		//sprintf(buf, "%s", strOrder.c_str());

		//sleep(1);
		//sprintf(buf, "%s: %d", (char*)(request.data()), sockfd);
		//sprintf(buf, "%d\t%s: %d", ++msgCounter, "Hello", sockfd);

		Json::Reader reader(Json::Features::strictMode());
		Json::Value input;
		Json::Value jsonOrder;
		Json::Value jsonInst;
		if(reader.parse((char *)(request.data()), input))
		{
			jsonInst["InstructionID"] = "";
			jsonInst["ClientID"] = atoi(input["clientID"].asCString());
			if(input["instType"] == "LimitPriceOrder")
				jsonInst["InstType"] = InstructionType::LIMITPRICEORDER ;

			jsonOrder["Instruction"] = jsonInst;

			jsonOrder["InstrumentID"] = "TestInstrument";

			if(input["direction"] == "BUY")
				jsonOrder["Direction"] = DirectionType::BUY;
			jsonOrder["OffsetFlag"] = OffsetType::OPEN;
			jsonOrder["OrderPrice"] = atof(input["orderPrice"].asCString());
			jsonOrder["Volume"] = atoi(input["volume"].asCString());
			jsonOrder["OrderID"] = "";
			jsonOrder["LocalOrderID"] = msgCounter;
			jsonOrder["VolumeLeft"] = "";
			jsonOrder["OrderStatus"] = "";

			StyledWriter fast_writer;
			StringType strOrder = fast_writer.write(jsonOrder);
			memcpy(buf, strOrder.c_str(), strOrder.size());
			len = strlen(buf);
			++msgCounter;
			ret = write(sockfd, buf, len);
			cout << "write " << msgCounter << "(" << ret << ") : " <<  buf << endl;
			//  Send reply back to client
			zmq::message_t reply (12);
			memcpy (reply.data (), "Order Sended", 12);
			socket.send (reply);
		}
		else
		{
			cout << "Invalid Order" << endl;
			//  Send reply back to client
			zmq::message_t reply (13);
			memcpy (reply.data (), "Invalid Order", 13);
			socket.send (reply);
		}
		//sprintf(buf, "%s", (char*)(request.data()));


	}

	/*
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
	*/
	printf("write finished, sleep 1s\n");
	sleep(1);

	for(int i = 0; i < counter; ++i)
	{
		close(vsock[i]);
	}

	return err;
}


