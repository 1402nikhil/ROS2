// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from my_robot_interfaces:msg/RobotLocation.idl
// generated code does not contain a copyright notice

#ifndef MY_ROBOT_INTERFACES__MSG__DETAIL__ROBOT_LOCATION__BUILDER_HPP_
#define MY_ROBOT_INTERFACES__MSG__DETAIL__ROBOT_LOCATION__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "my_robot_interfaces/msg/detail/robot_location__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace my_robot_interfaces
{

namespace msg
{

namespace builder
{

class Init_RobotLocation_name
{
public:
  explicit Init_RobotLocation_name(::my_robot_interfaces::msg::RobotLocation & msg)
  : msg_(msg)
  {}
  ::my_robot_interfaces::msg::RobotLocation name(::my_robot_interfaces::msg::RobotLocation::_name_type arg)
  {
    msg_.name = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_robot_interfaces::msg::RobotLocation msg_;
};

class Init_RobotLocation_y
{
public:
  explicit Init_RobotLocation_y(::my_robot_interfaces::msg::RobotLocation & msg)
  : msg_(msg)
  {}
  Init_RobotLocation_name y(::my_robot_interfaces::msg::RobotLocation::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_RobotLocation_name(msg_);
  }

private:
  ::my_robot_interfaces::msg::RobotLocation msg_;
};

class Init_RobotLocation_x
{
public:
  Init_RobotLocation_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_RobotLocation_y x(::my_robot_interfaces::msg::RobotLocation::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_RobotLocation_y(msg_);
  }

private:
  ::my_robot_interfaces::msg::RobotLocation msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_robot_interfaces::msg::RobotLocation>()
{
  return my_robot_interfaces::msg::builder::Init_RobotLocation_x();
}

}  // namespace my_robot_interfaces

#endif  // MY_ROBOT_INTERFACES__MSG__DETAIL__ROBOT_LOCATION__BUILDER_HPP_
