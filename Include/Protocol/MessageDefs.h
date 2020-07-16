//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-08
//		Description :					通信协议消息数据定义
//
//		History:						Author									Date										Description
//										王科威									2020-06-08									创建
//

#ifndef MESSAGE_DEFS_H
#define MESSAGE_DEFS_H

//====================================设备部分=============================================//



typedef struct tagDeviceCamera_t
{
	bool enable;
	int index;
	std::string cid;
	std::string ip;
	std::string nickname;
}DeviceCamera;

typedef struct tagDeviceEmbedded_t
{
	bool enable;
	DeviceFactory factory;
	DeviceType type;
	int port;
	std::string name;
	std::string password;
	std::string ip;
	std::string did;
	std::string nickname;
	std::vector<DeviceCamera> cameras;
}DeviceEmbedded;

typedef struct tagDeviceRequest_t
{
	std::string did;
	DeviceEmbedded embedded;
}DeviceRequest;

typedef struct tagDeviceResponse_t
{
	int result;
	std::string did;
	std::vector<DeviceEmbedded> embeddeds;
}DeviceResponse;

typedef struct tagDevicePacket_t 
{
	DeviceCommand command;								//信令类型
	void* data;											//请求或应答
}DevicePacket;

//====================================状态部分=============================================//



typedef struct tagStatusRequest_t
{
	StatusType type;
	bool flag;
	std::string name;
}StatusRequest;

typedef struct tagStatusResponse_t
{
	int result;
	bool flag;
	std::string name;
}StatusResponse;

typedef struct tagStatusPacket_t
{
	StatusCommand command;
	void* data;
}StatusPacket;

//====================================算法部分=============================================//

typedef enum class tagAlgorithmCommand_t : int
{
	ALGORITHM_COMMAND_NONE = 0,
	ALGORITHM_COMMAND_SET_REQ = 1,
	ALGORITHM_COMMAND_SET_REP = 2,
	ALGORITHM_COMMAND_QUERY_REQ = 3,
	ALGORITHM_COMMAND_QUERY_REP = 3,
}AlgorithmCommand;

typedef struct tagAlgorithmConfig_t
{
	bool sleep;
	bool fight;
	bool helmet;
	bool phone;
	bool attendancein;
	bool attendanceout;
	int gpu;
	float similar;
	std::string cid;
}AlgorithmConfig;

typedef struct tagAlgorithmRequest_t
{
	AlgorithmConfig config;
}AlgorithmRequest;

typedef struct tagAlgorithmResponse_t
{
	int result;
	AlgorithmConfig config;
}AlgorithmResponse;

typedef struct tagAlgorithmPacket_t
{
	AlgorithmCommand command;
	void* data;
}AlgorithmPacket;

//====================================成员部分=============================================//

typedef enum class tagCrewCommand_t : int
{
	CREW_COMMAND_NONE = 0,
	CREW_COMMAND_NEW_REQ = 1,
	CREW_COMMAND_NEW_REP = 2,
	CREW_COMMAND_DELETE_REQ = 3,
	CREW_COMMAND_DELETE_REP = 4,
	CREW_COMMAND_MODIFY_REQ = 5,
	CREW_COMMAND_MODIFY_REP = 6,
	CREW_COMMAND_QUERY_REQ = 7,
	CREW_COMMAND_QUERY_REP = 8
}CrewCommand;

typedef struct tagEmployee_t
{
	std::string name;
	std::string job;
	void* picture;
}Employee;

typedef struct tagCrewRequest_t
{
	Employee employee;
	std::string uid;
}CrewRequest;

typedef struct tagCrewResponse_t
{
	int result;
	std::string uid;
	std::vector<Employee> employees;
}CrewResponse;

typedef struct tagCrewPacket_t
{
	CrewCommand command;
	void* data;
}CrewPacket;

#endif//MESSAGE_DEFS_H
