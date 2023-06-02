//
// Created by deslobodzian on 5/31/23.
//

#pragma once

class Subsystem {
public:
    virtual ~Subsystem() = default;

    virtual void initialize() = 0;
    virtual void run() = 0;
    virtual void cleanup() = 0;
};