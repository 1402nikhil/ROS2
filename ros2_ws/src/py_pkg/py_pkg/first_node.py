#!/bin/python3.10 python3
import rclpy
from rclpy.node import Node

def main(args=None):
    rclpy.init(args = args)       #initialize ros2 communication
    node = Node("py_test")
    node.get_logger().info("Hello ROS")
    rclpy.spin(node)       #pauses the program here and helps the node to be alive, it can behelpfull when the node has callbacks, they will be executed from the spin function, it basically don't let the program end
    rclpy.shutdown()           #shutdown the ros2 communication

if __name__ == "__main__":
    main()


print