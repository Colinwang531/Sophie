//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-07
//		Description :					包抽象基类
//
//		History:						Author									Date										Description
//										王科威									2020-07-07									创建
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
			//读/写包序列号
			//@seq : 包序列号
			//@Return : 包序列号
			inline void setPacketSequence(const long long seq = 0)
			{
				packetSequence = seq;
			}
			inline const long long getPacketSequence(void) const
			{
				return packetSequence;
			}

			//获取包时间戳
			//@Return : 包时间戳
			inline const long long getPacketTimestamp(void) const
			{
				return packetTimestamp;
			}

			//获取包数据类型
			//@Return : 包数据类型
			inline const PacketType getPacketType(void) const
			{
				return packetType;
			}

			//获取包命令类型
			//@Return : 包命令类型
			virtual const int getPacketDataCommandType(void) const = 0;

			//获取应答消息结果
			//@Return : 应答消息结果
			virtual const int getPacketDataReplyResult(void) const = 0;

			//读/写包数据
			//@pkt : 包数据
			//@Return : 包数据类型
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
