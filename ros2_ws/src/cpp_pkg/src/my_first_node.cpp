#include "rclcpp/rclcpp.hpp"

class MyNode : public rclcpp::Node
{
public:
    MyNode() : Node("cpp_test"), counter_(0)
    {
        RCLCPP_INFO(this->get_logger(), "Hello cpp node");

        timer_ = this->create_wall_timer(std::chrono::seconds(1),
                                         std::bind(&MyNode::timeCallback, this));
    }

private:
    void timeCallback()
    {
        counter_++;
        RCLCPP_INFO(this->get_logger(), "Hello %d", counter_);
    }

    rclcpp::TimerBase::SharedPtr timer_; // timer declaration
    int counter_;
};

int main(int argc, char **argv)
{

    rclcpp::init(argc, argv); // initialises ros2 communication
    auto node = std::make_shared<MyNode>();
    rclcpp::spin(node); // keeps the node alive
    rclcpp::shutdown(); // shuts down the ros2 communicaton
    return 0;
}