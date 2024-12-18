#include "FL/Fl_Text_Display.H"
#include "FL/Fl_Text_Buffer.H"

#include <memory>

#pragma once
class FileTextDisplay : public Fl_Text_Display
{
private:
	Fl_Text_Buffer m_textBuffer;

public:
	FileTextDisplay(const char* _fileName, int _parentW, int _parentH);
	~FileTextDisplay();
};

