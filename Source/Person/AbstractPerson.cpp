#include "Person/AbstractPerson.h"

namespace base
{
	namespace person
	{
		AbstractPerson::AbstractPerson(const std::string id) : personID{ id }
		{}
		AbstractPerson::~AbstractPerson(){}
	}//namespace packet
}//namespace base
