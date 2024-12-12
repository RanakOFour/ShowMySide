#include "Timer.h"

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
}

void Timer::Tick(void* _userData)
{
	Timer* timer = (Timer*)_userData;
	timer->OnTick(_userData);
	Fl::repeat_timeout(timer->m_duration, Tick, _userData);
}