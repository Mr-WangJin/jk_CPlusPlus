
SET(PROJECT_NAME "JKCore")

ADD_DEFINITIONS(-D_JK_BUILD_DLL)

## common 公共操作
FILE(GLOB JK_COMMON_LIST ./Common/*)
SOURCE_GROUP(JKCommon FILES ${JK_COMMON_LIST})

FILE(GLOB JK_Framework_LIST ./Framework/*)
SOURCE_GROUP(JKFramework FILES ${JK_Framework_LIST})

FILE(GLOB JK_Exception_LIST ./Exception/*)
SOURCE_GROUP(JKException FILES ${JK_Exception_LIST})

FILE(GLOB JK_Log_LIST ./Log/*)
SOURCE_GROUP(JKLog FILES ${JK_Log_LIST})

## c++ file 所有关于文件的操作
FILE(GLOB JK_FILE_LIST ./File/*)
SOURCE_GROUP(JKFile FILES ${JK_FILE_LIST})

## string 所有关于字符串的操作
FILE(GLOB JK_STRING_LIST ./String/*)
SOURCE_GROUP(JKString FILES ${JK_STRING_LIST})

## error 
FILE(GLOB JK_ERROR_LIST ./Error/*)
SOURCE_GROUP(JKError FILES ${JK_ERROR_LIST})


## thread
FILE(GLOB JK_WIN_THREAD_SRC ./jk_win_thread/*)
SOURCE_GROUP(jk_win_thread FILES ${JK_WIN_THREAD_SRC})

## std thread
FILE(GLOB JK_THREAD_SRC ./thread/*)
SOURCE_GROUP(JKThread FILES ${JK_THREAD_SRC})

## framework
FILE(GLOB JK_FRAMEWORK_SRC ./jk_framework/*)
SOURCE_GROUP(jk_framework FILES ${JK_FRAMEWORK_SRC})

SET(src_list
	lib_jk.h	
	${JK_COMMON_LIST}
	${JK_Framework_LIST}
	${JK_Log_LIST}
	${JK_FILE_LIST}
	${JK_STRING_LIST}
	${JK_WIN_THREAD_SRC}
	${JK_THREAD_SRC}
	${JK_FRAMEWORK_SRC}
	${JK_ERROR_LIST}
	${JK_Exception_LIST}
)

#ADD_LIBRARY(${PROJECT_NAME} SHARED ${src_list})
INCLUDE_DIRECTORIES( ./ )
ADD_LIBRARY(${PROJECT_NAME} ${BUILD_DYNAMIC_OR_STATIC} ${src_list})

IF(WIN32)
#	TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
#		debug glfw3.lib optimized glfw3.lib
#	)
ENDIF(WIN32)
if(UNIX)
#	TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
#		debug glfw3.so optimized glfw3.so
#	)
ENDIF(UNIX)