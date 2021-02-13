#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "Error.h"
#include "Data/Buffer.h"
#include "Time/XTime.h"
using Time = framework::time::Time;
#include "Camera/Hikvision/HikvisionCamera.h"

namespace framework
{
	namespace device
	{
		HikvisionCamera::HikvisionCamera(const int idx /* = -1 */)
			: Camera(idx)
		{}
		HikvisionCamera::~HikvisionCamera()
		{}

		int HikvisionCamera::openStream(TCPSessionPtr sp, const int userID /* = -1 */)
		{
			int e{ -1 < userID ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				NET_DVR_PREVIEWINFO previewInfo{ 0 };
				previewInfo.lChannel = Camera::getIndex();
				streamID = NET_DVR_RealPlay_V40(
					userID, &previewInfo, &HikvisionCamera::livestreamDataCaptureCallback, this);
				tsp.swap(sp);
			}

			return e;
		}

		int HikvisionCamera::closeStream()
		{
			int e{ -1 < streamID ? eSuccess : eBadOperate };

			if (eSuccess == e)
			{
				NET_DVR_StopRealPlay(streamID);
			}

			return e;
		}

		int HikvisionCamera::sendData(
			BYTE* pBuffer /* = nullptr */, const DWORD dwBufSize /* = 0 */, const bool head /* = false */)
		{
			int e{ pBuffer && 0 < dwBufSize ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				const int bytes{ framework::data::FixedHeadSize + dwBufSize };
				char* buffer{ new(std::nothrow) char[bytes] };

				if (buffer)
				{
					int* flag{ (int*)buffer };
					*flag = framework::data::FixedHeadFlag;
					int* factoryType{ (int*)(buffer + 4) };
					*factoryType = static_cast<int>(framework::data::DataFactory::DATA_FACTORY_HK);
					int* dataType{ (int*)(buffer + 8) };
					*dataType = static_cast<int>(head ? framework::data::DataType::DATA_TYPE_HEADER : framework::data::DataType::DATA_TYPE_BYTES);
					int* frameType{ (int*)(buffer + 12) };
					*frameType = -1;
					int* frameSize{ (int*)(buffer + 16) };
					*frameSize = (int)dwBufSize;
					long long* frameSequence{ (long long*)(buffer + 20) };
					*frameSequence = -1;
					long long* frameTimestamp{ (long long*)(buffer + 28) };
					*frameTimestamp = (long long)(Time().tickcount());
#ifdef WINDOWS
					memcpy_s(buffer + framework::data::FixedHeadSize, dwBufSize, pBuffer, dwBufSize);
#else
					memcpy(buffer + framework::data::FixedHeadSize, pBuffer, dwBufSize);
#endif//WINDOWS

					if (tsp)
					{
						tsp->send(buffer, bytes);
					}
				}

				boost::checked_array_delete(buffer);
			}
			
			return e;
		}

		void HikvisionCamera::livestreamDataCaptureCallback(
			LONG lPlayHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser)
		{
			HikvisionCamera* camera{ reinterpret_cast<HikvisionCamera*>(pUser) };

			if (lPlayHandle == camera->streamID)
			{
				if (NET_DVR_SYSHEAD == dwDataType || NET_DVR_STREAMDATA == dwDataType)
				{
					camera->sendData(pBuffer, dwBufSize, NET_DVR_SYSHEAD == dwDataType ? true : false);
				}
// 				else if (NET_DVR_AUDIOSTREAMDATA == dataBytes)
// 				{
// 					livestream->captureAudioStreamProcess(streamData, dataBytes);
// 				}
			}
		}
	}//namespace device
}//namespace framework
