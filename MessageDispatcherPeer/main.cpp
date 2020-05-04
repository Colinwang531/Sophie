#include <iostream>
#include "MessageQueue/AsynchronousClient.h"
using AsynchronousClient = mq::module::AsynchronousClient;

int main()
{
	AsynchronousClient* c{ new(std::nothrow) AsynchronousClient };

	if (c)
	{
		c->startClient("127.0.0.1");
		getchar();
		c->stopClient();
	}

    return 0;
}