from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()

    robot_name = ["R1", "R2", "R3", "R4", "R5"]

    robot_news_station_name = []

    for name in robot_name:
        robot_news_station_name.append(Node(
            package="cpp_pkg",
            executable="robot_news_station",
            name="station_" + name,
            parameters=[{"robot_name":name}]
        ))

    smartphone_node = Node(
        package="cpp_pkg",
        executable="smartphone",
    )

    for node in robot_news_station_name:
        ld.add_action(node)
    ld.add_action(smartphone_node)
    return ld