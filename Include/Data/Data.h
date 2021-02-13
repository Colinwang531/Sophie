//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-12-14
//		Description :					数据类
//
//		History:						Author									Date										Description
//										王科威									2020-12-14									创建
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
			//设置数据
			//@data : 数据
			//@databytes : 数据大小
			//@Return : 错误码
			int setData(
				const void* data = nullptr,
				const unsigned long long bytes = 0);

			//获取数据厂商
			//@Return : 数据厂商
			inline const DataFactory getDataFactory(void) const
			{
				return dataFactory;
			}

			//获取数据类型
			//@Return : 数据类型
			inline const DataType getDataType(void) const
			{
				return dataType;
			}

			//获取数据
			//@Return : 数据
			inline const void* getData(void) const
			{
				return buffer;
			}

			//获取数据大小
			//@Return : 数据大小
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
