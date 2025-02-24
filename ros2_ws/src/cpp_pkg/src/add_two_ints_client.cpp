/* The future.get() function stops the code till the response is not received. If the code doesn't run, the node cannot spin which is
 needed for the result from the future. thus the function is is called and ran from a different thread.*/

#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

class AddTwoIntsClientNode : public rclcpp::Node
{
public:
    AddTwoIntsClientNode() : Node("add_two_ints_client")
    {
        thread1_ = std::thread(std::bind(&AddTwoIntsClientNode::callAddTwoIntsService, this, 1, 4));
        threads_.push_back(std::thread(std::bind(&AddTwoIntsClientNode::callAddTwoIntsService, this, 2, 9)));
        threads_.push_back(std::thread(std::bind(&AddTwoIntsClientNode::callAddTwoIntsService, this, 55, 69)));
    }

    void callAddTwoIntsService(int a, int b) // function to call the service
    {
        auto client = this->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints"); // add_twom_ints: services node name
        while (!client->wait_for_service(std::chrono::seconds(1)))                              // wait for the server to be started and check for it every 1 second
        {
            RCLCPP_WARN(this->get_logger(), "Waiting for the server to be up...");
        }

        auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>(); // shared pointer of the request object
        request->a = a;
        request->b = b;

        auto future = client->async_send_request(request);     //send asynchronous request from the client

        try{
            auto response = future.get();         //waits until the response is received and also throws an exception incase of errors
            RCLCPP_INFO(this->get_logger(), "%d + %d = %td", a, b, response->sum);
        }
        catch(const std::exception& e){               //catches the exception
            RCLCPP_ERROR(this->get_logger(), "Service call failed");
        }
    }

private:
    std::thread thread1_;
    std::vector<std::thread> threads_;
};

int main(int argc, char **argv)
{

    rclcpp::init(argc, argv);
    auto node = std::make_shared<AddTwoIntsClientNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}