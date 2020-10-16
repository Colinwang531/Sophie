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
			//��/д��ԱID��ʶ
			//@Return : ��ԱID��ʶ
			inline const std::string getPersonID(void) const
			{
				return personID;
			}

			//��/д��Ա����
			//@name : ��Ա����,���Կ�
			//@Return : ��Ա����
			inline void setPersonName(const std::string name)
			{
				personName = name;
			}
			inline const std::string getPersonName(void) const
			{
				return personName;
			}

			//��/д��Աְλ����
			//@name : ְλ����,���Կ�
			//@Return : ְλ����
			inline void setPersonJob(const std::string job)
			{
				personJob = job;
			}
			inline const std::string getPersonJob(void) const
			{
				return personJob;
			}

			//�����Ա��Ƭ����
			//@picture : ��Ա��Ƭ����,���Կ�
			//@Return : ����������
			virtual int addPersonPicture(const std::string picture) = 0;

			//��ȡ��ԱͼƬ����
			//@idx : ͼƬ������
			//@Return : ��ԱͼƬ
			virtual const std::string getPersonPicture(const int idx = 0) = 0;

		private:
			const std::string personID;
			std::string personName;
			std::string personJob;
		};//class AbstractPerson
	}//namespace person
}//namespace base

#endif//BASE_PERSON_ABSTRACT_PERSON_H
