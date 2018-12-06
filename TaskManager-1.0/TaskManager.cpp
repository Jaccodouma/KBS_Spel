/*
* Task.h
*
* Created: 12/5/2018 11:13:01
* Author: Jacco Douma
*/
#include "TaskManager.h"

/* addTask() */
void TaskManager::addTask(Task *taskToAdd) {
	tasks[taskCount] = taskToAdd;
	taskCount++;
}

/* addSideTask() */
void TaskManager::addSideTask(Task *taskToAdd) {
	this->hasSideTask = 1; 
	this->sideTask = taskToAdd;
}

/* doTask() */
void TaskManager::doTask() {
	if (!this->hasSideTask) {
		// run current main task object
		// if it returns true (TASK_DONE), go to next task
		if (tasks[currentTasknr]->run()) {
			// go to next task
			currentTasknr++;
			currentTasknr = currentTasknr % taskCount;
		};
	} else {
		// run sideTask
		if (sideTask->run()) {
			sideTask = NULL;
			hasSideTask = 0;
		}
	}

}

int Task::run() {
	Serial.println("run() not implemented yet!");
	return TASK_DONE;
}