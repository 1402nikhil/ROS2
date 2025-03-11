import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/nikhil/Work/ROS/ros2_ws/install/py_pkg'
