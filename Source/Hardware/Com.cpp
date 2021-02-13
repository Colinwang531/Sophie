#include <fcntl.h>
#include <termios.h>
#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "Error.h"
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Hardware/Com.h"  

namespace framework
{
	namespace hardware
	{
		Com::Com()
		{}
		Com::~Com(void)
		{
			closeCom();
		}

		int Com::openCom(
			const int number /* = 0 */, 
			const int baudrate /* = 4800 */)
		{
			int e{ 
				-1 < number && 0 < baudrate ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				e = eBadOperate;
				const std::string name{ (boost::format("/dev/ttyUSB%d") % number).str() };
				fd = open(name.c_str(), O_RDWR | O_NOCTTY | O_SYNC);

				if (-1 < fd)
				{
					struct termios tty;

					if (-1 < tcgetattr(fd, &tty))
					{
						cfsetospeed(&tty, (speed_t)baudrate);
						cfsetispeed(&tty, (speed_t)baudrate);

						tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
						tty.c_cflag &= ~CSIZE;
						tty.c_cflag |= CS8;         /* 8-bit characters */
						tty.c_cflag &= ~PARENB;     /* no parity bit */
						tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
						tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

						/* setup for non-canonical mode */
						tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
						tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
						tty.c_oflag &= ~OPOST;

						/* fetch bytes as they become available */
						tty.c_cc[VMIN] = 1;
						tty.c_cc[VTIME] = 1;

						if (!tcsetattr(fd, TCSANOW, &tty)) 
						{
							ThreadPool::get_mutable_instance().createNewThread(
								boost::bind(&Com::processDataReceiveWorkerThread, this));
							e = eSuccess;
						}
					}
				}
			}

			return e;
		}

		int Com::closeCom()
		{
			int e{ fd ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				close(fd);
			}

			return e;
		}

		void Com::processDataReceiveWorkerThread()
		{
			while (1)
			{
				char buf{ 0 };
				int rdlen{ 0 };

				rdlen = read(fd, &buf, 1);
				if (rdlen > 0) 
				{
					afterReceiveComDataNotification(&buf, rdlen);
				}
				else
				{
					boost::this_thread::sleep(boost::posix_time::milliseconds(1));
				}
			}
		}
	}//namespace hardware
}//namespace framework
