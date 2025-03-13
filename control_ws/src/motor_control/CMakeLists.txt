cmake_minimum_required(VERSION 3.8)
project(motor_control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(pluginlib REQUIRED)
find_package(hardware_interface REQUIRED)

add_library(motor_hardware_interface SHARED
  src/motor_hardware_interface.cpp
)

target_link_libraries(motor_hardware_interface
  pluginlib::pluginlib
)

# export plugin
pluginlib_export_plugin_description_file(hardware_interface motor_control.xml)

ament_package()
