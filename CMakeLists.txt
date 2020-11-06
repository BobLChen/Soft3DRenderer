cmake_minimum_required(VERSION 3.12.0)

set(NAME Soft3DRenderer)

project(${NAME})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DNOMINMAX=1")

include_directories(
	include/
)

set(MAIN_SRCS
    include/Matrix3D.h
    include/mmath.h
    include/OBJLoader.h
    include/stb_image_write.h
    include/Vector2D.h
    include/Vector3D.h
)

set(MAIN_HDRS
    src/main.cpp
    src/Matrix3D.cpp
    src/OBJLoader.cpp
    src/Vector2D.cpp
    src/Vector3D.cpp
)

add_executable(${NAME} 
    ${MAIN_SRCS}
    ${MAIN_HDRS}
)

source_group(source FILES ${MAIN_SRCS} ${MAIN_HDRS})

add_custom_command(TARGET ${NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${NAME}>/assets/
)