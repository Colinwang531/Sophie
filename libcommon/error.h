//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-27
//		Description :					libcommon库错误码表
//
//		History:						Author									Date										Description
//										王科威									 2020-04-27									 创建
//

#ifndef FRAMEWORK_LIBCOMMON_ERROR_DEFINE_H
#define FRAMEWORK_LIBCOMMON_ERROR_DEFINE_H

typedef enum class tagCommonError_t : int
{
    COMMON_ERROR_SUCCESS = 0,
    COMMON_ERROR_INVALID_PARAMETER = -1,
    COMMON_ERROR_BAD_NEW_MEMORY = -2,
    COMMON_ERROR_BAD_NEW_THREAD = -3,
    COMMON_ERROR_BAD_NEW_INSTANCE = -4,
    COMMON_ERROR_OUT_OF_RANGE = -5,
    COMMON_ERROR_NOT_SUPPORT = -6,
    COMMON_ERROR_EXISTED = -7,
    COMMON_ERROR_NOT_EXIST = -8,
    COMMON_ERROR_SET_FAILED = -9,
    COMMON_ERROR_EMPTY = -10,
    COMMON_ERROR_BAD_OPERATE = -11,
    COMMON_ERROR_BAD_FILE_OPEN = -12
}CommonError;

#endif//FRAMEWORK_LIBCOMMON_ERROR_DEFINE_H
