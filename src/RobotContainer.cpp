//
// Created by prometheus on 5/31/23.
//

#include <RobotContainer.hpp>

RobotContainer::RobotContainer() {
    subsystems_.push_back(&drive_train_);
}

void RobotContainer::initialize_subsystems() {
    for (const auto& subsystem : subsystems_) {
        subsystem->initialize();
    }
}

void RobotContainer::run_subsystems() {
    for (const auto& subsystem : subsystems_) {
        subsystem->run();
    }
}

void RobotContainer::cleanup_subsystems() {
    for (const auto& subsystem : subsystems_) {
        subsystem->cleanup();
    }
}

