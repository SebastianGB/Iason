cmake_minimum_required (VERSION 2.6)
project (IasonSoftware)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

FILE(GLOB SRCS_IASON "*.cpp" "*.h" "GCSCommunication/*.cpp" "GCSCommunication/*.h" "WheelPlatform/*.cpp" "WheelPlatform/*.h")

ADD_EXECUTABLE( iasonSoftware ${SRCS_IASON} )
#include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
target_link_libraries (iasonSoftware pthread)
