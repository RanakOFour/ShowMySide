#include "Messagebox.h"
#include "Timer.h"
#include "ImagePool.h"
#include "FL/Fl_Window.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Multiline_Output.H"


Messagebox::Messagebox(int _playerX, int _playerY) :
	Fl_Group(_playerX + 20, _playerY - 100, 200, 30, ""),
	Timer(),
	m_textToShow(""),
	m_textBoxImage(nullptr),
	m_textBox(nullptr)
{
	m_textBoxImage = new Fl_Box(_playerX + 20, _playerY - 100, 200, 30, "");
	m_textBoxImage->image(ImagePool::GetImage(ImagePool::ImageType::TEXTBOX));
	m_textBox = new Fl_Multiline_Output(_playerX + 25, _playerY - 120, 195, 75, 0);
	m_textBox->insert(m_textToShow.c_str());
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


void Messagebox::OnTick(void* _userData)
{
	hide();
	m_textToShow = "";
	m_textBox->value("");
}

void Messagebox::DisplayMessage(std::string _message)
{
	m_textToShow = _message;
	m_textBox->insert(m_textToShow.c_str());
	show();
	Fl::add_timeout(10, Tick, (void*)this);
}