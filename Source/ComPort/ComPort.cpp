#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "Thread/ThreadPool.h"
using ThreadPool = base::thread::ThreadPool;
#include "ComPort/ComPort.h"  

namespace base
{
	namespace com
	{
		ComPort::ComPort(ComPortDataNotificationCallback callback /* = nullptr */) 
			: comPortDataNotificationCallback{ callback }
		{}

		ComPort::~ComPort(void)
		{
			closePort();
		}

		int ComPort::openPort(
			const unsigned char idx /* = 0 */, 
			const unsigned int baudrate /* = 4800 */)
		{
			int e{ 0xFFFF > idx ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				ComPortPtr cpp{
					boost::make_shared<boost::asio::serial_port>(ios) };
				if (cpp)
				{
					boost::system::error_code ec;
					const std::string desc{ (boost::format("COM%d") % idx).str() };
					cpp->open(desc, ec);

					if (!ec)
					{
						cpp->set_option(boost::asio::serial_port::baud_rate(baudrate), ec);
						cpp->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none), ec);
						cpp->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none), ec);
						cpp->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one), ec);
						cpp->set_option(boost::asio::serial_port::character_size(8), ec);

						e = ThreadPool::get_mutable_instance().createNewThread(
							boost::bind(&ComPort::processDataReceiveWorkerThread, this)) ? eSuccess : eBadNewThread;
						if (eSuccess == e)
						{
							cpp->async_read_some(
								boost::asio::buffer(comPortDataCache, eMaxSize),
								boost::bind(
									&ComPort::asyncReadDataNotificationCallback,
									this,
									cpp,
									boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred));
						}
					}
				}
			}

			return e;
		}

		int ComPort::closePort()
		{
			if (!ios.stopped())
			{
				ios.stop();
			}
		}

		void ComPort::asyncReadDataNotificationCallback(
			ComPortPtr cpp,
			const boost::system::error_code e, 
			const int bytes /* = 0 */)
		{
			if (!e && comPortDataNotificationCallback)
			{
				comPortDataNotificationCallback(comPortDataCache, bytes);
			}

			if (!e && cpp)
			{
				cpp->async_read_some(
					boost::asio::buffer(comPortDataCache, eMaxSize),
					boost::bind(
						&ComPort::asyncReadDataNotificationCallback,
						this,
						cpp,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
			}
		}

		void ComPort::processDataReceiveWorkerThread()
		{
			if (ios.stopped())
			{
				ios.run();
			}
		}
	}//namespace com
}//namespace base
