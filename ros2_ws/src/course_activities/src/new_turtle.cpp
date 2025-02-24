#include "rclcpp/rclcpp.hpp"
#include "my_robot_interfaces/msg/robot_location.hpp"
#include "turtlesim/srv/spawn.hpp"

class NewTurtleNode: public rclcpp::Node
{
    public:
        NewTurtleNode(): Node("new_turtle"), xRange(11), yRange(11), angleRange(6.28), turtleCounter(1)
        {
            this->declare_parameter("delay", 1);   //delay between spawning two turtles in seconds
            timer_ = this->create_wall_timer(std::chrono::seconds(2), std::bind(&NewTurtleNode::callback, this));
            publisher_ = this->create_publisher<my_robot_interfaces::msg::RobotLocation>("turtle_location", 10); // number: topic name ; 10: queue size // Intialization of the topic
            RCLCPP_INFO(this->get_logger(), "New turtle node started");
        }

    private:
        void callback(){
            threads_.push_back(std::thread(std::bind(&NewTurtleNode::addNewTurtle, this)));
        }

        void addNewTurtle(){
            auto client_ = this->create_client<turtlesim::srv::Spawn>("spawn");

            while(!client_->wait_for_service(std::chrono::seconds(1))){
                RCLCPP_WARN(this->get_logger(), "Waiting for server to be up...");
            }

            auto request = std::make_shared<turtlesim::srv::Spawn::Request>();

            xCoor = (float)((rand() % xRange) + 0.1);
            yCoor = (float)((rand() % yRange) + 0.1);
            angle = (float)((rand() % angleRange));
            turtleName = "T" + std::to_string(turtleCounter);

            request->x = xCoor;
            request->y = yCoor;
            request->theta = angle;
            request->name = turtleName;

            auto future = client_->async_send_request(request);
            turtleCounter++;

            try{
                auto response = future.get();
                // RCLCPP_INFO(this->get_logger(), "turtle added");
            }
            catch(const std::exception& e){
                RCLCPP_ERROR(this->get_logger(), "Service call failed");
            }

            auto coordinates = my_robot_interfaces::msg::RobotLocation();
            coordinates.x = xCoor;
            coordinates.y = yCoor;
            coordinates.name = turtleName;
            publisher_ ->publish(coordinates);
        }

        rclcpp::Publisher<my_robot_interfaces::msg::RobotLocation>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        std::vector<std::thread> threads_;
        float xCoor, yCoor, angle;
        int xRange, yRange, angleRange;
        std::string turtleName;
        int turtleCounter;
};

int main(int argc, char **argv){

    rclcpp::init(argc, argv);
    auto node = std::make_shared<NewTurtleNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}