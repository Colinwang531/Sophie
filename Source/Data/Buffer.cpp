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
					//û�취ֻ�ж�������
					bufsize = 0;
					return eOutOfRange;
				}

				if (bytes + bufsize > gMaxBufferSize)
				{
					//���������ݿ�����������д���
					//ʣ�ಿ�ֱ����ڴ���������������������
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

				//һ�ν���һ֡���֡����
				e = packOneFrameData();
				//����֡������ȷ���ܿ���ʣ�����ݽ�����
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
				//ʣ������С�ڹ̶�ͷ���Ⱦ��ƶ�ʣ�����ݵ�����������ʼλ��
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

				//���ÿһ֡ͷ�����Ƿ���ȷ
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

				//ʣ������С��ʵ�����ݳ��Ⱦ��ƶ�ʣ�����ݵ�����������ʼλ��
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
