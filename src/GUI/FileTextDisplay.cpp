#include "GUI/FileTextDisplay.h"

#include "FL/Fl_Text_Display.H"
#include "FL/Fl_Text_Buffer.H"

#include <string>
#include <fstream>
#include <memory>

FileTextDisplay::FileTextDisplay(const char* _fileName, int _parentW, int _parentH) : 
	Fl_Text_Display(5, 35, _parentW - 10, _parentH - 40),
	m_textBuffer()
{

	std::ifstream aboutTextFile(_fileName);
	if (!aboutTextFile.is_open())
	{
		printf("Error opening %s\n", _fileName);
	}

	//Read all text from file into textString
	std::string textString;
	int currentIndex{ 0 };
	while (std::getline(aboutTextFile, textString))
	{
		m_textBuffer.insert(currentIndex, textString.c_str());
		currentIndex += (int)textString.size();

		m_textBuffer.insert(currentIndex, "\n");
		++currentIndex;
	}

	buffer(m_textBuffer);
}

FileTextDisplay::~FileTextDisplay()
{
	//Stops Fl_Text_Buffer errors on close
	buffer(NULL);

}