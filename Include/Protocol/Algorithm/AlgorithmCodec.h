//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					�㷨Э���װ/������
//
//		History:						Author									Date										Description
//										������									2020-05-29									����
//

#ifndef ALGORITHM_CODEC_H
#define ALGORITHM_CODEC_H

#include <vector>

namespace base
{
	namespace protocol
	{
		typedef struct tagAlgorithmConfigure_t
		{
			bool enablesleep;
			bool enablefight;
			bool enablehelmet;
			bool enablephone;
			bool enableattendancein;
			bool enableattendanceout;
			int gpu;
			float similar;
			std::string cid;
		}AlgorithmConfigure;//struct tagAlgorithmConfigure_t

		typedef struct tagAlgorithmRequest_t
		{
			std::vector<AlgorithmConfigure> configures;
		}AlgorithmRequest;//struct tagAlgorithmRequest_t

		typedef struct tagAlgorithmResponse_t
		{
			int result;
			std::vector<AlgorithmConfigure> configures;
		}AlgorithmResponse;//struct tagAlgorithmResponse_t

		class AlgorithmCodec
		{
		public:
			AlgorithmCodec(void);
			virtual ~AlgorithmCodec(void);

		public:
			int unpackAlgorithm(void* msg = nullptr);

		private:
			int command;
			AlgorithmRequest req;
			AlgorithmResponse rep;
		};//class AlgorithmCodec
	}//namespace protocol
}//namespace base

#endif//ALGORITHM_CODEC_H
