//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					针脚抽象类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_PIN_H
#define FRAMEWORK_MULTIMEDIA_PIN_H

#include "boost/shared_ptr.hpp"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace multimedia
	{
		BOOST_STATIC_CONSTANT(std::string, InputPinName = "FrameworkInputPin");
		BOOST_STATIC_CONSTANT(std::string, OutputPinName = "FrameworkOutputPin");

		typedef enum class tagPinType_t : int
		{
			PIN_TYPE_INPUT = 0,
			PIN_TYPE_OUTPUT
		}PinType;

		class Filter;
		class Pin;
		using PinPtr = boost::shared_ptr<Pin>;

		class Pin
		{
		public:
			Pin(
				Filter& filter,
				const PinType type = PinType::PIN_TYPE_INPUT);
			virtual ~Pin(void);

		public:
			//连接针脚
			//@input : 针脚
			//@Return : 错误码
			//@Comment : 输出针脚连接输入针脚
			virtual int joinPin(PinPtr input);

			//输入数据
			//@data : 数据
			//@Return : 错误码
			virtual int inputData(DataPtr data);

			//获取针脚类型
			//@Return : 针脚类型
			inline const PinType getPinType(void) const
			{
				return pinType;
			}

		private:
			const PinType pinType;
			Filter& parentFilter;
			PinPtr inputPin;
		};//class Pin
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_PIN_H
