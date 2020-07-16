//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-07
//		Description :					消息包类
//
//		History:						Author									Date										Description
//										王科威									2020-07-07									创建
//

#ifndef BASE_PACKET_MESSAGE_PACKET_H
#define BASE_PACKET_MESSAGE_PACKET_H

#include "Packet/AbstractPacket.h"

namespace base
{
	namespace packet
	{
		class MessagePacket : public AbstractPacket
		{
		public:
			MessagePacket(
				const PacketType type = PacketType::PACKET_TYPE_NONE,
				const int command = 0,
				const int result = 0);
			virtual ~MessagePacket(void);

		protected:
			//获取消息命令类型
			//@Return : 消息命令类型
			inline const int getPacketDataCommandType(void) const override
			{
				return messageCommand;
			}

			//读/写应答消息结果
			//@Return : 应答消息结果
			inline const int getPacketDataReplyResult(void) const override
			{
				return replyResult;
			}

			//读/写包数据
			//@pkt : 包数据
			//@Return : 包数据类型
			inline void setPacketData(void* pkt = nullptr) override
			{
				messageData = pkt;
			}
			inline const void* getPacketData(void) const override
			{
				return messageData;
			}

		private:
			const int messageCommand;
			const int replyResult;
			void* messageData;
		};//class AbstractPacket
	}//namespace packet
}//namespace base

#endif//BASE_PACKET_MESSAGE_PACKET_H
