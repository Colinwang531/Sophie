//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					��Ա��
//
//		History:						Author									Date										Description
//										������									2020-07-20									����
//

#ifndef BASE_PERSON_CREW_H
#define BASE_PERSON_CREW_H

#include <string>
#include <vector>
#include "Person/AbstractPerson.h"

namespace base
{
	namespace person
	{
		class Crew : public AbstractPerson
		{
		public:
			Crew(const std::string id);
			virtual ~Crew(void);

		protected:
			//�����Ա��Ƭ����
			//@picture : ��Ա��Ƭ����,���Կ�
			//@Return : ����������
			int addPersonPicture(const std::string picture) override;

		private:
			enum
			{
				eMaxPicutreNumber = 3
			};
			std::vector<std::string> crewPictures;
		};//class Crew
	}//namespace person
}//namespace base

#endif//BASE_PERSON_CREW_H
