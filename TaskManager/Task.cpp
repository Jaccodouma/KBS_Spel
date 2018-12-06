/*
* Task.h
*
* Created: 12/5/2018 11:13:01
* Author: Jacco Douma
*/
#include "Task.h"

/* addTask() */
void TaskManager::addTask(Task *taskToAdd) {
	tasks[taskCount] = taskToAdd;
	taskCount++;
}

/* doTask() */
void TaskManager::doTask() {	
	// run current task object
	// if it returns true (TASK_DONE), go to next task
	if (tasks[currentTasknr]->run()) {
		// go to next task
		currentTasknr++;
		currentTasknr = currentTasknr % taskCount;
	};
}

int Task::run() {
	// TODO: Remove serial print
	Serial.println("run() not implemented yet!");
	return TASK_DONE;
}