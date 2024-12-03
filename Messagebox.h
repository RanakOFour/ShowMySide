#include "Timer.h"
#include "FL/Fl_Box.H"
#include "FL/Fl_Multiline_Output.H"
#include <string>

#pragma once

class Messagebox :
    private Timer
{
private:
    std::string m_textToShow;
    Fl_Box* m_textBoxImage;
    Fl_Multiline_Output* m_textBox;
    void OnTick(void* _userData);

public:
    Messagebox(int _playerX, int _playerY, std::string _text, Fl_Window* _winToAdd);
    ~Messagebox();

    void UpdatePosition(int _xIncrement, int yIncrement);
    void show();
};

