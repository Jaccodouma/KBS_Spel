#include "Task.h"
#include <Arduino.h>

int main(void) {
	
	init();
	Serial.begin(9600);
	
	Serial.println("Creating TaskManager object.");
	TaskManager taskManager;
	
	Serial.println("Creating Task objects.");
	Task *task1 = new TestTask;
	Task *task2 = new TestTask;
	Task *task3 = new TestTask;
	Task *task4 = new TestTask;
	Task *task5 = new TestTask;
	Task *task6 = new TestTask;
	Task *task7 = new TestTask;
	
	Serial.println("Adding Tasks to TaskManager");
	taskManager.addTask(task1);
	taskManager.addTask(task2);
	taskManager.addTask(task3);
	taskManager.addTask(task4);
	taskManager.addTask(task5);
	taskManager.addTask(task6);
	taskManager.addTask(task7);
	
	while (1) {
		// Executing tasks
		taskManager.doTask();
		_delay_ms(200);
		taskManager.nextTask();
		_delay_ms(500);
	};
}