PROJECT(TTS CXX C)

set(APP_NAME "TTS")

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

SET( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#TODO: add flags
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

FILE(GLOB SRCS_TTS "src/espeak.cpp")

FIND_PACKAGE( Portaudio REQUIRED )
MESSAGE(STATUS "portaudio dirs : ${Portaudio_INCLUDE_DIR} ${Portaudio_LIBRARY}")
FIND_PACKAGE( Espeak REQUIRED )
MESSAGE(STATUS "portaudio dirs : ${Espeak_INCLUDE_DIR} ${Espeak_LIBRARY}")

INCLUDE_DIRECTORIES( ${Portaudio_PROCESS_INCLUDES} )
INCLUDE_DIRECTORIES( ${Espeak_PROCESS_INCLUDES} )

ADD_EXECUTABLE( ${APP_NAME} ${SRCS_TTS} )
TARGET_LINK_LIBRARIES( ${APP_NAME}  ${Portaudio_LIBRARY} ${Espeak_LIBRARY} stdc++)







