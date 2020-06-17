#include "Error.h"
#include "Protocol/Message.pb.h"
#include "Protocol/Algorithm/Algorithm.pb.h"
#include "Protocol/Algorithm/AlgorithmCodec.h"

namespace base
{
	namespace protocol
	{
		AlgorithmCodec::AlgorithmCodec(){}
		AlgorithmCodec::~AlgorithmCodec(){}

		int AlgorithmCodec::unpackAlgorithm(void* msg /* = nullptr */)
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
