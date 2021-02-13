//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-01-22
//		Description :					算法数据类
//
//		History:						Author									Date										Description
//										王科威									2021-01-22									创建
//

#ifndef FRAMEWORK_DATA_PROTOCOL_ALGORITHM_H
#define FRAMEWORK_DATA_PROTOCOL_ALGORITHM_H

#include <vector>
#include "Data/Data.h"

namespace framework
{
	namespace data
	{
		typedef struct tagAlgorithmData_t
		{
			bool inoutforface;
			int triggertimeforsleep;
			int type;
			int gpu;
			float detect;
			float track;
			float detect2forphone;
			float similarforface;
			std::string id;
			std::string camera;
		}AlgorithmData;

		typedef struct tagAlgorithmMsg_t
		{
			int command;
			int status;
			std::vector<AlgorithmData> algos;
			std::string pid;
		}AlgorithmMsg;

		class ProtocolAlgorithm final : public Data
		{
		public:
			ProtocolAlgorithm(void);
			~ProtocolAlgorithm(void);

		public:
			int unpack(const std::string msg, AlgorithmMsg& am);
			const std::string pack(const AlgorithmMsg& am);
		};//class ProtocolAlgorithm
	}//namespace data
}//namespace framework

#endif//FRAMEWORK_DATA_PROTOCOL_ALGORITHM_H
