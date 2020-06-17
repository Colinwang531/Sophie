#include "boost/checked_delete.hpp"
#include "Error.h"
#include "Protocol/Message.pb.h"
#include "Protocol/Status/Status.pb.h"
#include "Protocol/MessageDefs.h"
#include "Protocol/Status/StatusCodec.h"

namespace base
{
	namespace protocol
	{
		StatusParser::StatusParser(){}
		StatusParser::~StatusParser(){}

		void* StatusParser::createNewStatus()
		{
			StatusPacket* sp{ new(std::nothrow) StatusPacket };

			if (sp)
			{
				sp->command = StatusCommand::STATUS_COMMAND_NONE;
				sp->data = nullptr;
			}

			return sp;
		}

		void StatusParser::destroyStatus(void* pkt /* = nullptr */)
		{
			StatusPacket* sp{ reinterpret_cast<StatusPacket*>(pkt) };

			if (sp)
			{
				if (StatusCommand::STATUS_COMMAND_SET_REQ == sp->command ||
					StatusCommand::STATUS_COMMAND_QUERY_REQ == sp->command)
				{
					boost::checked_delete(reinterpret_cast<StatusRequest*>(sp->data));
				}
				else if (StatusCommand::STATUS_COMMAND_SET_REP == sp->command ||
					StatusCommand::STATUS_COMMAND_QUERY_REP == sp->command)
				{
					boost::checked_delete(reinterpret_cast<StatusResponse*>(sp->data));
				}
			}

			boost::checked_delete(sp);
		}

		int StatusParser::unpackStatus(
			void* s /* = nullptr */, 
			void* pkt /* = nullptr */)
		{
			int e{ s && pkt ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				msg::Status* status{ reinterpret_cast<msg::Status*>(s) };
				StatusPacket* sp{ reinterpret_cast<StatusPacket*>(pkt) };
				sp->command = static_cast<StatusCommand>(status->command());

				if (StatusCommand::STATUS_COMMAND_SET_REQ == sp->command ||
					StatusCommand::STATUS_COMMAND_QUERY_REQ == sp->command)
				{
					StatusRequest* req{ new(std::nothrow) StatusRequest };

					if (req)
					{
						const msg::StatusRequest sr{ status->statusrequest() };
						req->type = static_cast<StatusType>(sr.type());
						if (sr.has_flag())
						{
							req->flag = sr.flag();
						}
						if (sr.has_name())
						{
							req->name = sr.name();
						}
						sp->data = req;
					} 
					else
					{
						e = eBadNewObject;
					}
				}
				else if (StatusCommand::STATUS_COMMAND_SET_REP == sp->command ||
					StatusCommand::STATUS_COMMAND_QUERY_REP == sp->command)
				{
					StatusResponse* rep{ new(std::nothrow) StatusResponse };

					if (rep)
					{
						const msg::StatusResponse sr{ status->statusresponse() };
						rep->result = sr.result();
						if (sr.has_flag())
						{
							rep->flag = sr.flag();
						}
						if (sr.has_name())
						{
							rep->name = sr.name();
						}
						sp->data = rep;
					}
					else
					{
						e = eBadNewObject;
					}
				}
			}

			return e;
		}
	}//namespace protocol
}//namespace base
