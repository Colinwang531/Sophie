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

#include <vector>

//====================================组件部分=============================================//

typedef enum class tagComponentCommand_t : int
{
	COMPONENT_COMMAND_NONE = 0,
	COMPONENT_COMMAND_SIGNIN_REQ = 1,
	COMPONENT_COMMAND_SIGNIN_REP = 2,
	COMPONENT_COMMAND_SIGNOUT_REQ = 3,
	COMPONENT_COMMAND_SIGNOUT_REP = 4,
	COMPONENT_COMMAND_QUERY_REQ = 5,
	COMPONENT_COMMAND_QUERY_REP = 6
}ComponentCommand;

typedef enum class tagComponentType_t : int
{
	COMPONENT_TYPE_NONE = 0,
	COMPONENT_TYPE_XMQ = 1,
	COMPONENT_TYPE_WEB = 2,
	COMPONENT_TYPE_HKD = 3,
	COMPONENT_TYPE_DHD = 4,
	COMPONENT_TYPE_ALM = 5
}ComponentType;

typedef struct tagComponentInfo_t
{
	ComponentType type;
	long long timestamp;
	std::string cid;
	std::string cname;									//可选参数
}ComponentInfo;

typedef struct tagComponentRequest_t
{
	ComponentInfo cinfo;
}ComponentRequest;

typedef struct tagComponentResponse_t
{
	int result;
	std::string cid;
	std::vector<ComponentInfo> cinfos;
}ComponentResponse;

typedef struct tagComponentPacket_t
{
	ComponentCommand command;							//信令类型
	void* data;											//请求或应答
}ComponentPacket;

//====================================状态部分=============================================//
typedef enum class tagStatusCommand_t : int
{
	STATUS_COMMAND_NONE = 0,
	STATUS_COMMAND_SET_REQ = 1,
	STATUS_COMMAND_SET_REP = 2,
	STATUS_COMMAND_QUERY_REQ = 3,
	STATUS_COMMAND_QUERY_REP = 3,
}StatusCommand;

typedef enum class tagStatusType_t : int
{
	STATUS_TYPE_NONE = 0,
	STATUS_TYPE_SAIL = 1,
	STATUS_TYPE_AUTO = 2,
	STATUS_TYPE_NAME = 3
}StatusType;

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


//====================================消息部分=============================================//

typedef enum class tagMessageType_t : int
{
	MESSAGE_TYPE_NONE = 0,
	MESSAGE_TYPE_ALARM = 1,
	MESSAGE_TYPE_ALGORITHM = 2,
	MESSAGE_TYPE_COMPONENT = 3,
	MESSAGE_TYPE_CREW = 4,
	MESSAGE_TYPE_DEVICE = 5,
	MESSAGE_TYPE_STATUS = 6,
	MESSAGE_TYPE_USER = 7
}MessageType;

typedef struct tagMessagePacket_t
{
	MessageType type;									//消息类型
	long long sequence;									//消息序列号
	long long timestamp;								//消息时间戳
	void* packet;										//消息数据
}MessagePacket;

#endif//MESSAGE_DEFS_H
