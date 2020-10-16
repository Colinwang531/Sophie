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

#include "Packet/DataPacket.h"

namespace base
{
	namespace packet
	{
		typedef enum tagMessagePacketType_t : int
		{
			MESSAGE_PACKET_TYPE_NONE = 0,
			MESSAGE_PACKET_TYPE_ALARM,
			MESSAGE_PACKET_TYPE_ALGORITHM,
			MESSAGE_PACKET_TYPE_COMPONENT,
			MESSAGE_PACKET_TYPE_CREW,
			MESSAGE_PACKET_TYPE_DEVICE,
			MESSAGE_PACKET_TYPE_STATUS,
			MESSAGE_PACKET_TYPE_USER,
			MESSAGE_PACKET_TYPE_EVENT
		}MessagePacketType;

		class MessagePacket : public DataPacket
		{
		public:
			MessagePacket(
				const MessagePacketType type = MessagePacketType::MESSAGE_PACKET_TYPE_NONE);
			virtual ~MessagePacket(void);

		public:
			//获取消息类型
			//@Return : 消息类型
			inline const MessagePacketType getMessagePacketType(void) const
			{
				return messagePacketType;
			}

			//读/写消息命令
			//@command : 消息命令
			//@Return : 消息命令
			inline void setMessagePacketCommand(const int command = -1)
			{
				messagePacketCommand = command;
			}
			inline const int getMessagePacketCommand(void) const
			{
				return messagePacketCommand;
			}

			//读/写消息状态
			//@status : 消息状态
			//@Return : 消息状态
			//@Comment : 用于记录消息应答的错误码
			inline void setMessageStatus(const int status = -1)
			{
				messageStatus = status;
			}
			inline const int getMessageStatus(void) const
			{
				return messageStatus;
			}

		private:
			void cleanupPacketData(void);

		private:
 			const MessagePacketType messagePacketType;
			int messagePacketCommand;
			int messageStatus;
		};//class MessagePacket
	}//namespace packet
}//namespace base

#endif//BASE_PACKET_MESSAGE_PACKET_H
