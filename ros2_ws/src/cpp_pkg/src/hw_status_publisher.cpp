#include "rclcpp/rclcpp.hpp"
#include "my_robot_interfaces/msg/hardware_status.hpp"

class HardwareStatusPublisher: public rclcpp::Node
{
    public:
        HardwareStatusPublisher(): Node("hardware_status_publisher")
        {
            publisher_ = this->create_publisher<my_robot_interfaces::msg::HardwareStatus>(
                "hardware_status", 10); // hardware_status: topic name ; 10: queue size // Intialization of the topic
            timer_ = this->create_wall_timer(std::chrono::seconds(1), std::bind(&HardwareStatusPublisher::publishHardwareStatus, this));
            RCLCPP_INFO(get_logger(), "Hardware status publisher strted.");
        }

    private:
        void publishHardwareStatus(){
            auto data = my_robot_interfaces::msg::HardwareStatus();
            data.temperature = 47;
            data.are_motor_ready = true;
            data.debug_message = "Motors are cool";
            publisher_ ->publish(data);
        }

        rclcpp::Publisher<my_robot_interfaces::msg::HardwareStatus>::SharedPtr publisher_;  //shared pointer of Publisher type created
        rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv){

    rclcpp::init(argc, argv);
    auto node = std::make_shared<HardwareStatusPublisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}