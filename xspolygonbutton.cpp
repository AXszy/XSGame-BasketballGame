/**
 * @file xspolygonbutton.cpp 
 * @brief: 多边形按钮实现
 * @author: XSSunet
 * @date: 2023-07-25 01:42:33
 * 
 **/
#include "xspolygonbutton.h"

XSPolygonButton::XSPolygonButton(POINT *apt, int cpt)
{
	this->m_apt = new POINT[cpt];
	
	for(int i = 0; i < cpt; i++)
	{
		this->m_apt[i] = apt[i];
	}
	
	this->m_cpt =  cpt;
	this->m_function = [](){};
	
	m_hp_normal = m_hp_in = m_hp_pushed = (HPEN)GetStockObject(WHITE_PEN);
	m_hbr_normal = (HBRUSH)GetStockObject(GRAY_BRUSH);
	m_hbr_in = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	m_hbr_pushed = (HBRUSH)GetStockObject(WHITE_BRUSH);
}

XSPolygonButton::~XSPolygonButton()
{
	delete[] this->m_apt;
}

void XSPolygonButton::set_pen_normal(HPEN hp)
{
	this->m_hp_normal = hp;
}

void XSPolygonButton::set_pen_in(HPEN hp)
{
	this->m_hp_in = hp;
}

void XSPolygonButton::set_pen_pushed(HPEN hp)
{
	this->m_hp_pushed = hp;
}

void XSPolygonButton::set_brush_normal(HBRUSH hbr)
{
	this->m_hbr_normal = hbr;
}

void XSPolygonButton::set_brush_in(HBRUSH hbr)
{
	this->m_hbr_in = hbr;
}

void XSPolygonButton::set_brush_pushed(HBRUSH hbr)
{
	this->m_hbr_pushed = hbr;
}

bool XSPolygonButton::XSPolygonButton::m_delta(int x, int y)
{
	//计数 : 与边的交点个数
	int n_inter = 0;
	for(int i = 0; i < this->m_cpt; i++)
	{
		//取两个点
		POINT *p1 = this->m_apt + i;
		POINT *p2 = this->m_apt + ((i + 1) % this->m_cpt);
		
		int max_y = (p1->y > p2->y) ? p1->y : p2->y;
		int min_y = (p1->y < p2->y) ? p1->y : p2->y;
		int max_x = (p1->x > p2->x) ? p1->x : p2->x;
		int min_x = (p1->x < p2->x) ? p1->x : p2->x;
		
		if(p1->x == p2->x)
		{
			if(x == p1->x && y <= min_y)
			{
				n_inter++;
			}
			continue;
		}
		else if(x >= min_x && x <= max_x)
		{
			double y0 = double(p2->y - p1->y) * (x - p1->x) / (p2->x - p1->x) + p1->y;
			if(y < y0 && y0 >= min_y && y0 <= max_y)
			{
				n_inter++;
			}
			continue;
		}
	}
	return (n_inter % 2 == 1);
}

void XSPolygonButton::m_draw(XSGraphic *g, bool bin, bool bpush)
{
	if(bpush)
	{
		g->select(m_hbr_pushed);
		g->select(m_hp_pushed);
	}
	else if(bin)
	{
		g->select(m_hbr_in);
		g->select(m_hp_in);
	}
	else
	{
		g->select(m_hbr_normal);
		g->select(m_hp_normal);
	}
	g->polygon(this->m_apt, this->m_cpt);
}

void XSPolygonButton::set_function(std::function<VOID()> func)
{
	this->m_function = func;
}

void XSPolygonButton::m_func()
{
	this->m_function();
}
