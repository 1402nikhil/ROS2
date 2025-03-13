#ifndef MOTOR_HARDWARE_INTERFACE_HPP
#define MOTOR_HARDWARE_INTERFACE_hpp

#include <rclcpp/rclcpp.hpp>
#include <hardware_interface/actuator_interface.hpp>  //according to the type of hardware
#include <rclcpp/macros.hpp>
#include <std_msgs/msg/float32.hpp>
#include <pluginlib/class_list_macros.hpp>

using hardware_interface::CallbackReturn;
using hardware_interface::return_type;

namespace motor_control
{
    class MotorHardwareInterface: public hardware_interface::ActuatorInterface
    {
        public:
            RCLCPP_SHARED_PTR_DEFINITIONS(MotorHardwareInterface)  //simplifies to create a shred pointer for a class

            //Lifecycle management methods
            CallbackReturn on_init(const hardware_interface::HardwareInfo &info) override;
            CallbackReturn on_configure(const rclcpp_lifecycle::State &previous_state) override;
            CallbackReturn on_cleanup(const rclcpp_lifecycle::State &previous_state) override;
            CallbackReturn on_shutdown(const rclcpp_lifecycle::State &previos_state) override;
            CallbackReturn on_activate(const rclcpp_lifecycle::State &previos_state) override;
            CallbackReturn on_deactivate(const rclcpp_lifecycle::State &previos_state) override;
            CallbackReturn on_error(const rclcpp_lifecycle::State &previos_state) override;

            //State and command interface methods
            std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
            std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

            //Read/Write functions
            return_type read(const rclcpp::Time &time, const rclcpp::Duration &period) override;
            return_type write(const rclcpp::Time &time, const rclcpp::Duration &period) override;

            void motor_speed_callback(const std_msgs::msg::Float32::SharedPtr);

            private:

                //Node and micro-ros communication
                rclcpp::Node::SharedPtr node_;
                rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr motor_speed_publisher_;
                rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr motor_speed_subscriber_;

                float motor_command_; //stores commanded motor velocity
                float motor_position_; //stores encoder feedback
                float motor_velocity_;

                
        };
}  //namespace motor_control

#endif