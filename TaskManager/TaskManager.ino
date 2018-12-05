#include "Task.h"
#include <Arduino.h>

class TestTask: public Task
{
public:
	TestTask(int number) {taskNumber = number;};
	int run() {
		Serial.print("Task executed: ");
		Serial.println(taskNumber);
		
		return TASK_DONE;
	}
	
private: 
	int taskNumber;
};

int main(void) {
	
	init();
	Serial.begin(9600);
	
	Serial.println("Creating TaskManager object.");
	TaskManager taskManager;
	
	Serial.println("Creating Task objects.");
	Task *task1 = new TestTask(1);
	Task *task2 = new TestTask(2);
	Task *task3 = new TestTask(3);
	Task *task4 = new TestTask(4);
	Task *task5 = new TestTask(5);
	Task *task6 = new TestTask(6);
	Task *task7 = new TestTask(7);
	
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
	};
}