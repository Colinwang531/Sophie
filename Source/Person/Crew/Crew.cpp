#include "Error.h"
#include "Person/Crew/Crew.h"

namespace base
{
	namespace person
	{
		Crew::Crew(const std::string id)
			: AbstractPerson(id)
		{}

		Crew::~Crew()
		{}

		int Crew::addPersonPicture(const std::string picture)
		{
			int e{ picture.empty() ? eInvalidParameter : eSuccess };

			if (eSuccess == e)
			{
				if (eMaxPicutreNumber > crewPictures.size())
				{
					crewPictures.push_back(picture);
				} 
				else
				{
					e = eOutOfRange;
				}
			}

			return e;
		}

		const std::string Crew::getPersonPicture(const int idx /* = 0 */)
		{
			std::string image;

			if (-1 < idx && idx < crewPictures.size())
			{
				image.append(crewPictures[idx]);
			}

			return image;
		}
	}//namespace packet
}//namespace base
