#include "three_omniwheel_controller/odometry.hpp"

namespace three_omniwheel_controller
{
    Odometry::Odometry(size_t velocity_rolling_window_size)
        : timestamp_(0.0),
          x_(0.0),
          y_(0.0),
          heading_(0.0),
          linear_(0.0),
          angular_(0.0),
          x_wheel_radius_(0.0),
          y_wheel_radius_(0.0),
          x_wheel_old_pos_(0.0),
          y_wheel_old_pos_(0.0),
          velocity_rolling_window_size_(velocity_rolling_window_size),
          linear_accumulator_(velocity_rolling_window_size),
          angular_accumulator_(velocity_rolling_window_size)
    {
    }

    void Odometry::init(const rclcpp::Time &time)
    {
        // Reset accumulators and timestamp:
        resetAccumulators();
        timestamp_ = time;
    }

    bool Odometry::update(double x_pos, double y_pos, double ang_pos, const rclcpp::Time &time)
    {
        //x_pos, y_pos are angular displacement in radians
        // We cannot estimate the speed with very small time intervals:
        const double dt = time.seconds() - timestamp_.seconds();
        if (dt < 0.0001)
        {
            return false; // Interval too small to integrate with
        }

        // Get current wheel joint positions:
        const double x_wheel_cur_pos = x_pos * x_wheel_radius_;
        const double y_wheel_cur_pos = y_pos * y_wheel_radius_;
        const double ang_cur_pos = ang_pos;

        // Estimate velocity of wheels using old and current position:
        const double x_wheel_est_vel = (x_wheel_cur_pos - x_wheel_old_pos_);
        const double y_wheel_est_vel = (y_wheel_cur_pos - y_wheel_old_pos_);
        const double ang_est_vel = (ang_cur_pos - ang_old_pos_) / dt;

        // Update old position with current:
        x_wheel_old_pos_ = x_wheel_cur_pos;
        y_wheel_old_pos_ = y_wheel_cur_pos;
        ang_old_pos_ = ang_cur_pos;

        updateFromVelocity(x_wheel_est_vel, y_wheel_est_vel, ang_est_vel, time);

        return true;
    }

    bool Odometry::updateFromVelocity(double x_vel, double y_vel, double ang_vel, const rclcpp::Time &time)
    {
        const double dt = time.seconds() - timestamp_.seconds();
        if (dt < 0.0001)
        {
            return false; // Interval too small to integrate with
        }
        // Compute linear and angular diff:
        const double linear = sqrt((x_vel * x_vel) + (y_vel * y_vel));
        // Now there is a bug about scout angular velocity
        const double angular = ang_vel;

        // Integrate odometry:
        integrateExact(linear, angular);

        timestamp_ = time;

        // Estimate speeds using a rolling mean to filter them out:
        linear_accumulator_.accumulate(linear / dt);
        angular_accumulator_.accumulate(angular / dt);

        linear_ = linear_accumulator_.getRollingMean();
        angular_ = angular_accumulator_.getRollingMean();

        return true;
    }

    void Odometry::updateOpenLoop(double linear, double angular, const rclcpp::Time &time)
    {
        /// Save last linear and angular velocity:
        linear_ = linear;
        angular_ = angular;

        /// Integrate odometry:
        const double dt = time.seconds() - timestamp_.seconds();
        timestamp_ = time;
        integrateExact(linear * dt, angular * dt);
    }

    void Odometry::resetOdometry()
    {
        x_ = 0.0;
        y_ = 0.0;
        heading_ = 0.0;
    }

    void Odometry::setWheelParams(        //wheel parameter of dead wheel
        double x_wheel_radius, double y_wheel_radius)
    {
        x_wheel_radius_ = x_wheel_radius;
        y_wheel_radius_ = y_wheel_radius;
    }

    void Odometry::setVelocityRollingWindowSize(size_t velocity_rolling_window_size)
    {
        velocity_rolling_window_size_ = velocity_rolling_window_size;

        resetAccumulators();
    }

    void Odometry::integrateRungeKutta2(double linear, double angular)
    {
        const double direction = heading_ + angular * 0.5;

        /// Runge-Kutta 2nd order integration:
        x_ += linear * cos(direction);
        y_ += linear * sin(direction);
        heading_ += angular;
    }

    void Odometry::integrateExact(double linear, double angular)
    {
        if (fabs(angular) < 1e-6)
        {
            integrateRungeKutta2(linear, angular);
        }
        else
        {
            /// Exact integration (should solve problems when angular is zero):
            const double heading_old = heading_;
            const double r = linear / angular;
            heading_ += angular;
            x_ += r * (sin(heading_) - sin(heading_old));
            y_ += -r * (cos(heading_) - cos(heading_old));
        }
    }

    void Odometry::resetAccumulators()
    {
        linear_accumulator_ = RollingMeanAccumulator(velocity_rolling_window_size_);
        angular_accumulator_ = RollingMeanAccumulator(velocity_rolling_window_size_);
    }
}