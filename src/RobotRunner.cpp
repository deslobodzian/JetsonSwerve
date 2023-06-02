//
// Created by prometheus on 5/31/23.
//
#include "RobotRunner.hpp"

RobotRunner::RobotRunner(
        std::unique_ptr<RobotContainer> robot_container,
        PeriodicTaskManager *task_manager,
        float period,
        std::string name)
        : RobotBase(task_manager, period, std::move(name)),
        robot_container_(std::move(robot_container)) {}

void RobotRunner::init() {}
void RobotRunner::run() {}
void RobotRunner::cleanup() {}