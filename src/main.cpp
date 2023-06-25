//
// Created by deslobodzian on 4/26/23.
//

#include <iostream>
#include <RobotContainer.hpp>
#include <RobotRunner.hpp>

int main() {
    info("Starting Robot");
    PeriodicTaskManager taskManager;
    std::unique_ptr<RobotContainer> robotContainer = std::make_unique<RobotContainer>();
    float period = 0.005; // update every 20 milliseconds
    std::string name = "RobotRunner";
    RobotRunner runner(std::move(robotContainer), &taskManager, period, std::move(name));
    runner.init();
    return 0;
}