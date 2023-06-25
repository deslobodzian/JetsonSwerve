//
// Created by prometheus on 5/30/23.
//

#include <PeriodicTask.hpp>
#include <iostream>
#include <utils.hpp>

PeriodicTask::PeriodicTask(PeriodicTaskManager* task_manager, float period, std::string name)
        : period_(std::chrono::duration<float>(period)), name_(std::move(name)) {
    task_manager->add_task(this);
}

PeriodicTask::~PeriodicTask() {
    debug("A task was decontructed");
    stop();
}

void PeriodicTask::start() {
    if (running_) {
        std::cout << "[PeriodicTask] Tried to start " << name_ << " but it was already running!\n";
        return;
    }
    init();
    running_ = true;
    thread_ = std::thread(&PeriodicTask::loop_function, this);
}

void PeriodicTask::stop() {
    if (!running_) {
        std::cout << "[PeriodicTask] Tried to stop " << name_ << " but it wasn't running!\n";
        return;
    }
    running_ = false;
    std::cout << "[PeriodicTask] Waiting for " << name_ << " to stop...\n";
    thread_.join();
    std::cout << "[PeriodicTask] Done!\n";
    cleanup();
}

bool PeriodicTask::is_loop_overrun() const {
    if (last_period_time_ > period_.count() * 1.3f || max_runtime_ > period_.count()) {
        printf("[PeriodicTask] Warning: Task %s is running slow!\n", name_.c_str());
        return true;
    }
    return false;
}

void PeriodicTask::print_status() const {
    if (!running_) return;
    if (is_loop_overrun()) {
        printf("|%-20s|%6.4f|%6.4f|%6.4f|%6.4f|%6.4f\n",
               name_.c_str(), last_runtime_, max_runtime_, period_.count(),
               last_period_time_, max_period_);
    } else {
        printf("|%-20s|%6.4f|%6.4f|%6.4f|%6.4f|%6.4f\n", name_.c_str(),
               last_runtime_, max_runtime_, period_.count(), last_period_time_, max_period_);
    }
}

void PeriodicTask::clear_max() {
    max_period_ = 0;
    max_runtime_ = 0;
}

void PeriodicTask::loop_function() {
    auto timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);

    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(period_);
    auto nano_seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(period_ - seconds);

    itimerspec timer_spec{};
    timer_spec.it_interval.tv_sec = seconds.count();
    timer_spec.it_value.tv_sec = seconds.count();

    timer_spec.it_interval.tv_nsec = nano_seconds.count();
    timer_spec.it_value.tv_nsec = nano_seconds.count();

    timerfd_settime(timer_fd, 0, &timer_spec, nullptr);

    unsigned long long missed = 0;

    auto last_run_time = std::chrono::high_resolution_clock::now();

    while (running_) {
        auto start = std::chrono::high_resolution_clock::now();
        run();
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> runtime = end - start;
        last_runtime_ = runtime.count();

        std::chrono::duration<float> period = start - last_run_time;
        last_period_time_ = period.count();

        last_run_time = start;

        ssize_t m = read(timer_fd, &missed, sizeof(missed));
        (void)m;
        is_loop_overrun();

        std::cout << "Task " << name_ << " period: " << last_period_time_ << " seconds\n";
    }
}

const std::string& PeriodicTask::get_name() const {
    return name_;
}

PeriodicTaskManager::~PeriodicTaskManager() {
    stop_all();
}

void PeriodicTaskManager::stop_all() {
    for (auto& task : tasks_) {
        task->stop();
    }
}

void PeriodicTaskManager::add_task(PeriodicTask* task) {
    tasks_.push_back(std::unique_ptr<PeriodicTask>(task));
}

void PeriodicTaskManager::print_status() {
    for (auto& task : tasks_) {
        task->print_status();
        task->clear_max();
    }
}


