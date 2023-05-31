//
// Created by prometheus on 4/26/23.
//
#pragma once

#include <ctre/phoenixpro/TalonFX.hpp>

using namespace ctre::phoenixpro;

typedef struct MotorConfig {
    units::second_t timeout = 0.1_s;

    // motor configs
    signals::InvertedValue inverted_value;
    signals::NeutralModeValue neutral_mode_value;
    bool use_foc = false;

    // current/torque config
    units::ampere_t max_current = 60.0_A;
    units::ampere_t current_deadband = 0_A;

    // voltage config
    units::volt_t max_voltage = 12.0_V;
    units::volt_t voltage_time_constant = 0_V;

    units::ampere_t max_stator_current = 60.0_A;
    units::ampere_t max_supply_current = 60.0_A;
    bool enable_stator_current_limit = false;
    bool enable_supply_current_limit = false;

    signals::FeedbackSensorSourceValue feedback_sensor_source_value = signals::FeedbackSensorSourceValue::RotorSensor;
    double sensor_to_mechanism_ratio = 1.0;

} MotorConfig;

typedef struct ClosedLoopConfig {
    double kP = 0.0;
    double kI = 0.0;
    double kD = 0.0;
    double kF = 0.0;

    double cruise_velocity = 0.0;
    double acceleration = 0.0;
    double jerk = 0.0;

} ClosedLoopConfig;

class OutliersTalon : public hardware::TalonFX {
public:
    OutliersTalon(const uint8_t &port, const std::string &can_bus) : TalonFX{port, can_bus}{
    }

    void configure(const MotorConfig &config) {
        motor_configs_.Inverted = config.inverted_value;
        motor_configs_.NeutralMode = config.neutral_mode_value;

        current_limits_configs_.StatorCurrentLimit = config.max_stator_current.value();
        current_limits_configs_.SupplyCurrentLimit = config.max_supply_current.value();
        current_limits_configs_.StatorCurrentLimitEnable = config.enable_stator_current_limit;
        current_limits_configs_.SupplyCurrentLimitEnable = config.enable_supply_current_limit;

        torque_current_configs_.PeakForwardTorqueCurrent = config.max_current.value();
        torque_current_configs_.PeakReverseTorqueCurrent = -config.max_current.value();
        torque_current_configs_.TorqueNeutralDeadband = config.current_deadband.value();

        voltage_configs_.PeakForwardVoltage = config.max_voltage.value();
        voltage_configs_.PeakReverseVoltage = -config.max_voltage.value();
        voltage_configs_.SupplyVoltageTimeConstant = config.voltage_time_constant.value();

        feedback_configs_.FeedbackSensorSource = config.feedback_sensor_source_value;
        feedback_configs_.SensorToMechanismRatio = config.sensor_to_mechanism_ratio;

        voltage_out_.EnableFOC = config.use_foc;
        motion_magic_voltage_.EnableFOC = config.use_foc;
        percent_out_.EnableFOC = config.use_foc;
        velocity_voltage_.EnableFOC = config.use_foc;
        torque_current_foc_.Deadband = config.current_deadband;

        configuration_.MotorOutput = motor_configs_;
        configuration_.TorqueCurrent = torque_current_configs_;
        configuration_.CurrentLimits = current_limits_configs_;
        configuration_.Feedback = feedback_configs_;

        this->GetConfigurator().Apply(configuration_);
    }

    void configure_closed_loop(const ClosedLoopConfig &config) {
        slot0_configs_.kV = config.kF;
        slot0_configs_.kP = config.kP;
        slot0_configs_.kI = config.kI;
        slot0_configs_.kD = config.kD;

        motion_magic_configs_.MotionMagicCruiseVelocity = config.cruise_velocity;
        motion_magic_configs_.MotionMagicAcceleration = config.acceleration;
        motion_magic_configs_.MotionMagicJerk = config.jerk;

        configuration_.Slot0 = slot0_configs_;
        configuration_.MotionMagic = motion_magic_configs_;

        this->GetConfigurator().Apply(configuration_);
    }
    void set_percent_output(double output) {
        if (percent_out_.Output != output) {
            SetControl(percent_out_.WithOutput(output));
        }
    }

    void set_voltage(units::volt_t voltage) {
        if (voltage_out_.Output != voltage) {
            SetControl(voltage_out_.WithOutput(voltage));
        }
    }

    void set_motion_magic(units::angle::turn_t position) {
        if (motion_magic_voltage_.Position != position) {
            SetControl(motion_magic_voltage_.WithPosition(position).WithSlot(0));
        }
    }

    void set_velocity(units::turns_per_second_t rps) {
        if (velocity_voltage_.Velocity != rps) {
            SetControl(velocity_voltage_.WithVelocity(rps));
        }
    }

    void set_torque_current_foc_rate(units::hertz_t hz) {
        torque_current_foc_.WithUpdateFreqHz(hz);
    }

    void set_torque_current_foc(units::ampere_t current) {
        if (torque_current_foc_.Output != current) {
            SetControl(torque_current_foc_.WithOutput(current));
        }
    }
private:
    // Configs
    configs::TalonFXConfiguration configuration_{};
    configs::CurrentLimitsConfigs current_limits_configs_{};
    configs::VoltageConfigs voltage_configs_{};
    configs::FeedbackConfigs feedback_configs_{};
    configs::Slot0Configs slot0_configs_{};
    configs::MotorOutputConfigs motor_configs_{};
    configs::TorqueCurrentConfigs torque_current_configs_{};
    configs::MotionMagicConfigs motion_magic_configs_{};

    // Control
    controls::DutyCycleOut percent_out_{0.0};
    controls::TorqueCurrentFOC torque_current_foc_{0_A};
    controls::VoltageOut voltage_out_{0_V};
    controls::MotionMagicVoltage motion_magic_voltage_{0_tr};
    controls::VelocityVoltage velocity_voltage_{0_rad_per_s};
};

