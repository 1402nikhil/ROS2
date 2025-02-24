#include "rclcpp/rclcpp.hpp"
#include "my_robot_interfaces/srv/set_led.hpp"
#include "my_robot_interfaces/msg/led_status.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

class ledPanelNode: public rclcpp::Node
{
    public:
        ledPanelNode(): Node("led_panel")
        {
            this->declare_parameter("led_initial_status", std::vector<bool>{0,0,0});
            led_states_ = this->get_parameter("led_initial_status").as_bool_array();
            server_ = this->create_service<my_robot_interfaces::srv::SetLed>("set_led",
            std::bind(&ledPanelNode::callbackSetLed, this, _1, _2));
            publisher_ = this->create_publisher<my_robot_interfaces::msg::LedStatus>("led_panel_state", 10);
            RCLCPP_INFO(this->get_logger(), "Server node started.");
        }

    private:
        void callbackSetLed(const my_robot_interfaces::srv::SetLed::Request::SharedPtr request,
        const my_robot_interfaces::srv::SetLed::Response::SharedPtr response){
            auto status = my_robot_interfaces::msg::LedStatus();
            if(request->state == true){
                ledState[request->led_number - 1] = true;
                response->success = true;
                led_states_.at(request->led_number - 1) = 1;
                status.led_state = led_states_;
                publisher_->publish(status);
                RCLCPP_INFO(this->get_logger(), "Led %d is on", (int)request->led_number);
            }
            else if(request->state == false){
                ledState[request->led_number - 1] = false;
                response->success = true;
                led_states_.at(request->led_number - 1) = 0;
                status.led_state = led_states_;
                publisher_->publish(status);
                RCLCPP_INFO(this->get_logger(), "Led %d is off", (int)request->led_number);
            }

        }

        rclcpp::Service<my_robot_interfaces::srv::SetLed>::SharedPtr server_;
        rclcpp::Publisher<my_robot_interfaces::msg::LedStatus>::SharedPtr publisher_;
        bool ledState[3];
        std::vector<bool> led_states_;
};

int main(int argc, char **argv){

    rclcpp::init(argc, argv);
    auto node = std::make_shared<ledPanelNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}