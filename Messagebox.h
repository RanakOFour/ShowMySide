#include "Timer.h"

#include "FL/Fl_Group.H"
#include "FL/Fl_Multiline_Output.H"
#include "FL/Fl_Box.H"

#include <string>
#include <memory>

#pragma once

/**Text bubble that appears next to a parent Player when DisplayMessage() is called*/
class Messagebox : public Timer, public Fl_Group
{
private:
    Fl_Multiline_Output m_textBox;
    Fl_Box m_textBoxImage;

    /**
        Hides the text and image
    */
    void OnTick();

public:
    Messagebox(int _playerX, int _playerY);
    ~Messagebox();

    /**
        Shows FLTK property components and loads message into m_textToShow and m_textBox text
    */
    void DisplayMessage(std::string& _message);
};

