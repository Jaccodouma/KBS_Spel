/* 
* Task.h
*
* Created: 12/5/2018 11:13:01
* Author: Jacco Douma
*/
#ifndef __TASK_H__
#define __TASK_H__

#include <stdio.h>
#include <Arduino.h> // Used for Serial.println

#define MAX_TASKS 40

// returns for run() function
#define TASK_BUSY 0
#define TASK_DONE 1

class Task
{
public:
	virtual int run();
}; //Task

class TaskManager
{
public: 
	void addTask(Task *taskToAdd); // adds task to TaskManager
	void doTask(); // Executes the current task
	
private: 
	int currentTasknr = 0;
	int taskCount = 0;
	Task *tasks[MAX_TASKS];
};

#endif //__TASK_H__
