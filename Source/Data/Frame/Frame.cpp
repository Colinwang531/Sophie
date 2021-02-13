#include <string.h>
#include "boost/checked_delete.hpp"
#include "Error.h"
#include "Data/Frame/Frame.h"

namespace framework
{
	namespace data
	{
		Frame::Frame(
			const int w /* = 0 */,
			const int h /* = 0 */,
			const FrameType ftype /* = FrameType::FRAME_TYPE_NONE */,
			const DataFactory dfactory /* = DataFactory::DATA_FACTORY_NONE */,
			const DataType dtype /* = DataType::DATA_TYPE_NONE */)
			: Data(dfactory, dtype), frameType{ ftype }, width{ w }, height{ h }, buffer{ nullptr }, bufsize{ 0 }, ot{ true }
		{}
		Frame::~Frame()
		{
			boost::checked_array_delete(buffer);
		}

		int Frame::setFrameData(
			const void* data /* = nullptr */, 
			const int bytes /* = 0 */)
		{
			int e{ data && 0 < bytes ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				boost::checked_array_delete(buffer);
				bufsize = bytes;
				buffer = new(std::nothrow) char[bufsize] {0};

				if (buffer)
				{
#ifdef WINDOWS
					memcpy_s(buffer, bufsize, (char*)data, bufsize);
#else
					memcpy(buffer, (char*)data, bufsize);
#endif//WINDOWS
				}
				else
				{
					e = eBadNewMemory;
				}
			}

			return e;
		}

		void Frame::addOT(
			const OTP otp, 
			const bool track /* = false */)
		{
			ot.enable = track;
			ot.otps.push_back(otp);
		}

		void Frame::clearOT()
		{
			ot.enable = false;
			ot.otps.clear();
		}
	}//namespace packet
}//namespace base
