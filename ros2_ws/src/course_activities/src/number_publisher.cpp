#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/int64.hpp"

class NumberPublisherNode : public rclcpp::Node
{
public:
    NumberPublisherNode() : Node("Number_publisher")
    {
        this->declare_parameter("number_to_publish", 2);
        this->declare_parameter("publish_frequency", 1.0);

        number_ = this->get_parameter("number_to_publish").as_int();
        frequency_ = this->get_parameter("publish_frequency").as_double();

        publisher_ = this->create_publisher<example_interfaces::msg::Int64>("number", 10); // number: topic name ; 10: queue size // Intialization of the topic
        RCLCPP_INFO(this->get_logger(), "Number publisher node started");
        timer_ = this->create_wall_timer(std::chrono::milliseconds((int) (1000.0/ frequency_)),
                                         std::bind(&NumberPublisherNode::publishNumber, this));
    }

private:
    void publishNumber()
    {
        auto msg = example_interfaces::msg::Int64();
        msg.data = number_;
        publisher_->publish(msg);
    }

    int number_;
    double frequency_;
    rclcpp::TimerBase::SharedPtr timer_;                                     // timer declaration
    rclcpp::Publisher<example_interfaces::msg::Int64>::SharedPtr publisher_; // shared pointer of Publisher type
};

int main(int argc, char **argv)
{

    rclcpp::init(argc, argv);
    auto node = std::make_shared<NumberPublisherNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}