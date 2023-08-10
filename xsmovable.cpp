/**
 * @file xsmovable.cpp 
 * @brief: 可移动对象的实现
 * @author: XSSunet
 * @date: 2023-07-16 01:00:13
 * 
 **/

#include "xsmovable.h"
#include <windows.h>

XSMovable::XSMovable()
{
	m_pos.x = XSGRAPHIC_COMPATIBLEBMP_W / 2;
	m_pos.y = XSGRAPHIC_COMPATIBLEBMP_H / 2;
	
	b_in = false;
	b_push = false;
}

XSMovable::XSMovable(int x, int y)
{
	m_pos.x = x;
	m_pos.y = y;
	
	b_in = false;
	b_push = false;
}

XSMovable::~XSMovable()
{
	
}

void XSMovable::move_to(int x, int y)
{
	if(b_push)
	{
		this->m_move_to(x, y);
		this->m_pos.x = x;
		this->m_pos.y = y;
	}
	else this->m_subobjects_move_to(x, y);
}

void XSMovable::draw(XSGraphic *g)
{
	this->m_draw(g, b_in, b_push);
}

bool XSMovable::b_in_range(int x, int y)
{
	b_in = !this->m_b_in_subobject_range(x, y);
	b_in &= this->m_delta(x, y);
	return b_in;
}

void XSMovable::push()
{
	b_push = true;
}

void XSMovable::pop()
{
	b_push = false;
}

POINT *XSMovable::get_pos()
{
	return &this->m_pos;
}

void XSMovable::set_pos(int x, int y)
{
	this->m_pos.x = x;
	this->m_pos.y = y;
}

bool XSMovable::m_b_in_subobject_range(int, int)
{
	return false;
}

void XSMovable::m_subobjects_move_to(int, int)
{
	
}

void XSMovable::m_move_to(int x, int y)
{
	
}
