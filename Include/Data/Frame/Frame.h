//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-09-17
//		Description :					֡������
//
//		History:						Author									Date										Description
//										������									2020-09-17									����
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
			//����/��ȡ֡����
			//type : ֡����
			//@Return : ֡����
			inline void setFrameType(const FrameType type = FrameType::FRAME_TYPE_NONE)
			{
				frameType = type;
			}
			inline const FrameType getFrameType(void) const
			{
				return frameType;
			}

			//����/��ȡ֡ID
			//id : ֡ID
			//@Return : ֡ID
			inline void setFrameID(const std::string id)
			{
				frameID = id;
			}
			inline const std::string getFrameID(void) const
			{
				return frameID;
			}

			//��ȡ֡����
			//@Return : ֡����
			inline const void* getFrameData(void) const
			{
				return buffer;
			}

			//����֡����
			//@data : ֡����
			//@bytes : ֡���ݴ�С
			//@Return : ������
			int setFrameData(
				const void* data = nullptr,
				const int bytes = 0);

			//��ȡ֡���ݴ�С
			//@Return : ֡���ݴ�С
			inline const unsigned long long getFrameBytes(void) const
			{
				return bufsize;
			}

			//��ȡ֡�ֱ���֮���
			//@Return : ���
			inline const int getFrameWidth(void) const
			{
				return width;
			}

			//��ȡ֡�ֱ���֮�߶�
			//@Return : �߶�
			inline const int getFrameHeight(void) const
			{
				return height;
			}

			//���Ŀ��λ����Ϣ
			//@otp : Ŀ��λ����Ϣ
			//@track : ���ٱ�ʶ
			void addOT(const OTP otp, const bool track = false);

			//����Ŀ��λ����Ϣ
			void clearOT(void);

			//��ȡĿ��λ����Ϣ
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
