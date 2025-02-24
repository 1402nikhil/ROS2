#include "rclcpp/rclcpp.hpp"
#include "my_robot_interfaces/srv/set_led.hpp"

class BatteryStatusNode: public rclcpp::Node
{
    public:
        BatteryStatusNode(): Node("battery_status"), delay(6)
        {
            timer_ = this->create_wall_timer(std::chrono::seconds(delay), std::bind(&BatteryStatusNode::callback, this));
            RCLCPP_INFO(this->get_logger(), "Battery status node started");
        }

    private:
        void callback(){
            threads_.push_back(std::thread(std::bind(&BatteryStatusNode::callbackBatteryStatus, this)));
        }

        void callbackBatteryStatus(){
            auto client_ = this->create_client<my_robot_interfaces::srv::SetLed>("set_led"); //set_led: service name

            while(!client_->wait_for_service(std::chrono::seconds(1))){
                RCLCPP_WARN(this->get_logger(), "Waiting for server to be up...");
            }

            auto request = std::make_shared<my_robot_interfaces::srv::SetLed::Request>();

            if(delay == 6){
                request->led_number = 1;
                request->state = true;
                delay = 4;
            }
            else if(delay == 4){
                request->led_number = 1;
                request->state = false;
                delay = 6;
            }

            auto future = client_->async_send_request(request);

            try{
                auto response = future.get();
                if(request->state == true)
                    RCLCPP_INFO(this->get_logger(), "Battery low, Led on. delay: %d", (int)delay);
                else if(request->state == false)
                    RCLCPP_INFO(this->get_logger(), "Battery full, Led off. delay: %d", (int)delay);
            }
            catch(const std::exception& e){
                RCLCPP_ERROR(this->get_logger(), "Service call failed");
            }
        }

        std::vector<std::thread> threads_;
        rclcpp::TimerBase::SharedPtr timer_;
        int64_t delay;
};

int main(int argc, char **argv){

    rclcpp::init(argc, argv);
    auto node = std::make_shared<BatteryStatusNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}