#include "Chatbox.h"
#include "FL/Fl_Group.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Multiline_Input.H"

Chatbox::Chatbox() :
	Fl_Group(0, 0, 300, 300, "")
{
	m_inputBuffer = new Fl_Multiline_Input(220, 260, 200, 50, "Enter Text");
	m_sendButton = new Fl_Button(270, 320, 100, 50, "Send");

	add(m_inputBuffer);
	add(m_sendButton);
	end();


	m_inputBuffer->show();
	m_sendButton->show();
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
		if (Fl::event_key() == FL_Enter)
		{
			hide();
		}
	}
}