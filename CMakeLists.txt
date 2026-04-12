cmake_minimum_required(VERSION 3.8)
project(detecting_groups LANGUAGES CXX CUDA)
find_package(ament_cmake REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED True)
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES OR CMAKE_CUDA_ARCHITECTURES STREQUAL "")
	set(CMAKE_CUDA_ARCHITECTURES native)
endif()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Collect cpp files and target them for compilation, then add our headers
set(SOURCES
	src/main.cpp
	src/DetectingGroupsNode.cpp
	src/InputImageHandler.cpp
	src/Tokenizer.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Dependencies
find_package(OpenCV 4 REQUIRED HINTS "/opt/cudaopencv")
find_package(CUDAToolkit REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(detecting_groups_custom_msg REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(isaac_ros_managed_nitros REQUIRED)
find_package(isaac_ros_nitros_image_type REQUIRED)
find_package(llama REQUIRED)

# Link standard CMAKE libs
target_link_libraries(${PROJECT_NAME}
	${OpenCV_LIBS}
	CUDA::cudart
	llama
	mtmd
)

# Link special ROS2 libs
ament_target_dependencies(${PROJECT_NAME}
	rclcpp
	std_msgs
	detecting_groups_custom_msg
	ament_index_cpp
	isaac_ros_managed_nitros
	isaac_ros_nitros_image_type
)

install(TARGETS
	detecting_groups
	DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/gemma-model"
	DESTINATION share/${PROJECT_NAME}
)

ament_package()
