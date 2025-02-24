#include"rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/string.hpp"

class RobotNewsStationNode: public rclcpp::Node
{
    public:
        RobotNewsStationNode(): Node("robot_news_station")  //("robot_news_station") is passed to the Node class and initialised as name of node
        {
            this->declare_parameter("robot_name", "R2");
            robotName = this->get_parameter("robot_name").as_string();
            publisher_ = this->create_publisher<example_interfaces::msg::String>("robot_news", 10); // robot_new: topic name ; 10: queue size // Intialization of the topic
            timer_ = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&RobotNewsStationNode::publishNews, this));
            RCLCPP_INFO(get_logger(), "Robot news station started.");
        }

    private:
        void publishNews(){
            auto msg = example_interfaces::msg::String();
            msg.data = std::string("Hi, this is ") + robotName + std::string(" from the Robot News Station");
            publisher_ ->publish(msg);
        }

        std::string robotName;
        rclcpp::Publisher<example_interfaces::msg::String>::SharedPtr publisher_;  //shared pointer of Publisher type created
        rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv){

    rclcpp::init(argc, argv);
    auto node = std::make_shared<RobotNewsStationNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}