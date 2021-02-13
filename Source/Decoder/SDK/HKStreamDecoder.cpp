#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "boost/pointer_cast.hpp"
#include "boost/thread.hpp"
#include "Error.h"
#include "Thread/ThreadPool.h"
using ThreadPool = framework::thread::ThreadPool;
#include "Data/Frame/Frame.h"
using Frame = framework::data::Frame;
using FramePtr = boost::shared_ptr<Frame>;
#include "Decoder/SDK/HKStreamDecoder.h"

namespace framework
{
	namespace multimedia
	{
		HKStreamDecoder::HKStreamDecoder(
			DataNotificationCallback callback /* = nullptr */)
			: decoderID{ -1 }, dataNotificationCallback{ callback }, stopped{ false }
		{
			ThreadPool::get_mutable_instance().createNewThread(
				boost::bind(&HKStreamDecoder::decodeWorkerThread, this));
		}
		HKStreamDecoder::~HKStreamDecoder(void)
		{
			stopped = true;
			if (-1 < decoderID)
			{
				PlayM4_Stop(decoderID);
				PlayM4_CloseStream(decoderID);
				PlayM4_FreePort(decoderID);
			}
		}

		int HKStreamDecoder::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				if (framework::data::DataFactory::DATA_FACTORY_HK == data->getDataFactory())
				{
					fifo.pushBack(data);
				} 
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}

		void HKStreamDecoder::decodeWorkerThread()
		{
			while (!stopped)
			{
				DataPtr data{ fifo.front() };

				if (data)
				{
					const framework::data::DataType type{ data->getDataType() };

					//头信息且未初始化时执行
					if (framework::data::DataType::DATA_TYPE_HEADER == type && 0 > decoderID)
					{
						if (PlayM4_GetPort(&decoderID))
						{
							PlayM4_SetStreamOpenMode(decoderID, STREAME_REALTIME);
							PlayM4_SetDecCallBackExMend(decoderID, &HKStreamDecoder::afterDecodeOneFrameCallback, NULL, 0, this);
							PlayM4_OpenStream(decoderID, (unsigned char*)data->getData(), data->getDataBytes(), 1048576);
							//HARD_DECODE_ENGINE / SOFT_DECODE_ENGINE
//							PlayM4_SetDecodeEngineEx(decoderID, SOFT_DECODE_ENGINE);
							PlayM4_Play(decoderID, NULL);
						}
					}
					else if (framework::data::DataType::DATA_TYPE_BYTES == type && -1 < decoderID)
					{
						PlayM4_InputData(decoderID, (unsigned char*)data->getData(), data->getDataBytes());
					}

					fifo.popFront();
				}
				else
				{
					//如果没有数据还是休息一下
					boost::this_thread::sleep(boost::posix_time::milliseconds(1));
				}
			}
		}

		void HKStreamDecoder::afterDecodeOneFrameCallback(
			int nPort, char* pBuf, int nSize, FRAME_INFO* pFrameInfo, void* nUser, int nReserved2)
		{
			HKStreamDecoder* decoder{
				reinterpret_cast<HKStreamDecoder*>(nUser) };

			if (T_YV12 == pFrameInfo->nType)
			{
				if (decoder->dataNotificationCallback)
				{
					DataPtr dp{
						boost::make_shared<Frame>(
							pFrameInfo->nWidth,
							pFrameInfo->nHeight,
							framework::data::FrameType::FRAME_TYPE_YV12, 
							framework::data::DataFactory::DATA_FACTORY_HK,
							framework::data::DataType::DATA_TYPE_BYTES) };

					if (dp)
					{
						FramePtr fp{ 
							boost::dynamic_pointer_cast<Frame>(dp) };
						fp->setFrameData(pBuf, nSize);
						decoder->dataNotificationCallback(dp);
					}
				}
			}
			else if (T_AUDIO16 == pFrameInfo->nType)
			{
			}
		}
	}//namespace multimedia
}//namespace framework
