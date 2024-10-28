#include "ThirdsColumn.h"

ThirdsColumn::ThirdsColumn(int x, int y, int w, int h) : Fl_Flex(x, y, w, h, 0)
{
	m_secondFlex = new Fl_Flex(0);
	add_resizable(*m_secondFlex);

	Fl_Flex* m_thirdFlex = new Fl_Flex(0);
	add_resizable(*m_thirdFlex);
}

ThirdsColumn::~ThirdsColumn()
{
	delete child(1);
	delete child(0);
}

Fl_Flex* ThirdsColumn::second()
{
	return m_secondFlex;
}

Fl_Flex* ThirdsColumn::third()
{
	return m_thirdFlex;
}