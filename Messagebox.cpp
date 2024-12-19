#include "Messagebox.h"
#include "Timer.h"
#include "ImagePool.h"

#include "FL/Fl_Window.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Multiline_Output.H"


Messagebox::Messagebox(int _playerX, int _playerY) :
	Timer(),
	Fl_Group(_playerX + 20, _playerY - 100, 200, 30, ""),
	m_textBoxImage(_playerX + 20, _playerY - 100, 200, 30, ""),
	m_textBox(_playerX + 35, _playerY - 120, 175, 75)
{
	m_textBoxImage.image(ImagePool::GetImage(ImageType::TEXTBOX).get());
	
	m_textBox.box(FL_NO_BOX);

	m_textBoxImage.show();
	m_textBox.show();
	hide();
	end();
}

Messagebox::~Messagebox()
{

}


void Messagebox::OnTick()
{
	//printf("MessageTick\n");
	m_textBox.value("");
	hide();
	redraw();
}

void Messagebox::DisplayMessage(std::string& _message)
{
	//fl_alert("Message to show: %s", _message.c_str());

	if (m_textBox.value() != "")
	{
		m_textBox.value(_message.c_str());

		Fl::repeat_timeout(6, TickOnce, this);
	}
	else
	{
		m_textBox.insert(_message.c_str());
		show();

		Fl::add_timeout(6, TickOnce, this);
	}
}