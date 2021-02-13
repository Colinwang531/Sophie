//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-23
//		Description :					过滤器抽象类
//
//		History:						Author									Date										Description
//										王科威									2020-09-23									创建
//

#ifndef FRAMEWORK_MULTIMEDIA_FILTER_H
#define FRAMEWORK_MULTIMEDIA_FILTER_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "DataStruct/UnorderedMap.h"
#include "Data/Data.h"
using Data = framework::data::Data;
using DataPtr = boost::shared_ptr<Data>;

namespace framework
{
	namespace multimedia
	{
		BOOST_STATIC_CONSTANT(std::string, HKNetSDKFilterName = "HKNetSDKFilter");
// 		BOOST_STATIC_CONSTANT(std::string, TCPDataReceiverFilterName = "TCPDataReceiverFilter");
// 		BOOST_STATIC_CONSTANT(std::string, DataBufferFilterName = "DataBufferFilter");
		BOOST_STATIC_CONSTANT(std::string, StreamDecoderFilterName = "StreamDecoderFilter");
		BOOST_STATIC_CONSTANT(std::string, Yv12ConvertYuv420pFilterName = "Yv12ConvertYuv420pFilter");
		BOOST_STATIC_CONSTANT(std::string, Yuv420pConvertBRG24FilterName = "Yuv420pConvertBRG24Filter");
		BOOST_STATIC_CONSTANT(std::string, CVCompuptingFilterName = " CVCompuptingFilter");
		BOOST_STATIC_CONSTANT(std::string, JPEGEncoderFilterName = "JPEGEncoderFilter");

		typedef enum class tagFilterType_t : int
		{
			FILTER_TYPE_MEDIUM = 0,
			FILTER_TYPE_SOURCE, 
			FILTER_TYPE_TARGET
		}FilterType;

		class Pin;
		using PinPtr = boost::shared_ptr<Pin>;
		using PinPtrGroup = UnorderedMap<const std::string, PinPtr>;
		typedef boost::function<void(DataPtr)> DataNotificationCallback;

		class Filter
		{
		public:
			Filter(
				const FilterType type = FilterType::FILTER_TYPE_MEDIUM);
			virtual ~Filter(void);

		public:
			//创建过滤器
			//@param : 用户参数
			//@Return : 错误码
			virtual int addPin(const std::string name);

			//销毁过滤器
			//@Return : 错误码
			virtual int removePin(const std::string name);

			//输入数据
			//@data : 数据
			//@Return : 错误码
			virtual int inputData(DataPtr data);

			//设置数据通知回调
			//@param : 用户参数
			inline void setDataNotificationCallback(DataNotificationCallback callback = nullptr)
			{
				dataNotificationCallback = callback;
			}

			//获取过滤器类型
			//@Return : 过滤器类型
			inline const FilterType getFilterType(void) const
			{
				return filterType;
			}

			//查询针脚
			//@name : 名称
			//@Return : 针脚实例
			PinPtr queryPin(const std::string name);

		protected:
			const FilterType filterType;
			DataNotificationCallback dataNotificationCallback;
			PinPtrGroup pins;
		};//class Filter
	}//namespace multimedia
}//namespace framework

#endif//FRAMEWORK_MULTIMEDIA_FILTER_H
