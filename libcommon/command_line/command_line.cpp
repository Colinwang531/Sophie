#include "boost/checked_delete.hpp"
#include "boost/program_options.hpp"
#include "error.h"
#include "command_line.h"

namespace framework
{
	namespace libcommon
	{
		class Impl
		{
		public:
			Impl(void);
			~Impl(void);

		public:
			int setOption(
				const char* opt = nullptr, 
				const char* val = nullptr);
			const char* getValue(const char* opt = nullptr);
			int parse(
				const int argc = 0, 
				const char** argv = nullptr);

		private:
			boost::program_options::options_description desc;
			boost::program_options::variables_map variables;
		};//class Impl

		Impl::Impl()
		{}
		Impl::~Impl()
		{}

		int Impl::setOption(
			const char* option /* = nullptr */, 
			const char* param /* = nullptr */)
		{
			CommonError e{ 
				option && param ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

			if (CommonError::COMMON_ERROR_SUCCESS == e)
			{
				desc.add_options()(option, boost::program_options::value<std::string>(), param);
			}

			return static_cast<int>(e);
		}

		const char* Impl::getValue(const char* option /* = nullptr */)
		{
			return 0 < variables.count(option) ? variables[option].as<const char*>() : nullptr;
		}

		int Impl::parse(
			const int argc/* = 0*/, 
			const char** argv/* = nullptr*/)
		{
			CommonError e{ 
				0 < argc && argv ? CommonError::COMMON_ERROR_SUCCESS : CommonError::COMMON_ERROR_INVALID_PARAMETER };

			if (CommonError::COMMON_ERROR_SUCCESS == e)
			{
				boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), variables);
			}

			return static_cast<int>(e);
		}

		CommandLineParser::CommandLineParser() : impl{new(std::nothrow) Impl}
		{}
		CommandLineParser::~CommandLineParser()
		{
			boost::checked_delete(impl);		
		}

		int CommandLineParser::setOption(
			const char* option /* = nullptr */, 
			const char* param /* = nullptr */)
		{
			return impl ? impl->setOption(option, param) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
		}

		const char* CommandLineParser::getValue(const char* option /* = nullptr */)
		{
			return impl ? impl->getValue(option) : nullptr;
		}

		int CommandLineParser::parse(
			const int argc/* = 0*/, 
			const char** argv/* = nullptr*/)
		{
			return impl ? impl->parse(argc, argv) : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE;
		}
	}//namespace libcommon
}//namespace framework
