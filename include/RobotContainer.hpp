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
public:
    RobotContainer();
    void initialize_subsystems();
    void run_subsystems();
    void cleanup_subsystems();

private:
    PeriodicTaskManager task_manager_;
    DriveSubsystem drive_train_;
    std::vector<Subsystem*> subsystems_;
};