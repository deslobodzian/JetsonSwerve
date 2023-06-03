//
// Created by prometheus on 5/31/23.
//

#include <RobotContainer.hpp>

RobotContainer::RobotContainer() {
    subsystems_.push_back(&drive_train_);
}
RobotContainer::~RobotContainer() {}

void RobotContainer::initialize_robot() {
    for (const auto& subsystem : subsystems_) {
        subsystem->initialize();
    }
}

void RobotContainer::run_robot() {
    for (const auto& subsystem : subsystems_) {
        subsystem->run();
    }
}

void RobotContainer::cleanup_robot() {
    for (const auto& subsystem : subsystems_) {
        subsystem->cleanup();
    }
}

