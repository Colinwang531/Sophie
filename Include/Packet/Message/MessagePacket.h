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
			//��ȡ��Ϣ��������
			//@Return : ��Ϣ��������
			inline const int getPacketDataCommandType(void) const override
			{
				return messageCommand;
			}

			//��/дӦ����Ϣ���
			//@Return : Ӧ����Ϣ���
			inline const int getPacketDataReplyResult(void) const override
			{
				return replyResult;
			}

			//��/д������
			//@pkt : ������
			//@Return : ����������
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
