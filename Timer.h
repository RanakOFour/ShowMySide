#include <FL/Fl_Timer.H>

#pragma once
class Timer
{
public:
	Timer(double _duration);
	virtual ~Timer();
	virtual void OnTick(void* _userData);
protected:
	void* m_userdata;
	double m_duration;
	static void Tick(void* _userData);
};
