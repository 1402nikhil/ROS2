from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()

    turtlesim_node = Node(
        package="turtlesim",
        executable="turtlesim_node"
    )
    turtle_controller_node = Node(
        package="course_activities",
        executable="controller"
    )

    add_new_turtle_node = Node(
        package="course_activities",
        executable="new_turtle"
    )

    ld.add_action(turtlesim_node)
    ld.add_action(turtle_controller_node)
    ld.add_action(add_new_turtle_node)

    return ld