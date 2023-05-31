//
// Created by deslobodzian on 5/30/23.
//
#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <sys/timerfd.h>
#include <unistd.h>

class PeriodicTaskManager;

class PeriodicTask {
public:
    PeriodicTask(PeriodicTaskManager* task_manager, float period, std::string name);
    virtual ~PeriodicTask();

    void start();
    void stop();
    bool is_loop_overrun() const;
    void print_status() const;
    void clear_max();

    virtual void init() = 0;
    virtual void run() = 0;
    virtual void cleanup() = 0;

    const std::string& get_name() const;
    float get_period() const;
    float get_runtime() const;
    float get_max_period() const;
    float get_max_runtime() const;

private:
    void loop_function();

    std::chrono::duration<float> period_;
    std::string name_;
    std::atomic<bool> running_;
    std::thread thread_;
    float last_runtime_ = 0;
    float last_period_time_ = 0;
    float max_period_ = 0;
    float max_runtime_ = 0;
};

class PeriodicTaskManager {
public:
    PeriodicTaskManager() = default;
    ~PeriodicTaskManager();

    void add_task(PeriodicTask* task);
    void stop_all();
    void print_status();

private:
    std::vector<std::unique_ptr<PeriodicTask>> tasks_;
};

class PeriodicFunction : public PeriodicTask {
public:
    PeriodicFunction(PeriodicTaskManager* task_manager, float period,
                     std::string name, std::function<void()> function)
            : PeriodicTask(task_manager, period, name), function_(function) {}

    void cleanup() override {}
    void init() override {}
    void run() override { function_(); }

private:
    std::function<void()> function_;
};

template <typename T>
class PeriodicMemberFunction : public PeriodicTask {
public:
    PeriodicMemberFunction(PeriodicTaskManager* task_manager, float period,
                           std::string name, std::function<void(T*)> function, T* obj)
            : PeriodicTask(task_manager, period, name),
              function_(function),
              obj_(obj) {}

    void cleanup() override {}
    void init() override {}
    void run() override { function_(obj_); }

private:
    std::function<void(T*)> function_;
    T* obj_;
};