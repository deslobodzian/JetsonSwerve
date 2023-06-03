//
// Created by prometheus on 6/2/23.
//
#pragma once


class State {
public:
    virtual ~State();

    virtual void on_enter();
    virtual void run();
    virtual void on_exit();
};