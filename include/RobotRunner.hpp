//
// Created by prometheus on 5/31/23.
//
#pragma once

#include <RobotContainer.hpp>
#include <RobotBase.hpp>
#include <PeriodicTask.hpp>

class RobotRunner : public RobotBase {
public:
    RobotRunner(std::unique_ptr<RobotContainer> robot_container, PeriodicTaskManager* task_manager, float period, std::string name);
    void init() override;
    void run() override;
    void cleanup() override;

    void robot_init() override;
    void robot_periodic() override;
    bool is_enabled() override;
    void enable_init() override;
    void enable_periodic() override;
    void disable_init() override;
    void disable_periodic() override;

private:
    std::unique_ptr<RobotContainer> robot_container_;
};
