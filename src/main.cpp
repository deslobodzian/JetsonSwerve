//
// Created by deslobodzian on 4/26/23.
//

#include <iostream>
#include <PeriodicTask.hpp>

class MyTask : public PeriodicTask {
public:
    MyTask(PeriodicTaskManager* taskManager, float period, std::string name)
            : PeriodicTask(taskManager, period, std::move(name)) {}

    void init() override {
        std::cout << "Initializing task " << getName() << std::endl;
    }

    void run() override {
        std::cout << "Running task " << getName() << std::endl;
    }

    void cleanup() override {
        std::cout << "Cleaning up task " << getName() << std::endl;
    }
};
int main() {
    PeriodicTaskManager taskManager;
    MyTask task(&taskManager, 0.0005, "MyTask");  // 0.0005 seconds = 500 microseconds
    task.start();
    MyTask task2(&taskManager, 0.005, "MyTask2");  // 0.005 seconds = 5 milliseconds
    task2.start();

    // Let the task run for a while
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Stop the task
    task.stop();
    task2.stop();

    return 0;
}