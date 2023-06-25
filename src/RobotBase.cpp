//
// Created by deslobodzian on 5/31/23.
//

#include <RobotBase.hpp>
#include <utils.hpp>

RobotBase::RobotBase(PeriodicTaskManager *task_manager, float period, std::string name)
    : PeriodicTask(task_manager, period, std::move(name)), state_(RobotState::DISABLED) {
        debug("RobotBase Constructor");
    }

void RobotBase::init() {
    if (state_ == RobotState::DISABLED) {
        enable_init();
    }
}

void RobotBase::run() {

}

void RobotBase::cleanup() {

}

void RobotBase::enable_init() {

}
void RobotBase::enable_periodic() {

}
void RobotBase::disable_init() {

}
void RobotBase::disable_periodic() {

}

bool RobotBase::is_running() {
    return true;
}

void RobotBase::set_robot_state(const RobotState& new_state) {
    if (state_ != new_state) {
        state_ = new_state;
    }
}

RobotState RobotBase::get_robot_state() {
    return state_;
}

void RobotBase::execute_robot_state() {
    switch (state_) {
        case RobotState::DISABLED:
            disable_periodic();
            break;
        case RobotState::AUTONOMOUS:
            break;
        case RobotState::TELEOPERATION:
            break;
        case RobotState::TEST:
            break;
    }
}

