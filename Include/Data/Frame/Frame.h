//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-17
//		Description :					帧数据类
//
//		History:						Author									Date										Description
//										王科威									2020-09-17									创建
//

#ifndef FRAMEWORK_DATA_FRAME_H
#define FRAMEWORK_DATA_FRAME_H

#include <vector>
#include "Data/Data.h"

namespace framework
{
	namespace data
	{
		typedef enum class tagFrameType_t : int
		{
			FRAME_TYPE_NONE = 0,
			FRAME_TYPE_HEADER = 1,
			FRAME_TYPE_I = 2,
			FRAME_TYPE_P = 3,
			FRAME_TYPE_B = 4,
			FRAME_TYPE_YV12 = 5,
			FRAME_TYPE_YUV420P = 6,
			FRAME_TYPE_BGR = 7,
			FRAME_TYPE_JPEG = 8,
			FRAME_TYPE_AAC = 9,
			FRAME_TYPE_G711 = 10,
			FRAME_TYPE_G722 = 11,
		}FrameType;

		typedef struct tagObjectTrackPosition_t
		{
			int x;
			int y;
			int w;
			int h;
		}OTP;

		typedef struct tagObjectTrack_t
		{
			bool enable;
			std::vector<OTP> otps;
		}OT;

		class Frame : public Data
		{
		public:
			Frame(
				const int w = 0,
				const int h = 0,
				const FrameType ftype = FrameType::FRAME_TYPE_NONE,
				const DataFactory dfactory = DataFactory::DATA_FACTORY_NONE,
				const DataType dtype = DataType::DATA_TYPE_NONE);
			virtual ~Frame(void);

		public:
			//设置/获取帧类型
			//type : 帧类型
			//@Return : 帧类型
			inline void setFrameType(const FrameType type = FrameType::FRAME_TYPE_NONE)
			{
				frameType = type;
			}
			inline const FrameType getFrameType(void) const
			{
				return frameType;
			}

			//设置/获取帧ID
			//id : 帧ID
			//@Return : 帧ID
			inline void setFrameID(const std::string id)
			{
				frameID = id;
			}
			inline const std::string getFrameID(void) const
			{
				return frameID;
			}

			//获取帧数据
			//@Return : 帧数据
			inline const void* getFrameData(void) const
			{
				return buffer;
			}

			//设置帧数据
			//@data : 帧数据
			//@bytes : 帧数据大小
			//@Return : 错误码
			int setFrameData(
				const void* data = nullptr,
				const int bytes = 0);

			//获取帧数据大小
			//@Return : 帧数据大小
			inline const unsigned long long getFrameBytes(void) const
			{
				return bufsize;
			}

			//获取帧分辨率之宽度
			//@Return : 宽度
			inline const int getFrameWidth(void) const
			{
				return width;
			}

			//获取帧分辨率之高度
			//@Return : 高度
			inline const int getFrameHeight(void) const
			{
				return height;
			}

			//添加目标位置信息
			//@otp : 目标位置信息
			//@track : 跟踪标识
			void addOT(const OTP otp, const bool track = false);

			//清理目标位置信息
			void clearOT(void);

			//获取目标位置信息
			inline const OT getOT(void) const
			{
				return ot;
			}

		public:
			std::string faceID;
		private:
			const int width;
			const int height;
 			FrameType frameType;
			void* buffer;
			unsigned long long bufsize;
			OT ot;
			std::string frameID;
		};//class Frame
	}//namespace data
}//namespace framework

#endif//FRAMEWORK_DATA_FRAME_H
