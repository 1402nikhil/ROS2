//reset smallest distance when killing turtle
#include "rclcpp/rclcpp.hpp"
#include "my_robot_interfaces/msg/robot_location.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/kill.hpp"
#include <cmath>

class ControllerNode: public rclcpp::Node
{
    public:
        ControllerNode(): Node("controller_node"), smallestDistance(20.0), newDataFlag(false), smallDistFlag(false)
        {
            newTutrleSubscriber_ = this->create_subscription<my_robot_interfaces::msg::RobotLocation>("turtle_location", 10,
            bind(&ControllerNode::callbackRobotLocation, this, std::placeholders::_1));
            poseSubscriber_ = this->create_subscription<turtlesim::msg::Pose>("turtle1/pose", 10,
            bind(&ControllerNode::poseUpdate, this, std::placeholders::_1));
            velocityPublisher_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10); // number: topic name ; 10: queue size // Intialization of the topic
            timer_ = this->create_wall_timer(std::chrono::milliseconds(10), std::bind(&ControllerNode::moveTurtle, this));
            poseUpdateTimer_ = this->create_wall_timer(std::chrono::milliseconds(5), std::bind(&ControllerNode::callbackPoseUpdate, this));

            killFLag[0] = false;
            killFLag[1] = false;
        }

    private:
        void callbackRobotLocation(const my_robot_interfaces::msg::RobotLocation coordinate){
            coordinates.push_back(coordinate);
        }

        void callbackPoseUpdate(){
            if(newDataFlag){
                xCoor = latest_pose_.x;
                yCoor = latest_pose_.y;
                angle = latest_pose_.theta;
                newDataFlag = false;
            }
        }

        void poseUpdate(const turtlesim::msg::Pose::SharedPtr pose){
            latest_pose_ = *pose;
            newDataFlag = true;
        }

        void velocityValue(){
            auto velocity = geometry_msgs::msg::Twist();
            killFLag[0] = false;
            killFLag[1] = false;

            if(angleDiff > 0.017)
                velocity.angular.z = (angleDiff * 3);
            else if(angleDiff < -0.017)
                velocity.angular.z = (angleDiff * 3);
            else{
                velocity.angular.z = 0;
                killFLag[0] = true;
            }

            if(smallestDistance > 0.4)
                velocity.linear.x = 3;
            else{
                velocity.linear.x = 0;
                killFLag[1] = true;
            }

            velocityPublisher_ -> publish(velocity);
        }

        void killTurtle(){
            auto client_ = this->create_client<turtlesim::srv::Kill>("kill");

            while(!client_->wait_for_service(std::chrono::seconds(1))){
                RCLCPP_WARN(this->get_logger(), "Waiting for server to be up...");
            }

            auto request = std::make_shared<turtlesim::srv::Kill::Request>();

            request->name = closestTurtle;

            auto future = client_->async_send_request(request);

            try{
                auto response = future.get();

                RCLCPP_INFO(this->get_logger(), "turtle killed");
            }
            catch(const std::exception& e){
                RCLCPP_ERROR(this->get_logger(), "Service call failed");
            }
        }

        void moveTurtle(){
            if(!coordinates.empty()){
                turtleDistance.clear();
                // RCLCPP_INFO(this->get_logger(), "turtleDistance cleared");
                for(int i = 0; i < (int)coordinates.size(); i++){
                    float dist = sqrt(pow(coordinates[i].x - xCoor, 2) + pow(coordinates[i].y - yCoor, 2));   //distance between turtles
                    float ang = atan2((coordinates[i].y - yCoor), (coordinates[i].x - xCoor));       //angle between turtles
                    turtleDistance.push_back({dist, ang});
                }
                // RCLCPP_INFO(this->get_logger(), "turtleDistance calculated");
            }
            // else
                // RCLCPP_INFO(this->get_logger(), "No turtle available");

            if(!turtleDistance.empty()){
                smallestDistance = 20;
                // RCLCPP_INFO(this->get_logger(), "smallestDistance reset");
                for(int i = 0; i < (int)turtleDistance.size(); i++){
                    if(turtleDistance[i][0] < smallestDistance){
                        smallestDistance = turtleDistance[i][0];
                        angleDiff = turtleDistance[i][1] - angle;
                        smallDistFlag = true;
                        closestTurtle = coordinates[i].name;
                        nearestInteger = i;
                    }
                }
            }
            else{
                // RCLCPP_INFO(this->get_logger(), "Smallest distance cannot be find");
                smallestDistance = 0;
                angleDiff = 0;
            }

            if(smallDistFlag){
                if(angleDiff > M_PI)
                    angleDiff = angleDiff - 2 * M_PI;
                else if(angleDiff < -M_PI)
                    angleDiff = 2 * M_PI + angleDiff;
                
                velocityValue();
            }

            if(killFLag[0] == true && killFLag[1] == true){
                threads_.push_back(std::thread(std::bind(&ControllerNode::killTurtle, this)));
                coordinates.erase(coordinates.begin() + nearestInteger);
                killFLag[0] = false;
                killFLag[1] = false;
            }
        }

        rclcpp::Subscription<my_robot_interfaces::msg::RobotLocation>::SharedPtr newTutrleSubscriber_;
        rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr poseSubscriber_;
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocityPublisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::TimerBase::SharedPtr poseUpdateTimer_;
        std::vector<std::thread> threads_;
        std::vector<my_robot_interfaces::msg::RobotLocation> coordinates;
        std::vector<std::array<float, 2>> turtleDistance;
        turtlesim::msg::Pose latest_pose_; // Variable to store the latest pose
        float smallestDistance;
        float angleDiff = 0;
        int nearestInteger = 0;
        std::string closestTurtle;
        bool newDataFlag;
        bool smallDistFlag;
        bool killFLag[2];
        float xCoor, yCoor, angle;
};

int main(int argc, char **argv){

    rclcpp::init(argc, argv);
    auto node = std::make_shared<ControllerNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

/*
polarCoor.clear();
            for(int i = 0; i < (int)coordinates.size(); i++){                  //calculates distance from each turtle
                float dist = sqrt(pow((coordinates[i][1] - yCoor), 2) + pow((coordinates[i][0] - xCoor), 2));
                float ang = atan2(coordinates[i][1] - yCoor, coordinates[i][0] - xCoor);
                polarCoor.push_back({dist, ang});
                RCLCPP_INFO(this->get_logger(), "Polar: %f, %f", polarCoor[i][0], polarCoor[i][1]);
                RCLCPP_INFO(this->get_logger(), "Coordinates: %f, %f, %f", coordinates[i][0], coordinates[i][1], coordinates[i][2]);
            }

            if (polarCoor.empty()) {
                RCLCPP_WARN(this->get_logger(), "Distance vector is empty, skipping...");
                rclcpp::sleep_for(std::chrono::milliseconds(100));
            }
            else{
                smallestDistance = std::numeric_limits<float>::max(); // Reset to a very large value
                for(int i = 0; i < (int)polarCoor.size(); i++){     //selects the closest turtle
                    if(smallestDistance > polarCoor[i][0]){
                        smallestDistance = polarCoor[i][0];
                        angleDiff = coordinates[i][2] - polarCoor[i][1];       //angle difference between main turtle and closest turtle
                        RCLCPP_INFO(this->get_logger(), "%f distance", smallestDistance);
                    }
                }
            }

            if(angleDiff > M_PI){
                angleDiff = angleDiff - 2 * M_PI;
            }
            else if(angleDiff < -M_PI){
                angleDiff = 2 * M_PI - angleDiff;
            }

            RCLCPP_INFO(this->get_logger(), "Angle difference: %f", angleDiff);

            if(smallestDistance > 0 || angleDiff > 0){
                    velocityValue(1);
            }
            else{
                velocityValue(0);
            }
            */