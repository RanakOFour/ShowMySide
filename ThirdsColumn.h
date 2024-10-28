#include <FL/Fl_Flex.H>

#pragma once
class ThirdsColumn : public Fl_Flex
{
private:
	Fl_Flex* m_secondFlex;
	Fl_Flex* m_thirdFlex;


public:
	ThirdsColumn(int x, int y, int w, int h);
	~ThirdsColumn();
	Fl_Flex* second();
	Fl_Flex* third();
};

