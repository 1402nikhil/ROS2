from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()

    remap_number_topic = ("number", "my_number")

    number_publisher_node = Node(          #creating node with the Node constructor
        package="course_activities",       #package name
        executable="number_publisher",     #node name
        name="my_number_publisher",        #rename the node
        remappings=[                       #remap the topic    array of tupple
            remap_number_topic,
        ],
        parameters=[                       #setting parameters  array of dictionary
            {"number_to_publish": 4},
            {"publish_frequency": 2.0}
        ]
    )

    number_counter_node = Node(
        package="course_activities",
        executable="number_counter",
        name="my_number_counter",
        remappings=[
            remap_number_topic,
            ("number_count", "my_number_count")
        ]
    )

    ld.add_action(number_publisher_node)
    ld.add_action(number_counter_node)

    return ld
