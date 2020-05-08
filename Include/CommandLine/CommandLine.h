//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-06
//		Description :					Ӧ�ó��������в���������
//
//		History:						Author									Date										Description
//										������									2020-05-06									����
//

#ifndef BASE_COMMAND_LINE_H
#define BASE_COMMAND_LINE_H

#include "boost/program_options.hpp"

namespace base
{
	namespace commandline
	{
		class CommandLine
		{
		public:
			CommandLine(void);
			virtual ~CommandLine(void);

		public:
			//����������ѡ��Ͷ�Ӧֵ,�÷���������parseCommandLine����ǰ������ѡ�Χ�����÷�Χ����
			//ѡ������֧��ͬʱʹ��ȫ���ͼ�д,��ʽ��"name,n"
			//@option : ��������
			//@param : ����ֵ
			//@Return : ������ֵ
			int setCommandOptions(const char* option = nullptr, const char* param = nullptr);

			//��ȡ������ѡ���ֵ
			//ѡ������֧��ͬʱʹ��ȫ���ͼ�д,��ʽ��"name,n"
			//@option : ��������
			//@Return : ����ֵ,Ϊ�ձ�ʾ����������Ч
			const char* getParameterByOption(const char* option = nullptr);

			//����������ѡ��
			int parseCommandLine(int argc, char** argv);

		private:
			boost::program_options::options_description desc;
			boost::program_options::variables_map variables;
		};//class CommandLine
	}//namespace commandline
}//namespace base

#endif//BASE_COMMAND_LINE_H
