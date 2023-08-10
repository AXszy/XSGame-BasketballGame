/**
 * @file xsbutton.cpp 
 * @brief: 按钮类实现
 * @author: XSSunet
 * @date: 2023-07-25 01:23:37
 * 
 **/
#include "xsbutton.h"

XSButton::XSButton()
{
	this->b_in = false;
	this->b_push = false;
}

XSButton::~XSButton()
{
	
}

void XSButton::push()
{
	b_push = true;
}

void XSButton::pop()
{
	if(b_push) 
	{
		b_push = false;
		this->m_func();
	}
}

bool XSButton::b_in_range(int x, int y)
{
	b_in = m_delta(x, y);
	if(!b_in) b_push = false;
	return b_in;
}

void XSButton::draw(XSGraphic *g)
{
	this->m_draw(g, b_in, b_push);
}
