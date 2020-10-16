//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-07
//		Description :					��Ϣ����
//
//		History:						Author									Date										Description
//										������									2020-07-07									����
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
			//��ȡ��Ϣ����
			//@Return : ��Ϣ����
			inline const MessagePacketType getMessagePacketType(void) const
			{
				return messagePacketType;
			}

			//��/д��Ϣ����
			//@command : ��Ϣ����
			//@Return : ��Ϣ����
			inline void setMessagePacketCommand(const int command = -1)
			{
				messagePacketCommand = command;
			}
			inline const int getMessagePacketCommand(void) const
			{
				return messagePacketCommand;
			}

			//��/д��Ϣ״̬
			//@status : ��Ϣ״̬
			//@Return : ��Ϣ״̬
			//@Comment : ���ڼ�¼��ϢӦ��Ĵ�����
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
