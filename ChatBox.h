#pragma once
#include "FL/Fl_Group.H"
#include "FL/Fl_Multiline_Input.H"
#include "FL/Fl_Button.H"

class Chatbox :
    public Fl_Group
{
private:
    Fl_Multiline_Input* m_inputBuffer;
    int m_mode;

    int handle(int _event);
public:
    Chatbox();
    ~Chatbox();

    /**
        Wrapper for default show() that also takes an int indicating what the dialoguebox is being used for.
        For example, entering a new username or sending out a message.
    */
    void Display(int _mode);
};

