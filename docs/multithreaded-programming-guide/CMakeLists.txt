cmake_minimum_required(VERSION 3.5.0)
project(mpg-examples VERSION 0.1.0 LANGUAGES C)

include(CheckFunctionExists)

check_function_exists(pthread_mutex_getprioceiling HAVE_PTHREAD_PRIORITY_CEILING)
if(HAVE_PTHREAD_PRIORITY_CEILING)
    add_definitions(-DHAVE_PTHREAD_PRIORITY_CEILING)
endif()

# 定义源文件列表
set(SOURCE_FILES
    pthread_create.c
    pthread_detach.c
    pthread_setschedparam.c
    pthread_default_attr.c
    pthread_attr_setguardsize.c
    pthread_attr_setscope.c
    mutex_share.c
    pthread_mutex_recursive.c
    pthread_prio_inherit.c
    pthread_mutex_setprioceiling.c
)

# 判断是否是 Linux 系统
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND SOURCE_FILES 
    pthread_setaffinity_np.c
    pthread_mutexattr_setrobust.c
    ) # _np只能运行在linux系统中
endif()

# 遍历源文件列表，为每个源文件添加可执行目标
foreach(SOURCE_FILE ${SOURCE_FILES})
    get_filename_component(TARGET_NAME ${SOURCE_FILE} NAME_WE) # 获取文件名称
    add_executable(${TARGET_NAME} src/${SOURCE_FILE})
endforeach()
