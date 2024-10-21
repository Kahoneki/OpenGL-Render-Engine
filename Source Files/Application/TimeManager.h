#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

class Application;

class TimeManager
{
public:
	TimeManager(Application* _app);
	void NewFrame();
	double dt;

private:
	Application* app;

	double lastTime;
};

#endif