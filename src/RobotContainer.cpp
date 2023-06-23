//
// Created by prometheus on 5/31/23.
//

#include <RobotContainer.hpp>

RobotContainer::RobotContainer() {
    subsystems_.push_back(&drive_train_);
}
RobotContainer::~RobotContainer() {}


void RobotContainer::run_spi_board() {
    spi_sensor_board_.update_spi();
}

void RobotContainer::initialize_robot() {
    // start spi task at 500 micro seconds
    PeriodicMemberFunction<RobotContainer> spi_task(
        &task_manager_,
        0.0005,
        "spi_board",
        &RobotContainer::run_spi_board, 
        this
    );

    spi_task.start();

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

