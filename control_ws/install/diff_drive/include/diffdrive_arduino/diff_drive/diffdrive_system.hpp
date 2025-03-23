#ifndef DIFF_DRIVE__DIFFDRIVE_SYSTEM_HPP_
#define DIFF_DRIVE__DIFFDRIVE_SYSTEM_HPP_

#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "rclcpp/clock.hpp"
#include "rclcpp/duration.hpp"
#include "rclcpp/macros.hpp"
#include "rclcpp/time.hpp"
#include "rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp"
#include "rclcpp_lifecycle/state.hpp"

#include "/home/nikhil/Work/ROS/control_ws/src/diff_drive/hardware/include/diff_drive/visibility_control.h"

namespace diff_drive
{
    class DiffDriveHardware: public hardware_interface::SystemInterface
    {
        public:
            RCLCPP_SHARED_PTR_DEFINITIONS(DiffDriveHardware);

            DIFF_DRIVE_PUBLIC
            hardware_interface::CallbackReturn on_init();
    };
}

#endif