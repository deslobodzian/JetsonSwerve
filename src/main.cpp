//
// Created by deslobodzian on 4/26/23.
//

#include <iostream>
#include <PeriodicTask.hpp>

class MyTask : public PeriodicTask {
public:
    MyTask(PeriodicTaskManager* task_manager, float period, std::string name)
            : PeriodicTask(task_manager, period, std::move(name)) {}

    void init() override {
        std::cout << "Initializing task " << get_name() << std::endl;
    }

    void run() override {
        std::cout << "Running task " << get_name() << std::endl;
    }

    void cleanup() override {
        std::cout << "Cleaning up task " << get_name() << std::endl;
    }
};

int main() {
    PeriodicTaskManager task_manager;
    MyTask task(&task_manager, 0.0005, "MyTask");  // 0.0005 seconds = 500 microseconds
    task.start();
    MyTask task2(&task_manager, 0.005, "MyTask2");  // 0.005 seconds = 5 milliseconds
    task2.start();

    // Let the task run for a while
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Stop the task
    task.stop();
    task2.stop();

    return 0;
}