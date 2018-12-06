#include "TaskManager.h"
#include <Arduino.h>

class TestTask_side: public Task 
{
public:
	// Example side task, only prints that it's a side task
	int run() {
		Serial.println("TASK: SIDE");
		Serial.println("");
		return TASK_DONE;
	}	
};

class TestTask: public Task
{
public:
	/* Constructor */
	TestTask(int number, TaskManager *taskManager, Task *sideTask) {
		this->taskNumber = number;
		this->taskManager = taskManager;
		this->sideTask = sideTask;
	};
	int run() {
		// Print task and received number
		Serial.print("TASK: ");
		Serial.println(taskNumber);
		
		// Side task example
		if (taskNumber == 2) { // Add side task on this number
			Serial.println("Adding side task");
			taskManager->addSideTask(sideTask);
		}
		
		// Print extra line to make console readable
		Serial.println("");
		
		// Task is done, return TASK_BUSY if this function needs to run again
		return TASK_DONE;
	}
	
private: 
	int taskNumber;
	TaskManager *taskManager; 
	Task *sideTask; 
};

int main(void) {
	
	// Set up serial communication
	init();
	Serial.begin(9600);
	
	// Create task manager object
	Serial.println("Creating TaskManager object.");
	TaskManager *taskManager = new TaskManager;
	
	// Create tasks 
	Serial.println("Creating Task objects.");
	
	Task *sideTask = new TestTask_side;
	
	Task *task1 = new TestTask(1, taskManager, sideTask);
	Task *task2 = new TestTask(2, taskManager, sideTask);
	Task *task3 = new TestTask(3, taskManager, sideTask);
	Task *task4 = new TestTask(4, taskManager, sideTask);
	Task *task5 = new TestTask(5, taskManager, sideTask);
	Task *task6 = new TestTask(6, taskManager, sideTask);
	Task *task7 = new TestTask(7, taskManager, sideTask);
	
	// Add tasks to task manager (in order)
	Serial.println("Adding Tasks to TaskManager");
	taskManager->addTask(task1);
	taskManager->addTask(task2);
	taskManager->addTask(task3);
	taskManager->addTask(task4);
	taskManager->addTask(task5);
	taskManager->addTask(task6);
	taskManager->addTask(task7);
	
	// Print out message stating we're starting tasks
	Serial.println("===============");
	Serial.println("STARTING TASKS");
	Serial.println("===============");
	
	while (1) {
		// Execute current task and wait a bit
		taskManager->doTask();
		_delay_ms(2000);
	};
}