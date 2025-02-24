#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/set_bool.hpp"

class RessetCounterClientNode : public rclcpp::Node
{
public:
    RessetCounterClientNode() : Node("reset_counter_client")
    {
        thread1_ = std::thread(std::bind(&RessetCounterClientNode::callResetCounterService, this, true));
    }

    void callResetCounterService(bool data)
    {
        auto client = this->create_client<example_interfaces::srv::SetBool>("reset_counter"); // add_twom_ints: services node name
        while (!client->wait_for_service(std::chrono::seconds(1)))                            // wait for the server to be started and check for it every 1 second
        {
            RCLCPP_WARN(this->get_logger(), "Waiting for the server to be up...");
        }

        auto request = std::make_shared<example_interfaces::srv::SetBool::Request>(); // shared pointer of the request object
        request->data = data;

        auto future = client->async_send_request(request);     //send asynchronous request from the client

        try{
            auto response = future.get();         //waits until the response is received and also throws an exception incase of errors
            RCLCPP_INFO(this->get_logger(), "Response from server: %d", response->success);
        }
        catch(const std::exception& e){               //catches the exception
            RCLCPP_ERROR(this->get_logger(), "Service call failed");
        }
    }

private:
    std::thread thread1_;

};

int main(int argc, char **argv)
{

    rclcpp::init(argc, argv);
    auto node = std::make_shared<RessetCounterClientNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}