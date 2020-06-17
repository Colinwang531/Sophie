//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-15
//		Description :					组件协议封装/解析类
//
//		History:						Author									Date										Description
//										王科威									2020-05-15									创建
//										王科威									2020-05-29									添加数据结构和解析/封装方法
//

#ifndef COMPONENT_CODEC_H
#define COMPONENT_CODEC_H

namespace base
{
	namespace protocol
	{
		class ComponentParser
		{
		public:
			ComponentParser(void);
			~ComponentParser(void);

		public:
			//创建组件消息包
			//@Return : 消息包实例
			void* createNewComponent(void);

			//销毁组件消息包
			//@pkt : 组件消息包实例
			//@Comment : 使用createNewComponent创建的组件消息包实例必须使用该方法进行销毁
			//			 调用者也可以自行进行销毁
			void destroyComponent(void* pkt = nullptr);

			//解析组件消息
			//@c : 组件消息数据
			//@pkt : 消息包
			//@Return : 错误码
			int unpackComponent(
				void* c = nullptr, 
				void* pkt = nullptr);
		};//class ComponentParser

		class ComponentPacker
		{
		public:
			ComponentPacker(void);
			~ComponentPacker(void);

		public:
			//封装组件消息
			//@command : 命令类型
			//@result : 当COMPONENT_COMMAND_*_REP == command时该参数有效,表示应答状态
			//@data : 消息数据
			//		  当COMPONENT_COMMAND_SIGNIN_REP == command时表示组件ID标识
			//		  当COMPONENT_COMMAND_QUERY_REP == command时表示组件信息集合
			//@Return : 消息内容
			void* packComponent(
				const int command = 0,
				const int result = 0,
				void* data = nullptr);
		};//class ComponentPacker
	}//namespace protocol
}//namespace base

#endif//COMPONENT_CODEC_H
