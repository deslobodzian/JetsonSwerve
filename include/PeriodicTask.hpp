//
// Created by prometheus on 5/30/23.
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
    PeriodicTask(PeriodicTaskManager* taskManager, float period, std::string name);
    virtual ~PeriodicTask();

    void start();
    void stop();
    bool isLoopOverrun() const;
    void printStatus() const;
    void clearMax();

    virtual void init() = 0;
    virtual void run() = 0;
    virtual void cleanup() = 0;

    const std::string& getName() const;
    float getPeriod() const;
    float getRuntime() const;
    float getMaxPeriod() const;
    float getMaxRuntime() const;

private:
    void loopFunction();

    std::chrono::duration<float> _period;
    std::string _name;
    std::atomic<bool> _running;
    std::thread _thread;
    float _lastRuntime = 0;
    float _lastPeriodTime = 0;
    float _maxPeriod = 0;
    float _maxRuntime = 0;
};
class PeriodicTaskManager {
public:
    PeriodicTaskManager() = default;
    ~PeriodicTaskManager();

    void addTask(PeriodicTask* task);
    void stopAll();
    void printStatus();

private:
    std::vector<std::unique_ptr<PeriodicTask>> _tasks;
};

class PeriodicFunction : public PeriodicTask {
public:
    PeriodicFunction(PeriodicTaskManager* taskManager, float period,
                     std::string name, std::function<void()> function)
            : PeriodicTask(taskManager, period, name), _function(function) {}

    void cleanup() override {}
    void init() override {}
    void run() override { _function(); }

private:
    std::function<void()> _function;
};

template <typename T>
class PeriodicMemberFunction : public PeriodicTask {
public:
    PeriodicMemberFunction(PeriodicTaskManager* taskManager, float period,
                           std::string name, std::function<void(T*)> function, T* obj)
            : PeriodicTask(taskManager, period, name),
              _function(function),
              _obj(obj) {}

    void cleanup() override {}
    void init() override {}
    void run() override { _function(_obj); }

private:
    std::function<void(T*)> _function;
    T* _obj;
};