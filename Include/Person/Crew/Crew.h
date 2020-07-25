//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					组员类
//
//		History:						Author									Date										Description
//										王科威									2020-07-20									创建
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
			//添加人员照片数据
			//@picture : 人员照片数据,可以空
			//@Return : 包命令类型
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
