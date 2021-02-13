#include <string.h>
#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "Define.h"
#include "Error.h"
#include "Data/Buffer.h"

namespace framework
{
	namespace data
	{
		Buffer::Buffer(const int bytes /* = 3 * 1024 * 1024 */) 
			: buffer{ nullptr }, bufsize{ 0 }, factory{ DataFactory::DATA_FACTORY_NONE }, type{ DataType::DATA_TYPE_NONE }
		{
			buffer = new(std::nothrow) unsigned char[bytes];
		}
		Buffer::~Buffer()
		{
			boost::checked_array_delete(buffer);
		}

		int Buffer::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				if (DataFactory::DATA_FACTORY_NONE == factory && DataType::DATA_TYPE_NONE == type)
				{
					factory = data->getDataFactory();
					type = data->getDataType();
				}

				const void* raw{ data->getData() };
				const int bytes{ data->getDataBytes() };
				int more{ 0 }, part{ 0 };

				if (0 == bufsize)
				{
					const int* flag{ (const int*)raw };
					if (FixedHeadFlag != *flag)
					{
						return eBadOperate;
					}
				}

				if (bufsize >= gMaxBufferSize)
				{
					//Amazing
					//没办法只有丢数据了
					bufsize = 0;
					return eOutOfRange;
				}

				if (bytes + bufsize > gMaxBufferSize)
				{
					//将部分数据拷贝进缓存进行处理
					//剩余部分必须在处理结束后继续拷贝进缓存
					part = gMaxBufferSize - bufsize;
					more = bytes - part;

#ifdef WINDOWS
					memcpy_s(buffer + bufsize, part, raw, part);
#else
					memcpy(buffer + bufsize, raw, part);
#endif//WINDOWS
					bufsize += part;
				}
				else
				{
#ifdef WINDOWS
					memcpy_s(buffer + bufsize, bytes, raw, bytes);
#else
					memcpy(buffer + bufsize, raw, bytes);
#endif//WINDOWS
					bufsize += bytes;
				}

				//一次解析一帧或多帧数据
				e = packOneFrameData();
				//所有帧解析正确才能拷贝剩余数据进缓存
				if (eSuccess == e && e < more && 0 < part)
				{
#ifdef WINDOWS
					memcpy_s(buffer + bufsize, more, raw + part, more);
#else
					memcpy(buffer + bufsize, raw + part, more);
#endif//WINDOWS
					bufsize += more;
				}
			}

			return e;
		}

		int Buffer::packOneFrameData()
		{
			int pos{ 0 }, e{ eSuccess };

			while (pos < bufsize)
			{
				//剩余数据小于固定头长度就移动剩余数据到缓存区域起始位置
				if (bufsize - pos <= FixedHeadSize)
				{
					bufsize -= pos;
#ifdef WINDOWS
					memcpy_s(buffer, bufsize, buffer + pos, bufsize);
#else
					memcpy(buffer, buffer + pos, bufsize);
#endif//WINDOWS
					return e;
				}

				//检查每一帧头数据是否正确
				const int* flag{ (const int*)(buffer + pos) };
				if (FixedHeadFlag != *flag)
				{
					//Ops!!!!!!!!!!!!!!!!!!!!!!!!!
					bufsize = 0;
					return eBadOperate;
				}

				const int* dataType{ (const int*)(buffer + pos + 4) };
				const int* streamType{ (const int*)(buffer + pos + 8) };
				const int* frameType{ (const int*)(buffer + pos + 12) };
				const int* frameSize{ (const int*)(buffer + pos + 16) };
				const long long* frameSequence{ (const long long*)(buffer + pos + 20) };
				const long long* frameTimestamp{ (const long long*)(buffer + pos + 28) };

				//剩余数据小于实际数据长度就移动剩余数据到缓存区域起始位置
				if (bufsize - pos - FixedHeadSize < *frameSize)
				{
					bufsize -= pos;
#ifdef WINDOWS
					memcpy_s(buffer, bufsize, buffer + pos, bufsize);
#else
					memcpy(buffer, buffer + pos, bufsize);
#endif//WINDOWS
					return e;
				}

				DataPtr dp{ boost::make_shared<Data>(factory, type) };
				if (dp)
				{
					dp->setData(buffer + pos + FixedHeadSize, *frameSize);
					afterParseOneFrameData(dp);
				}

				pos += (FixedHeadSize + *frameSize);
			}

			bufsize -= pos;
			return e;
		}
	}//namespace stream
}//namespace base
