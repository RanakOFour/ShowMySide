#include "Chatbox.h"
#include "FL/Fl_Group.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Multiline_Input.H"

Chatbox::Chatbox() :
	Fl_Group(0, 0, 300, 300, ""),
	m_mode(-1)
{
	m_inputBuffer = new Fl_Multiline_Input(550, 375, 200, 50, "Enter Text");

	add(m_inputBuffer);
	end();


	m_inputBuffer->show();
	hide();
}

Chatbox::~Chatbox()
{

}

int Chatbox::handle(int _event)
{
	switch (_event)
	{
	case FL_FOCUS:
		return 1;

	case FL_KEYDOWN:
		m_inputBuffer->take_focus();
		if (Fl::event_key() == FL_Enter)
		{
			hide();
		}
	}
}

void Chatbox::Display(int _mode)
{
	m_mode = _mode;
	show();
	take_focus();
}