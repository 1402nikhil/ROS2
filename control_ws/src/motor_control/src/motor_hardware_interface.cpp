#include "motor/motor_hardware_interface.hpp"

namespace motor_control
{
    CallbackReturn MotorHardwareInterface::on_init(const hardware_interface::HardwareInfo &info){
        if(hardware_interface::ActuatorInterface::on_init(info) != CallbackReturn::SUCCESS)
            return CallbackReturn::SUCCESS;
            
        motor_position_ = 0.0;
        motor_velocity_ = 0.0;
        motor_command_ = 0.0;

        return CallbackReturn::SUCCESS;
    }

    CallbackReturn MotorHardwareInterface::on_configure(const rclcpp_lifecycle::State &){
        node_ = std::make_shared<rclcpp::Node>("motor_hardware_interface");

        motor_speed_publisher_ = node_->create_publisher<std_msgs::msg::Float32>("motor_speed", 10);
        motor_speed_subscriber_ = node_->create_subscription<std_msgs::msg::Float32>(
            "motor_command", 10, std::bind(&MotorHardwareInterface::motor_speed_callback, this, std::placeholders::_1));

        return CallbackReturn::SUCCESS;
    }

    CallbackReturn MotorHardwareInterface::on_cleanup(const rclcpp_lifecycle::State &){
        motor_speed_publisher_.reset();
        motor_speed_subscriber_.reset();
        node_.reset();

        return CallbackReturn::SUCCESS;
    }

    CallbackReturn MotorHardwareInterface::on_activate(const rclcpp_lifecycle::State &){
        RCLCPP_INFO(rclcpp::get_logger("MotorHardwareInterface"), "Activating motor hardware interface");
        return CallbackReturn::SUCCESS;
    }

    CallbackReturn MotorHardwareInterface::on_deactivate(const rclcpp_lifecycle::State &){
        RCLCPP_INFO(rclcpp::get_logger("MotorHardwareInterface"), "Deactivating motor hardwareinterface");
    }

    return_type MotorHardwareInterface::read(const rclcpp::Time &time, const rclcpp::Duration &period){
        motor_position_ += motor_velocity_ * period.seconds();
        auto msg = std_msgs::msg::Float32();
        msg.data = motor_velocity_;
        motor_speed_publisher_->publish(msg);
        return return_type::OK;
    }

    return_type MotorHardwareInterface::write(const rclcpp::Time &time, const rclcpp::Duration &period){
        motor_velocity_ = motor_command_;
        return return_type::OK;
    }

    void MotorHardwareInterface::motor_speed_callback(const std_msgs::msg::Float32::SharedPtr msg){
        motor_command_ = msg->data;
    }
}

PLUGINLIB_EXPORT_CLASS(motor_control::MotorHardwareInterface, hardware_interface::ActuatorInterface)