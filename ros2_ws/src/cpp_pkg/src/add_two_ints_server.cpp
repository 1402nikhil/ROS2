#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

class addTwoIntsServerNode: public rclcpp::Node
{
    public:
        addTwoIntsServerNode(): Node("add_two_ints_server")
        {
            server_ = this->create_service<example_interfaces::srv::AddTwoInts>(    //add_two_ints: Service name, another argument: callback function
                "add_two_ints", 
                std::bind(&addTwoIntsServerNode::callbackAddTwoInts, this, _1, _2));
            RCLCPP_INFO(this->get_logger(), "Service server has been started");
        }

    private:

        void callbackAddTwoInts(const example_interfaces::srv::AddTwoInts::Request::SharedPtr request,
        const example_interfaces::srv::AddTwoInts::Response::SharedPtr response){
            response->sum = request->a + request->b;         //no return type, just fill the response->sum
            RCLCPP_INFO(this->get_logger(), "%d + %d = %d", (int)request->a, (int)request->b, (int)response->sum);
        }

        rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr server_;  //Server declaration
};

int main(int argc, char **argv){

    rclcpp::init(argc, argv);
    auto node = std::make_shared<addTwoIntsServerNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}