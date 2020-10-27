//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-07
//		Description :					数据包抽象基类
//
//		History:						Author									Date										Description
//										王科威									2020-07-07									创建
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
			//@Comment : 包时间戳在包实例创建时自动生成且不能修改
			inline void setPacketTimestamp(const long long ts = 0)
			{
				packetTimestamp = ts;
			}
			inline const long long getPacketTimestamp(void) const
			{
				return packetTimestamp;
			}

			//获取包数据类型
			//@Return : 包数据类型
			inline const DataPacketType getPacketType(void) const
			{
				return packetType;
			}

			//读/写包数据
			//@data : 包数据
			//@idex : 索引号
			//@Return : 包数据
			//@Comment : 不分配内存空间,不保证数据有效性
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
