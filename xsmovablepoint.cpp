/**
 * @file xsmovablepoint.cpp 
 * @brief: 可移动的点实现
 * @author: XSSunet
 * @date: 2023-07-16 11:28:10
 * 
 **/
#include "xsmovablepoint.h"
#include <windows.h>
#include "xsgraphic.h"

XSMovablePoint::XSMovablePoint()
{
	m_r = 5;
}

XSMovablePoint::XSMovablePoint(int x, int y, int r) : XSMovable(x, y)
{
	m_r = r;
}

XSMovablePoint::~XSMovablePoint()
{
	
}

bool XSMovablePoint::m_delta(int x, int y)
{
	POINT *p = this->get_pos();
	return (p->x >= x - m_r) && (p->x <= x + m_r) && (p->y >= y - m_r) && (p->y <= y + m_r);
}

void XSMovablePoint::m_draw(XSGraphic *g, bool bin, bool bpush)
{
	POINT *p = this->get_pos();
	
	TRIVERTEX vtx[2];
	vtx[0].x = p->x - m_r;  
	vtx[0].y = p->y - m_r;  
	vtx[0].Alpha = 0;  
	vtx[0].Red = 0x0004;  
	vtx[0].Green = 0x0017;  
	vtx[0].Blue = 0xff00;  
	vtx[1].x = p->x + m_r;  
	vtx[1].y = p->y + m_r;  
	vtx[1].Alpha = 0;  
	vtx[1].Red = 0xfac0;  
	vtx[1].Green = 0xcccc;  
	vtx[1].Blue = 0xcef0;
	
	GRADIENT_RECT grc[1];
	grc[0].UpperLeft = 0;
	grc[0].LowerRight = 1;

	if(bpush || bin)
	{
		g->rectangle(p->x - m_r - 1, p->y - m_r - 1, p->x + m_r + 1, p->y + m_r + 1);
	}
	g->gradient_fill(vtx, 2, grc, 1, GRADIENT_FILL_RECT_V);
}
