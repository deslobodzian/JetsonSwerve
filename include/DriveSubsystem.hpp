//
// Created by prometheus on 5/31/23.
//

#pragma once
#include <Subsystem.hpp>

class DriveSubsystem : public Subsystem {
public:
    DriveSubsystem();

    void initialize() override;

    void run() override;

    void cleanup() override;
};
