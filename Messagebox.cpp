#include "Messagebox.h"
#include "Timer.h"
#include "ImagePool.h"
#include "FL/Fl_Window.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Multiline_Output.H"


Messagebox::Messagebox(int _playerX, int _playerY, std::string _text, Fl_Window* _winToAdd) :
	Timer(),
	m_textToShow(_text),
	m_textBoxImage(nullptr),
	m_textBox(nullptr)
{
	m_textBoxImage = new Fl_Box(_playerX + 20, _playerY - 100, 200, 30, "");
	m_textBoxImage->image(ImagePool::GetImage(ImagePool::ImageType::TEXTBOX));
	m_textBox = new Fl_Multiline_Output(_playerX + 25, _playerY - 120, 195, 75, 0);
	m_textBox->insert(m_textToShow.c_str());
	m_textBox->box(FL_NO_BOX);

	_winToAdd->add(m_textBoxImage);
	_winToAdd->add(m_textBox);
}

Messagebox::~Messagebox()
{

}

void Messagebox::UpdatePosition(int _xIncrement, int _yIncrement)
{
	m_textBoxImage->position(m_textBoxImage->x() + _xIncrement, m_textBoxImage->y() + _yIncrement);
}

void Messagebox::show()
{
	Fl::add_timeout(10, Tick, (void*)this);
	m_textBoxImage->show();
	m_textBox->show();
}

void Messagebox::OnTick(void* _userData)
{
	delete m_textBoxImage;
	delete m_textBox;
}