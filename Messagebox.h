#include "Timer.h"
#include "FL/Fl_Group.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Multiline_Output.H"
#include <string>

#pragma once

class Messagebox : public Fl_Group,
    private Timer
{
private:
    Fl_Multiline_Output* m_textBox;
    Fl_Box* m_textBoxImage;

    /**
        Hides the text and image
    */
    void OnTick(void* _userData);

public:
    Messagebox(int _playerX, int _playerY);
    ~Messagebox();

    /**
        Shows FLTK property components and loads message into m_textToShow and m_textBox text
    */
    void DisplayMessage(std::string& _message);
};

