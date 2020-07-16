#include "boost/checked_delete.hpp"
#include "boost/polymorphic_cast.hpp"
#include "Packet/Message/MessagePacket.h"
using AbstractPacket = base::packet::AbstractPacket;
using MessagePacket = base::packet::MessagePacket;
#include "Protocol/Message.pb.h"
#include "Protocol/Algorithm/Algorithm.pb.h"
#include "Protocol/Algorithm/AlgorithmPhrase.h"

namespace base
{
	namespace protocol
	{
		AlgorithmParser::AlgorithmParser(){}
		AlgorithmParser::~AlgorithmParser(){}

		void* AlgorithmParser::unpackFromAlgorithmMessage(void* a /* = nullptr */)
		{
			AbstractPacket* ap{ nullptr };
			msg::Algorithm* ma{ reinterpret_cast<msg::Algorithm*>(a) };

			if (ma)
			{

			}

			return ap;
		}

		AlgorithmPacker::AlgorithmPacker() {}
		AlgorithmPacker::~AlgorithmPacker() {}

		void* AlgorithmPacker::packAlgorithm(
			const int command /* = 0 */,
			const int result /* = 0 */,
			void* data /* = nullptr */)
		{
			const msg::Algorithm_Command cmd{ static_cast<msg::Algorithm_Command>(command) };
			msg::Algorithm* s{ msg::Algorithm().New() };

			if (s)
			{
				s->set_command(cmd);

				if (msg::Algorithm_Command::Algorithm_Command_CONFIGURE_REP == cmd)
				{
					msg::AlgorithmResponse* rep{ s->mutable_algorithmresponse() };
					rep->set_result(result);
				}
			}

			return s;
		}
	}//namespace protocol
}//namespace base
