/**
 * @file xsmovablepolygon.cpp 
 * @brief: 可移动多边形类实现
 * @author: XSSunet
 * @date: 2023-07-23 10:41:44
 * 
 **/
#include "xsmovablepolygon.h"

XSMovablePolygon::XSMovablePolygon(POINT *apt, int cpt) : XSMovable(0, 0)
{
	this->m_cpt = cpt;
	this->m_mvbl_pt = new XSMovablePoint[cpt];
	
	for(int i = 0; i < cpt; i++)
	{
		m_mvbl_pt[i].set_pos(apt[i].x, apt[i].y);
	}
	
	this->m_blbuttondown = false;
	this->m_blbuttonup = false;
}

XSMovablePolygon::~XSMovablePolygon()
{
	delete[] this->m_mvbl_pt;
}

bool XSMovablePolygon::m_delta(int x, int y)
{
	//计数 : 与边的交点个数
	int n_inter = 0;
	for(int i = 0; i < this->m_cpt; i++)
	{
		//取两个点
		POINT *p1 = (this->m_mvbl_pt + i)->get_pos();
		POINT *p2 = (this->m_mvbl_pt + ((i + 1) % this->m_cpt))->get_pos();
		
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

void XSMovablePolygon::m_draw(XSGraphic *g, bool bin, bool bpush)
{
	POINT *apt = new POINT[this->m_cpt];
	for(int i = 0 ; i < this->m_cpt; i++)
	{
		apt[i].x = this->m_mvbl_pt[i].get_pos()->x;
		apt[i].y = this->m_mvbl_pt[i].get_pos()->y;
	}
	
	g->polygon(apt, this->m_cpt);
	
	for(int i = this->m_cpt - 1; i >= 0; i--)
	{
		this->m_mvbl_pt[i].draw(g);
	}
	
	delete[] apt;
}

bool XSMovablePolygon::m_b_in_subobject_range(int x, int y)
{
	if(this->m_blbuttonup)
	{
		this->m_blbuttonup = false;
		for(int i = 0; i < this->m_cpt; i++)
		{
			this->m_mvbl_pt[i].pop();
		}
		this->pop();
	}
	if(this->m_blbuttondown) 
	{
		m_blbuttondown = false;
		for(int i = 0; i < this->m_cpt; i++)
		{
		
			if(this->m_mvbl_pt[i].b_in_range(x, y))
			{
				this->m_mvbl_pt[i].push();
				
				return true;
				break;
			}
		}
	}
	for(int i = 0; i < this->m_cpt; i++)
	{
		if(this->m_mvbl_pt[i].b_in_range(x, y))
		{
			return true;
		}
	}
	return false;
}

void XSMovablePolygon::lbuttondown()
{
	this->m_blbuttondown = true;
}

void XSMovablePolygon::lbuttonup()
{
	this->m_blbuttonup = true;
}

void XSMovablePolygon::m_subobjects_move_to(int x, int y)
{
	for(int i = 0; i < this->m_cpt; i++)
	{
		this->m_mvbl_pt[i].move_to(x, y);
	}
	this->set_pos(x, y);
}

void XSMovablePolygon::m_move_to(int x, int y)
{
	POINT *p = this->get_pos();
	
	for(int i = 0; i < this->m_cpt; i++)
	{
		int dx = this->m_mvbl_pt[i].get_pos()->x - p->x;
		int dy = this->m_mvbl_pt[i].get_pos()->y - p->y;
		this->m_mvbl_pt[i].push();
		this->m_mvbl_pt[i].move_to(x + dx, y + dy);
	}
}
