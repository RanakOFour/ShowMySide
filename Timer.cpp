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
	Fl::add_timeout(_duration, TickRepeat, this);
}

Timer::~Timer()
{
	if(Fl::has_timeout(TickRepeat, this))
	{
		Fl::remove_timeout(TickRepeat, this);
	}

	if (Fl::has_timeout(TickOnce, this))
	{
		Fl::remove_timeout(TickOnce, this);
	}
}

void Timer::OnTick()
{
	printf("Blarg\n");
}

void Timer::TickRepeat(void* _userData)
{
	Timer* timer = (Timer*)_userData;
	timer->OnTick();
	Fl::repeat_timeout(timer->m_duration, TickRepeat, _userData);
}

void Timer::TickOnce(void* _userData)
{
	Timer* timer = (Timer*)_userData;
	timer->OnTick();
}