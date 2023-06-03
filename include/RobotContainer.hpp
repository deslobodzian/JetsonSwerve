//
// Created by prometheus on 5/31/23.
//

#pragma once

#include <RobotBase.hpp>
#include <Subsystem.hpp>
#include <PeriodicTask.hpp>
#include <vector>
#include <DriveSubsystem.hpp>

class RobotContainer {
    friend class RobotRunner;
public:
    RobotContainer();
    ~RobotContainer();
    void initialize_robot();
    void run_robot();
    void cleanup_robot();

private:
    PeriodicTaskManager task_manager_;
    DriveSubsystem drive_train_;
    std::vector<Subsystem*> subsystems_;
};