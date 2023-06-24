//
// Created by prometheus on 5/31/23.
//
#include "RobotRunner.hpp"

RobotRunner::RobotRunner(std::unique_ptr<RobotContainer> robot_container, PeriodicTaskManager* task_manager, float period, std::string name)
    : RobotBase(task_manager, period, std::move(name)), robot_container_(std::move(robot_container)) {}

void RobotRunner::init() {
    RobotBase::init();
    robot_container_->initialize_robot();
}

void RobotRunner::run() {
    RobotBase::run();
    robot_container_->run_robot();
}

void RobotRunner::cleanup() {
    RobotBase::cleanup();
    robot_container_->cleanup_robot();
}

void RobotRunner::robot_init() {

}

void RobotRunner::robot_periodic() {
    // Code to perform periodic actions for the RobotRunner goes here
    // This would typically involve updating sensors, executing tasks, etc.
}

bool RobotRunner::is_enabled() {
    // This would typically return a boolean indicating if the robot is enabled or not
    // The exact implementation will depend on your robot system
    return true;
}

void RobotRunner::enable_init() {
    RobotBase::enable_init();
}

void RobotRunner::enable_periodic() {
    RobotBase::enable_periodic();
    robot_periodic();
}

void RobotRunner::disable_init() {
    RobotBase::disable_init();
    // Code to initialize the robot for the disabled state goes here
}

void RobotRunner::disable_periodic() {
    RobotBase::disable_periodic();
    // Code to perform periodic actions while the robot is in the disabled state goes here
}