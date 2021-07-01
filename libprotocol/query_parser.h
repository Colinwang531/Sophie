//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-06-29
//		Description :					查询解析类
//
//		History:						Author									Date										Description
//										王科威									 2021-06-29									 创建
//

#ifndef FRAMEWORK_LIBPROTOCOL_QUERY_PARSER_H
#define FRAMEWORK_LIBPROTOCOL_QUERY_PARSER_H

#include <vector>
#include "defs.h"

namespace framework
{
    namespace libprotocol
    {
        typedef enum class tagQueryType_t : int
        {
            QUERY_TYPE_NONE = 0,
            QUERY_TYPE_REQ,
            QUERY_TYPE_REP
        }QueryType;

        class IQueryParser;

        class QueryParser
        {
        public:
            QueryParser(void);
            ~QueryParser(void);

        public:
            //解析
			//@data : 数据
            //@Return : 错误码
            int parse(const std::string data);

            //编码
            //@Return : 数据序列化字符串
            const std::string compose(void);

            //获取查询类型
            //@Return ：查询类型
            const QueryType getQueryType(void) const;

            //设置查询类型
            //@type : 查询类型
            //@Return ：错误码
            int setQueryType(
                const QueryType type = QueryType::QUERY_TYPE_NONE);

            //获取查询应答状态码
            //@Return : 查询应答状态码
            const int getQueryStatusCode(void) const;

            //设置查询应答状态码
            //@info : 查询应答状态码
            //@Return ：错误码
            int setQueryStatusCode(const int sc = -1);

            //获取应用程序信息
            //@Return ：应用程序信息
            const std::vector<ApplicationInfo> getApplicationInfos(void) const;

            //设置应用程序信息
            //@info : 应用程序信息
            //@Return ：错误码
            int setApplicationInfos(const std::vector<ApplicationInfo> infos);

        private:
            IQueryParser* parser;
        };//class QueryParser
    }//namespace libprotocol
}//namespace framework

#endif//FRAMEWORK_LIBPROTOCOL_QUERY_PARSER_H