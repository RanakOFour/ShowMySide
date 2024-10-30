#include <FL/Fl_Timer.H>

#pragma once
class Timer
{
public:
	Timer(double _duration);
	virtual ~Timer();
	virtual void on_tick(void* _userData);
private:
	void* m_userdata;
	double m_duration;
	static void tick(void* _userData);
};
