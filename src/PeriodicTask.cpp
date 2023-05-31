//
// Created by prometheus on 5/30/23.
//

#include <PeriodicTask.hpp>
#include <iostream>

PeriodicTask::PeriodicTask(PeriodicTaskManager* taskManager, float period, std::string name)
        : _period(std::chrono::duration<float>(period)), _name(std::move(name)) {
    taskManager->addTask(this);
}
PeriodicTask::~PeriodicTask() {
    stop();
}

void PeriodicTask::start() {
    if (_running) {
        std::cout << "[PeriodicTask] Tried to start " << _name << " but it was already running!\n";
        return;
    }
    init();
    _running = true;
    _thread = std::thread(&PeriodicTask::loopFunction, this);
}

void PeriodicTask::stop() {
    if (!_running) {
        std::cout << "[PeriodicTask] Tried to stop " << _name << " but it wasn't running!\n";
        return;
    }
    _running = false;
    std::cout << "[PeriodicTask] Waiting for " << _name << " to stop...\n";
    _thread.join();
    std::cout << "[PeriodicTask] Done!\n";
    cleanup();
}

bool PeriodicTask::isLoopOverrun() const {
    if (_maxPeriod > _period.count() * 1.3f || _maxRuntime > _period.count()) {
        printf("[PeriodicTask] Warning: Task %s is running slow!\n", _name.c_str());
        return true;
    }
    return false;
}

void PeriodicTask::printStatus() const {
    if (!_running) return;
    if (isLoopOverrun()) {
        printf("|%-20s|%6.4f|%6.4f|%6.4f|%6.4f|%6.4f\n",
                     _name.c_str(), _lastRuntime, _maxRuntime, _period.count(),
                     _lastPeriodTime, _maxPeriod);
    } else {
        printf("|%-20s|%6.4f|%6.4f|%6.4f|%6.4f|%6.4f\n", _name.c_str(),
               _lastRuntime, _maxRuntime, _period.count(), _lastPeriodTime, _maxPeriod);
    }
}

void PeriodicTask::clearMax() {
    _maxPeriod = 0;
    _maxRuntime = 0;
}

void PeriodicTask::loopFunction() {
    auto timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);

    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(_period);
    auto nano_seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(_period - seconds);

    itimerspec timer_spec{};
    timer_spec.it_interval.tv_sec = seconds.count();
    timer_spec.it_value.tv_sec = seconds.count();

    timer_spec.it_interval.tv_nsec = nano_seconds.count();
    timer_spec.it_value.tv_nsec = nano_seconds.count();

    timerfd_settime(timer_fd, 0, &timer_spec, nullptr);

    unsigned long long missed = 0;

    auto last_run_time = std::chrono::high_resolution_clock::now();

    while (_running) {
        auto start = std::chrono::high_resolution_clock::now();
        run();
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> runtime = end - start;
        _lastRuntime = runtime.count();

        std::chrono::duration<float> period = start - last_run_time;
        _lastPeriodTime = period.count();

        last_run_time = start;

        ssize_t m = read(timer_fd, &missed, sizeof(missed));
        (void)m;
        isLoopOverrun();

        std::cout << "Task " << _name << " period: " << _lastPeriodTime << " seconds\n";
    }
}

const std::string& PeriodicTask::getName() const {
    return _name;
}


PeriodicTaskManager::~PeriodicTaskManager() {
    stopAll();
}

void PeriodicTaskManager::stopAll() {
    for (auto &task : _tasks) {
        task->stop();
    }
}

void PeriodicTaskManager::addTask(PeriodicTask* task) {
    _tasks.push_back(std::unique_ptr<PeriodicTask>(task));
}

void PeriodicTaskManager::printStatus() {
    for (auto& task : _tasks) {
        task->printStatus();
        task->clearMax();
    }
}

