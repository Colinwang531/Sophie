//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-14
//		Description :					������
//
//		History:						Author									Date										Description
//										������									2020-12-14									����
//

#ifndef FRAMEWORK_DATA_DATA_H
#define FRAMEWORK_DATA_DATA_H

#include <string>

namespace framework
{
	namespace data
	{
		typedef enum class tagDataFactory_t : int
		{
			DATA_FACTORY_NONE = 0,
			DATA_FACTORY_PRIVATE,
			DATA_FACTORY_HK,
			DATA_FACTORY_DH,
			DATA_FACTORY_AIS
		}DataFactory;

		typedef enum class tagDataType_t : int
		{
			DATA_TYPE_NONE = 0,
			DATA_TYPE_BYTES,
			DATA_TYPE_HEADER,
			DATA_TYPE_BASE64,
		}DataType;

		class Data
		{
		public:
			Data(
				const DataFactory factory = DataFactory::DATA_FACTORY_NONE,
				const DataType type = DataType::DATA_TYPE_NONE);
			virtual ~Data(void);

		public:
			//��������
			//@data : ����
			//@databytes : ���ݴ�С
			//@Return : ������
			int setData(
				const void* data = nullptr,
				const unsigned long long bytes = 0);

			//��ȡ���ݳ���
			//@Return : ���ݳ���
			inline const DataFactory getDataFactory(void) const
			{
				return dataFactory;
			}

			//��ȡ��������
			//@Return : ��������
			inline const DataType getDataType(void) const
			{
				return dataType;
			}

			//��ȡ����
			//@Return : ����
			inline const void* getData(void) const
			{
				return buffer;
			}

			//��ȡ���ݴ�С
			//@Return : ���ݴ�С
			inline const unsigned long long getDataBytes(void) const
			{
				return bufsize;
			}

		private:
			const DataFactory dataFactory;
			const DataType dataType;
			void* buffer;
			unsigned long long bufsize;
		};//class Data
	}//namespace data
}//namespace framework

#endif//FRAMEWORK_DATA_DATA_H
