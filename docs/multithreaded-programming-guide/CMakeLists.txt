cmake_minimum_required(VERSION 3.5.0)
project(mpg-examples VERSION 0.1.0 LANGUAGES C)


# 定义源文件列表
set(SOURCE_FILES
    pthread_create.c
    pthread_detach.c
    pthread_setschedparam.c
)

# 遍历源文件列表，为每个源文件添加可执行目标
foreach(SOURCE_FILE ${SOURCE_FILES})
    get_filename_component(TARGET_NAME ${SOURCE_FILE} NAME_WE) # 获取文件名称
    add_executable(${TARGET_NAME} src/${SOURCE_FILE})
endforeach()
