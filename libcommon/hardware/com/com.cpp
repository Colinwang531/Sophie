#include <fcntl.h>
#include <termios.h>
#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "boost/functional/factory.hpp"
#include "boost/thread.hpp"
#include "libcommon/error.h"
#include "libcommon/thread/threadpool.h"
using ThreadPool = framework::libcommon::ThreadPool;
#include "com.h"  

namespace framework
{
	namespace libcommon
	{
		namespace hardware
		{
			class ICom
			{
			public:
				ICom(void);
				~ICom(void);

			public:
				CommonError start(
					const int number = 0, 
					const int baudrate = 4800,
					ComReadDataHandler handler = nullptr);
				CommonError stop(void);

			private:
				void dataReadThreadHandler(void);

			private:
				int fd;
				void* tid;
				bool stopped;
				ComReadDataHandler comReadDataHandler;
			};//class ICom

			ICom::ICom() 
				: fd{-1}, tid{nullptr}, stopped{false}
			{}
			ICom::~ICom(void)
			{
				stop();
			}

			CommonError ICom::start(
				const int number /* = 0 */, 
				const int baudrate /* = 4800 */,
				ComReadDataHandler handler /*= nullptr*/)
			{
				CommonError e{CommonError::COMMON_ERROR_SUCCESS};
				const std::string fdName{
					(boost::format("/dev/ttyUSB%d") % number).str()};
				fd = open(fdName.c_str(), O_RDWR | O_NOCTTY | O_SYNC);

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
							comReadDataHandler = handler;
							tid = ThreadPool().create(
								boost::bind(&ICom::dataReadThreadHandler, this));
						}
						else
						{
							e = CommonError::COMMON_ERROR_SET_FAILED;
						}
					}
					else
					{
						e = CommonError::COMMON_ERROR_SET_FAILED;
					}
				}
				else
				{
					e = CommonError::COMMON_ERROR_BAD_FILE_OPEN;
				}

				return e;
			}

			CommonError ICom::stop()
			{
				stopped = true;
				ThreadPool().destroy(tid);
				tid = nullptr;
				return CommonError::COMMON_ERROR_SUCCESS;
			}

			void ICom::dataReadThreadHandler()
			{
				while (!stopped)
				{
					char buf{ 0 };
					int rdlen{static_cast<int>(read(fd, &buf, 1))};
					
					if (0 < rdlen && comReadDataHandler) 
					{
						comReadDataHandler(&buf, rdlen);
					}
					else
					{
						boost::this_thread::sleep(boost::posix_time::milliseconds(1));
					}
				}
			}

			Com::Com() : com{boost::factory<ICom*>()()}
			{}
			Com::~Com()
			{
				boost::checked_delete(com);
			}

			int Com::start(
				const int number /* = 0 */, 
				const int baudrate /* = 4800 */,
				ComReadDataHandler handler /*= nullptr*/)
			{
				CommonError e{ 
					-1 < number && 0 < baudrate ? 
					CommonError::COMMON_ERROR_SUCCESS : 
					CommonError::COMMON_ERROR_INVALID_PARAMETER };
				
				if (CommonError::COMMON_ERROR_SUCCESS == e)
				{
					e = (com ? com->start(number, baudrate, handler) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
				}
				
				return static_cast<int>(e);
			}

			int Com::stop()
			{
				return static_cast<int>(com ? com->stop() : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
			}
		}//namespace hardware
	}//namespace libcommon
}//namespace framework
