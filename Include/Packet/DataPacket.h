//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-07
//		Description :					���ݰ��������
//
//		History:						Author									Date										Description
//										������									2020-07-07									����
//

#ifndef BASE_PACKET_DATA_PACKET_H
#define BASE_PACKET_DATA_PACKET_H

#include <vector>

namespace base
{
	namespace packet
	{
		typedef enum tagDataPacketType_t : int
		{
			DATA_PACKET_TYPE_NONE = 0,
			DATA_PACKET_TYPE_MESSAGE,
			DATA_PACKET_TYPE_VIDEO,
			DATA_PACKET_TYPE_AUDIO,
			DATA_PACKET_TYPE_IMAGE
		}DataPacketType;

		class DataPacket
		{
		public:
			DataPacket(
				const DataPacketType type = DataPacketType::DATA_PACKET_TYPE_NONE);
			virtual ~DataPacket(void);

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
			//@Comment : ��ʱ����ڰ�ʵ������ʱ�Զ������Ҳ����޸�
			inline void setPacketTimestamp(const long long ts = 0)
			{
				packetTimestamp = ts;
			}
			inline const long long getPacketTimestamp(void) const
			{
				return packetTimestamp;
			}

			//��ȡ����������
			//@Return : ����������
			inline const DataPacketType getPacketType(void) const
			{
				return packetType;
			}

			//��/д������
			//@data : ������
			//@idex : ������
			//@Return : ������
			//@Comment : �������ڴ�ռ�,����֤������Ч��
			void setPacketData(void* data = nullptr);
			void setPacketData(const int idx = 0, void* data = nullptr);
			void* getPacketData(const int idx = 0);

		private:
			const DataPacketType packetType;
			long long packetSequence;
			long long packetTimestamp;

		protected:
			std::vector<void*> packetData;
		};//class DataPacket
	}//namespace packet
}//namespace base

#endif//BASE_PACKET_DATA_PACKET_H
