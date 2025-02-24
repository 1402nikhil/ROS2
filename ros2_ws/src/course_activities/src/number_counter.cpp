#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/int64.hpp"
#include "example_interfaces/srv/set_bool.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

class NumberCounterNode: public rclcpp::Node
{
    public:
        NumberCounterNode(): Node("Number_counter")
        {
            subscriber_ = this->create_subscription<example_interfaces::msg::Int64>(
                "number", 10, 
                std::bind(&NumberCounterNode::callbackNumberPublish, this, std::placeholders::_1));
            publisher_ = this->create_publisher<example_interfaces::msg::Int64>("number_count", 10); // number_count: topic name ; 10: queue size // Intialization of the topic
            server_ = this->create_service<example_interfaces::srv::SetBool>(
                "reset_counter",
                std::bind(&NumberCounterNode::callbackResetCounter, this, _1, _2));
            RCLCPP_INFO(this->get_logger(), "Number counter started");
        }

    private:
        void callbackResetCounter(const example_interfaces::srv::SetBool::Request::SharedPtr request,
        const example_interfaces::srv::SetBool::Response::SharedPtr response){
            if(request->data == true){
                counter = 0;
                response->success = true;
                RCLCPP_INFO(this->get_logger(), "Counter reset successful");
            }
            else{
                response->success = false;
                RCLCPP_WARN(this->get_logger(), "Counter not reset");
            }
        }

        void callbackNumberPublish(const example_interfaces::msg::Int64::SharedPtr msg){
            counter += msg->data;
            auto newMsg = example_interfaces::msg::Int64();
            newMsg.data = counter;
            publisher_ ->publish(newMsg);
            RCLCPP_INFO(this->get_logger(), "Counter count: %d", counter);
        }

        int counter;
        rclcpp::Subscription<example_interfaces::msg::Int64>::SharedPtr subscriber_;
        rclcpp::Publisher<example_interfaces::msg::Int64>::SharedPtr publisher_;
        rclcpp::Service<example_interfaces::srv::SetBool>::SharedPtr server_;     //Server declaration
};

int main(int argc, char **argv){

    rclcpp::init(argc, argv);
    auto node = std::make_shared<NumberCounterNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}