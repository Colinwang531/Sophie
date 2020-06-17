//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-29
//		Description :					算法协议封装/解析类
//
//		History:						Author									Date										Description
//										王科威									2020-05-29									创建
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
