//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-07-20
//		Description :					人员类
//
//		History:						Author									Date										Description
//										王科威									2020-07-20									创建
//

#ifndef BASE_PERSON_ABSTRACT_PERSON_H
#define BASE_PERSON_ABSTRACT_PERSON_H

#include <string>

namespace base
{
	namespace person
	{
		class AbstractPerson
		{
		public:
			AbstractPerson(const std::string id);
			virtual ~AbstractPerson(void);

		public:
			//读/写人员ID标识
			//@Return : 人员ID标识
			inline const std::string getPersonID(void) const
			{
				return personID;
			}

			//读/写人员名称
			//@name : 人员名称,可以空
			//@Return : 人员名称
			inline void setPersonName(const std::string name)
			{
				personName = name;
			}
			inline const std::string getPersonName(void) const
			{
				return personName;
			}

			//读/写人员职位描述
			//@name : 职位描述,可以空
			//@Return : 职位描述
			inline void setPersonJob(const std::string job)
			{
				personJob = job;
			}
			inline const std::string getPersonJob(void) const
			{
				return personJob;
			}

			//添加人员照片数据
			//@picture : 人员照片数据,可以空
			//@Return : 包命令类型
			virtual int addPersonPicture(const std::string picture) = 0;

			//获取人员图片数据
			//@idx : 图片索引号
			//@Return : 人员图片
			virtual const std::string getPersonPicture(const int idx = 0) = 0;

		private:
			const std::string personID;
			std::string personName;
			std::string personJob;
		};//class AbstractPerson
	}//namespace person
}//namespace base

#endif//BASE_PERSON_ABSTRACT_PERSON_H
