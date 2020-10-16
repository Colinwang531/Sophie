#include <string.h>
#include <string>
#include <sstream>
#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/transform_width.hpp"
#include "Error.h"
#include "Device/SurveillanceDevice.h"

namespace base
{
	namespace device
	{
		int SurveillanceDevice::gDeviceCounter = 0;

		SurveillanceDevice::SurveillanceDevice(
			const std::string did,
			const SurveillanceDeviceFactory factory /* = SurveillanceDeviceFactory::SURVEILLANCE_DEVICE_FACTORY_NONE */,
			const SurveillanceDeviceType type /* = SurveillanceDeviceType::SURVEILLANCE_DEVICE_TYPE_NONE */)
			: AbstractDevice(did), deviceFactory{ factory }, deviceType{ type }
		{}
		SurveillanceDevice::~SurveillanceDevice(){}

		int SurveillanceDevice::getDeviceCamera(std::vector<AbstractCamera>& cameras)
		{
			return eNotSupport;
		}

		int SurveillanceDevice::captureRealplayJPEGImage(
			const int cameraIdx /* = -1 */,
			char* data /* = nullptr */,
			const int bytes /* = 0 */)
		{
			//将抓取的原始图片编码为Base64数据
			typedef boost::archive::iterators::base64_from_binary<
				boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8>> Base64EncodeIterator;
			std::string inputData((const char*)data, bytes);
			std::stringstream result;
			copy(Base64EncodeIterator(inputData.begin()), Base64EncodeIterator(inputData.end()), std::ostream_iterator<char>(result));
			size_t equal_count = (3 - inputData.length() % 3) % 3;
			for (size_t i = 0; i < equal_count; i++)
			{
				result.put('=');
			}
			//去掉结尾数据乱码
			result.put('\0');
			//Base64数据拷贝回缓存
			const int resultBytes{ (const int)result.str().length() };
#ifdef WINDOWS
			memcpy_s(data, resultBytes, result.str().c_str(), resultBytes);
#else
			memcpy(data, result.str().c_str(), resultBytes);
#endif//WINDOWS

			return resultBytes;
		}

		int SurveillanceDevice::loginDevice()
		{
			return ++gDeviceCounter;
		}

		int SurveillanceDevice::logoutDevice()
		{
			return 0 < gDeviceCounter ? --gDeviceCounter : 0;
		}
	}//namespace device
}//namespace base
