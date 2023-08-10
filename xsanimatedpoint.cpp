/**
 * @file xsanimatedpoint.cpp 
 * @brief: 循环动画点的实现
 * @author: XSSunet
 * @date: 2023-07-30 13:24:33
 * 
 **/
#include "xsanimatedpoint.h"

XSAnimatedPoint::XSAnimatedPoint(int x, int y, int T) : XSAnimate(T)
{
	this->m_pos.x = x;
	this->m_pos.y = y;
	this->m_nloop = -1;
	this->m_func_x = [x](long){return x;};
	this->m_func_y = [y](long){return y;};
}

XSAnimatedPoint::~XSAnimatedPoint()
{
	
}

POINT *XSAnimatedPoint::get_pos()
{
	return &this->m_pos;
}

void XSAnimatedPoint::on_loop(int n)
{
	this->m_nloop += n;
	//改变坐标
	this->m_pos.x = this->m_func_x(this->m_nloop);
	this->m_pos.y = this->m_func_y(this->m_nloop);
}

void XSAnimatedPoint::set_func_x(std::function<int(long)> func_x)
{
	this->m_func_x = func_x;
}

void XSAnimatedPoint::set_func_y(std::function<int(long)> func_y)
{
	this->m_func_y = func_y;
}

