#include "Timer.h"

Timer::Timer()
{
	m_duration = 0;
	m_userdata = (void*)0;
}

Timer::Timer(double _duration)
{
	m_duration = _duration;
	m_userdata = (void*)0;
	Fl::add_timeout(_duration, Tick, this);
}

Timer::~Timer()
{
	Fl::remove_timeout(Tick, this);
}

void Timer::OnTick(void* _userData)
{
	m_userdata = _userData;
	printf("Timer Tick!\n");
	Fl::repeat_timeout(m_duration, Tick, m_userdata);
}

void Timer::Tick(void* _userData)
{
	Timer* timer = (Timer*)_userData;
	timer->OnTick(_userData);
}