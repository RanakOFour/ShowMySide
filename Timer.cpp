#include "Timer.h"

Timer::Timer(double _duration)
{
	m_duration = _duration;
	m_userdata = (void*)0;
	Fl::add_timeout(_duration, tick, this);
}

Timer::~Timer()
{
	Fl::remove_timeout(tick, this);
}

void Timer::on_tick(void* _userData)
{
	m_userdata = _userData;
	printf("Tick!\n");
	Fl::repeat_timeout(m_duration, tick, m_userdata);
}

void Timer::tick(void* _userData)
{
	Timer* timer = (Timer*)_userData;
	timer->on_tick(_userData);
}