#include "Error.h"
#include "Hardware/Cpu.h"
using Cpu = framework::hardware::Cpu;
#include "Network/ASIO/Core/Asio.h"

namespace framework
{
	namespace network
	{
		namespace asio
		{
			ASIO::ASIO() : ioctxGroup{ static_cast<std::size_t>(Cpu().getCPUCoreNumber()) }, idleNumber{ 0 }
			{}

			ASIO::~ASIO()
			{}

			int ASIO::startASIO()
			{
				for (std::size_t i = 0; i != ioctxGroup.size(); ++i)
				{
					boost::thread* thread_{
						iothreadGroup.create_thread(
							boost::bind(&boost::asio::io_context::run, boost::ref(ioctxGroup[i]))) };
					Cpu().setThreadAffinityMask(thread_->native_handle(), static_cast<int>(i));
					ioworkGroup.push_back(boost::asio::io_context::work(ioctxGroup[i]));
				}

				return eSuccess;
			}

			int ASIO::stopASIO()
			{
				std::for_each(ioctxGroup.begin(), ioctxGroup.end(), boost::bind(&boost::asio::io_context::stop, _1));
				iothreadGroup.join_all();
				ioworkGroup.clear();
				ioctxGroup.clear();
			}

			boost::asio::ip::tcp::socket ASIO::getTCPSocket()
			{
				boost::asio::ip::tcp::socket so(getIdleIO());
				return std::move(so);
			}

			boost::asio::ip::udp::socket ASIO::getUDPSocket(const unsigned short port /* = 0 */)
			{
				//¼àÌý
				boost::asio::ip::udp::socket s{
					getIdleIO(),
					boost::asio::ip::udp::endpoint{ 
						boost::asio::ip::udp::v4(), port } };
				return std::move(s);
			}

			boost::asio::io_context& ASIO::getIdleIO()
			{
				WriteLock lock(mtx);
				return ioctxGroup.at(idleNumber++ % ioctxGroup.size());
			}
		}//namespace asio
	}//namespace network
}//namespace base
