#include <iostream>
#include "MessageQueue/AsynchronousUpstreamServer.h"
using AsynchronousServer = mq::module::AsynchronousServer;
using AsynchronousUpstreamServer = mq::module::AsynchronousUpstreamServer;

int main()
{
    AsynchronousServer* s{ new(std::nothrow) AsynchronousUpstreamServer };

    if (s)
    {
		s->startServer();
		getchar();
		s->stopServer();
    }

    return 0;
}