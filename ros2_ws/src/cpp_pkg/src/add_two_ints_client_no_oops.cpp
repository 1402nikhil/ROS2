#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

int main(int argc, char **argv){

    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("add_two_ints_client_no_oop");
    
    auto client = node->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints");  //add_twom_ints: services node name
    while(!client->wait_for_service(std::chrono::seconds(1))){
        RCLCPP_WARN(node->get_logger(), "Waiting for server...");
    }

    auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
    request->a = 3;
    request->b = 8;

    auto future = client->async_send_request(request);     //sends asynchronous request
    if(rclcpp::spin_until_future_complete(node, future) == rclcpp::FutureReturnCode::SUCCESS){        //spins(waits) untill response is received
        RCLCPP_INFO(node->get_logger(), "%d + %d = %d", (int)request->a, (int)request->b, (int)future.get()->sum);
    }
    else{
        RCLCPP_ERROR(node->get_logger(), "Error while calling service");
    }

    rclcpp::shutdown();
    return 0;
}