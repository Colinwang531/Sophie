#include "boost/bind.hpp"
#include "Data/Buffer.h"
#include "Network/ASIO/Session/TCPTargetSession.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			TCPTargetSession::TCPTargetSession(
				boost::asio::ip::tcp::socket& so,
				AfterTargetTCPReceiveResultCallback ttrb /* = nullptr */)
				: TCPSession(so, boost::function<void(const int, const int)>(), boost::function<void(const int, const void*, const int)>()), ttr_callback{ ttrb }
			{
				s_callback = boost::bind(&TCPTargetSession::afterTCPSendResultCallback, this, _1, _2);
				r_callback = boost::bind(&TCPTargetSession::afterTCPReceiveResultCallback, this, _1, _2, _3);
			}
			TCPTargetSession::~TCPTargetSession()
			{}

			void TCPTargetSession::afterTCPSendResultCallback(
				const int e /* = 0 */, 
				const int bytes /* = 0 */)
			{}

			void TCPTargetSession::afterTCPReceiveResultCallback(
				const int e /* = 0 */,
				const void* data /* = nullptr */,
				const int bytes /* = 0 */)
			{
				if (!e && data && bytes)
				{
					//检查每一帧头数据是否正确
					const int* flag{ (const int*)data };
					if (framework::data::FixedHeadFlag != *flag)
					{
						return;
					}

					const int* dataType{ (const int*)(data + 4) };
					const int* streamType{ (const int*)(data + 8) };
					const int* frameType{ (const int*)(data + 12) };
					const int* frameSize{ (const int*)(data + 16) };
					const long long* frameSequence{ (const long long*)(data + 20) };
					const long long* frameTimestamp{ (const long long*)(data + 28) };
					char url[1024]{ 0 };
					memcpy(url, data + framework::data::FixedHeadSize, *frameSize);

					if (ttr_callback)
					{
						ttr_callback(
							url, 
							*frameSize, 
							boost::enable_shared_from_this<TCPTargetSession>::shared_from_this());
					}
				}
			}
		}//namespace asio
	}//namespace network
}//namespace framework
