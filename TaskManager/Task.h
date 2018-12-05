/* 
* Task.h
*
* Created: 12/5/2018 11:13:01
* Author: Jacco
*/


#ifndef __TASK_H__
#define __TASK_H__

#include <stdio.h>
#include <Arduino.h> // Used for Serial.println

#define MAX_TASKS 40

class Task
{
public:
	virtual void run();
}; //Task

class TaskManager
{
public: 
	void addTask(Task *taskToAdd); // adds task to TaskManager
	void doTask(); // Executes the current task
	void nextTask();
	
private: 
	int currentTasknr = 0;
	int taskCount = 0;
	Task *tasks[MAX_TASKS];
};

class TestTask: public Task
{
	void run();
}; // TestTask1

#endif //__TASK_H__
