#include "Chatbox.h"
#include <FL/Fl_Group.H>
#include "FL/Fl_Button.H"
#include "FL/Fl_Input.H"

Chatbox::Chatbox() :
	Fl_Group(0, 0, 300, 300, ""),
	m_textBox(550, 375, 200, 50, "Enter Text"),
	m_mode(-1)
{
	add(m_textBox);
	end();

	m_textBox.show();
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
		m_textBox.take_focus();
		return 1;

	case FL_KEYDOWN:
		if (Fl::event_key() == FL_Enter)
		{
			hide();
			return 1;
		}

	case FL_PUSH:
		take_focus();
		return 1;
	}

	return 0;
}

void Chatbox::Display(int _mode)
{
	m_mode = _mode;

	switch (m_mode)
	{
	case 0:
		m_textBox.label("Enter message:");
		break;

	case 1:
		m_textBox.label("Enter new username:");
		break;
	}

	show();
	take_focus();
}

std::string Chatbox::FlushMessage()
{
	if (!visible())
	{
		std::string toReturn = m_textBox.value();
		m_textBox.value("");

		return toReturn;
	}

	return "";
}