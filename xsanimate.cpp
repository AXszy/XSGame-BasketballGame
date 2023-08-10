/**
 * @file xsanimate.cpp 
 * @brief: 循环动画类实现
 * @author: XSSunet
 * @date: 2023-07-30 12:46:01
 * 
 **/
#include "xsanimate.h"

XSAnimate::XSAnimate(int T)
{
	this->m_T = T;
	this->m_b_pause = false;
	this->m_time = 0;
	QueryPerformanceCounter(&this->m_lasttime);
}

XSAnimate::~XSAnimate()
{
	
}

void XSAnimate::set_period(int T)
{
	this->m_T = T;
}

int XSAnimate::get_period(int T)
{
	return this->m_T;
}

long XSAnimate::get_delay()
{
	LARGE_INTEGER current_time, last_time = this->m_lasttime;
	
	//获得当前时间
	QueryPerformanceCounter(&current_time);
	
	//将上一次调用的时间设置为现在
	this->m_lasttime = current_time;
	
	//返回两次时间的差值
	return (current_time.QuadPart - last_time.QuadPart) / 1000;
}

void XSAnimate::on_timer()
{
	if(!this->m_b_pause)
	{
		this->m_time += get_delay();
		if(this->m_time >= this->m_T)
		{
			on_loop(this->m_time / this->m_T);
			this->m_time %= this->m_T;
		}
	}
	else
	{
		get_delay();	//起到将上一次调用使时间设为现在的作用
	}
}

bool XSAnimate::is_paused()
{
	return this->m_b_pause;
}

void XSAnimate::pause()
{
	this->m_b_pause = true;
}

void XSAnimate::resume()
{
	this->m_b_pause = false;
}

void XSAnimate::restart()
{
	this->m_nloop = -1;
	this->on_loop(1);
}
