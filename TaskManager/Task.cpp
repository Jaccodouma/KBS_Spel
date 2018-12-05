#include "Task.h"

/* addTask() */
void TaskManager::addTask(Task *taskToAdd) {
	tasks[taskCount] = taskToAdd;
	taskCount++;
}

/* doTask() */
void TaskManager::doTask() {
	Serial.print("currentTasknr: \t");Serial.println(currentTasknr);
	Serial.print("taskCount: \t");	Serial.println(taskCount);
	tasks[currentTasknr]->run(); // run current task
}

/* nextTask() */
void TaskManager::nextTask() {
	currentTasknr++;
	currentTasknr = currentTasknr % taskCount;
}

void Task::run() {
	Serial.println("run() not implemented yet!");
}
void TestTask::run() {
	Serial.println("Task executed");
}