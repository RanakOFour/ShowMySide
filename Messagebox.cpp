#include "Messagebox.h"
#include "Timer.h"
#include "ImagePool.h"
#include "FL/Fl_Window.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Multiline_Output.H"


Messagebox::Messagebox(int _playerX, int _playerY) :
	Fl_Group(_playerX + 20, _playerY - 100, 200, 30, ""),
	Timer(),
	m_textBoxImage(nullptr),
	m_textBox(nullptr)
{
	m_textBoxImage = new Fl_Box(_playerX + 20, _playerY - 100, 200, 30, "");
	m_textBoxImage->image(ImagePool::GetImage(ImageType::TEXTBOX).get());
	m_textBox = new Fl_Multiline_Output(_playerX + 35, _playerY - 120, 175, 75, 0);
	m_textBox->box(FL_NO_BOX);

	add(m_textBox);
	add(m_textBoxImage);
	m_textBoxImage->show();
	m_textBox->show();
	hide();
	end();
}

Messagebox::~Messagebox()
{

}


void Messagebox::OnTick()
{
	printf("MessageTick\n");
	hide();
	m_textBox->value("");
	redraw();
}

void Messagebox::DisplayMessage(std::string& _message)
{
	if (m_textBox->value() != "")
	{
		m_textBox->value("");
		m_textBox->value(_message.c_str());
		if (Fl::has_timeout(TickOnce, this))
		{
			Fl::remove_timeout(TickOnce, this);
		}

		Fl::add_timeout(6, TickOnce, (void*)this);
	}
	else
	{
		m_textBox->insert(_message.c_str());
		show();

		Fl::add_timeout(6, TickOnce, (void*)this);
	}
}