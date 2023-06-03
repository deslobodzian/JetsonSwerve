//
// Created by deslobodzian on 6/2/23.
//
#pragma once

/* This class is for interfacing between the hardware and software */

#include <RobotContainer.hpp>


class Robot {

public:
    Robot(std::unique_ptr<RobotContainer> robot_container);
    ~Robot();
private:
    std::unique_ptr<RobotContainer> robot_container_;
};