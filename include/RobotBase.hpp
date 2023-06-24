//
// Created by deslobodzian on 5/31/23.
//

#pragma once

#include <PeriodicTask.hpp>
#include <units/time.h>

enum class RobotState {
   DISABLED,
   AUTONOMOUS,
   TELEOPERATION,
   TEST
};

class RobotBase : public PeriodicTask {
public:
    RobotBase(PeriodicTaskManager* task_manager, float period, std::string name);

    void init() override;
    void run() override;
    void cleanup() override;

    virtual void robot_init() = 0;
    virtual void robot_periodic() = 0;

    virtual bool is_enabled() = 0;
    virtual void enable_init() = 0;
    virtual void enable_periodic() = 0;

    virtual void disable_init() = 0;
    virtual void disable_periodic() = 0;

    virtual bool is_running();

    void set_robot_state(const RobotState& new_state);
    RobotState get_robot_state();

    void execute_robot_state();

private:
    RobotState state_;
};