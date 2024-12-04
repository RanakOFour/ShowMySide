#pragma once
#include "FL/Fl_Group.H"
#include "FL/Fl_Multiline_Input.H"
#include "FL/Fl_Button.H"

class Chatbox :
    public Fl_Group
{
private:
    Fl_Button* m_sendButton;
    Fl_Multiline_Input* m_inputBuffer;

    int handle(int _event);
public:
    Chatbox();
    ~Chatbox();
};

