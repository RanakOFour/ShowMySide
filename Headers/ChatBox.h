#include "FL/Fl_Group.H"
#include "FL/Fl_Input.H"

#include <string>
#include <memory>

#pragma once

/** A text box that is shown in the middle of the Lobby window during certain events */
class Chatbox :
    public Fl_Group
{
private:
    Fl_Input m_textBox;

    int handle(int _event);
public:
    Chatbox();
    ~Chatbox();

    int m_mode;

    /**
        Wrapper for default show() that also takes an int indicating what the dialoguebox is being used for.
        For example, entering a new username or sending out a message.
    */
    void Display(int _mode);

    std::string FlushMessage();
};

