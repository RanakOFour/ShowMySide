#pragma once
#include "FL/Fl_Group.H"
#include "FL/Fl_Text_Buffer.H"
#include "FL/Fl_Button.H"

class ChatBox :
    public Fl_Group
{
private:
    Fl_Button* m_sendButton;
    Fl_Text_Buffer* m_inputBuffer;


public:
    ChatBox();
    ~ChatBox();

    void AddTo(Fl_Window* _windowToAdd);
    void RemoveFrom(Fl_Window* _windowToRemove);
};

