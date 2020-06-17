#include "Error.h"
#include "Protocol/Message.pb.h"
#include "Protocol/Device/Device.pb.h"
#include "Protocol/Device/DeviceCodec.h"

namespace base
{
	namespace protocol
	{
		DeviceCodec::DeviceCodec(){}
		DeviceCodec::~DeviceCodec(){}

		int DeviceCodec::unpackDevice(void* msg /* = nullptr */)
		{
			int e{ msg ? eSuccess : eInvalidParameter };

// 			if (eSuccess == e)
// 			{
// 				msg::MSG* m{ reinterpret_cast<msg::MSG*>(msg) };
// 
// 				if (m && m->has_component())
// 				{
// 					const msg::Component c{ m->component() };
// 				}
// 			}

			return e;
		}
	}//namespace protocol
}//namespace base
