//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-07
//		Description :					���������
//
//		History:						Author									Date										Description
//										������									2020-07-07									����
//

#ifndef BASE_PACKET_ABSTRACT_PACKET_H
#define BASE_PACKET_ABSTRACT_PACKET_H

namespace base
{
	namespace packet
	{
		typedef enum class tagPacketType_t : int
		{
			PACKET_TYPE_NONE = 0,
			PACKET_TYPE_ALARM = 1,
			PACKET_TYPE_ALGORITHM = 2,
			PACKET_TYPE_COMPONENT = 3,
			PACKET_TYPE_CREW = 4,
			PACKET_TYPE_DEVICE = 5,
			PACKET_TYPE_STATUS = 6,
			PACKET_TYPE_USER
		}PacketType;

		class AbstractPacket
		{
		public:
			AbstractPacket(
				const PacketType type = PacketType::PACKET_TYPE_NONE);
			virtual ~AbstractPacket(void);

		public:
			//��/д�����к�
			//@seq : �����к�
			//@Return : �����к�
			inline void setPacketSequence(const long long seq = 0)
			{
				packetSequence = seq;
			}
			inline const long long getPacketSequence(void) const
			{
				return packetSequence;
			}

			//��ȡ��ʱ���
			//@Return : ��ʱ���
			inline const long long getPacketTimestamp(void) const
			{
				return packetTimestamp;
			}

			//��ȡ����������
			//@Return : ����������
			inline const PacketType getPacketType(void) const
			{
				return packetType;
			}

			//��ȡ����������
			//@Return : ����������
			virtual const int getPacketDataCommandType(void) const = 0;

			//��ȡӦ����Ϣ���
			//@Return : Ӧ����Ϣ���
			virtual const int getPacketDataReplyResult(void) const = 0;

			//��/д������
			//@pkt : ������
			//@Return : ����������
			virtual void setPacketData(void* pkt = nullptr) = 0;
			virtual const void* getPacketData(void) const = 0;

		private:
			const PacketType packetType;
			long long packetSequence;
			const long long packetTimestamp;
		};//class AbstractPacket
	}//namespace packet
}//namespace base

#endif//BASE_PACKET_ABSTRACT_PACKET_H
