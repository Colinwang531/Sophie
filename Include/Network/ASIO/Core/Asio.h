//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					ASIO��
//
//		History:						Author									Date										Description
//										������									2020-05-06									����
//

#ifndef FRAMEWORK_NETWORK_ASIO_H
#define FRAMEWORK_NETWORK_ASIO_H

#include "boost/asio.hpp"
using IOContextGroup = std::vector<boost::asio::io_context>;
using IOWorkGroup = std::vector<boost::asio::io_service::work>;
#include "boost/thread.hpp"
#include "Mutex/RWLock.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			class ASIO
			{
			public:
				ASIO(void);
				virtual ~ASIO(void);

			public:
				//����ASIO
				//@Return : ������ֵ
				virtual int startASIO(void);

				//ֹͣASIO
				virtual int stopASIO(void);

				//��ȡTCP socket
				//@Return : TCP socket
				boost::asio::ip::tcp::socket getTCPSocket(void);

				//��ȡUDP socket
				//@port : �����˿ں�
				//@Return : socket
				//@Comment : 0 < portʱ���ڼ���
				boost::asio::ip::udp::socket getUDPSocket(const unsigned short port = 0);

				//��ȡTCP����
				//@port : �����˿ں�
				//@Return : ����
				//@Comment : 0 < portʱ���ڼ���
//				boost::asio::ip::tcp::acceptor getTCPListener(const unsigned short port = 0);

//			private:
				//��ȡIO
				//@Return : IO
				boost::asio::io_context& getIdleIO(void);

			private:
				IOContextGroup ioctxGroup;
				IOWorkGroup ioworkGroup;
				boost::thread_group iothreadGroup;
				int idleNumber;
				SharedMutex mtx;
			};//class ASIO
		}//namespace asio
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_ASIO_H
