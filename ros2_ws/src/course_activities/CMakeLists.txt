cmake_minimum_required(VERSION 3.8)
project(course_activities)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(my_robot_interfaces REQUIRED)
find_package(turtlesim REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(number_publisher src/number_publisher.cpp)                     #"cpp_node" name of executable
ament_target_dependencies(number_publisher rclcpp example_interfaces)     #these two lines create an executable but not install it

add_executable(number_counter src/number_counter.cpp)                     #"cpp_node" name of executable
ament_target_dependencies(number_counter rclcpp example_interfaces)     #these two lines create an executable but not install it

add_executable(reset_counter_client src/reset_counter_client.cpp)                     #"cpp_node" name of executable
ament_target_dependencies(reset_counter_client rclcpp example_interfaces)     #these two lines create an executable but not install it

add_executable(battery_status src/battery_status.cpp)                     #"cpp_node" name of executable
ament_target_dependencies(battery_status rclcpp my_robot_interfaces)     #these two lines create an executable but not install it

add_executable(led_panel src/led_panel.cpp)                     #"cpp_node" name of executable
ament_target_dependencies(led_panel rclcpp my_robot_interfaces)     #these two lines create an executable but not install it

add_executable(new_turtle src/new_turtle.cpp)                     #"cpp_node" name of executable
ament_target_dependencies(new_turtle rclcpp my_robot_interfaces turtlesim)     #these two lines create an executable but not install it

add_executable(controller src/controller.cpp)                     #"cpp_node" name of executable
ament_target_dependencies(controller rclcpp my_robot_interfaces turtlesim geometry_msgs)     #these two lines create an executable but not install it

install(TARGETS            #installs the executable
  number_publisher
  number_counter
  reset_counter_client
  battery_status
  led_panel
  new_turtle
  controller
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
