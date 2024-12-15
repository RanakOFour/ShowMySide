#include <FL/Fl_Timer.H>

#pragma once
class Timer
{
protected:
	void* m_userdata;
	double m_duration;
	static void TickRepeat(void* _userData);
	static void TickOnce(void* _userData);
	virtual void OnTick();

public:
	Timer();
	Timer(double _duration);
	virtual ~Timer();
};
