/**
 * @file xspanimate.h 
 * @brief: 循环动画类
 * @author: XSSunet
 * @date: 2023-07-30 12:45:50
 * 
 **/
#ifndef XSANIMATE_H
#define XSANIMATE_H

#include <Windows.h>

class XSAnimatedPoint;
class XSUpdater;
class XSUpdaterLong;

class XSAnimate {
public:
	XSAnimate() = delete;
	XSAnimate(int T);			/*构造函数*/
	virtual ~XSAnimate();				/*析构函数*/
	
	void set_period(int T);					/*设置周期*/
	int get_period(int T);					/*获得周期*/
	void on_timer();						/*响应计时器*/
	virtual void on_loop(int n) = 0;		/*响应循环(每周期要干的事)*/
	
	void pause();					/*暂停动画*/
	void resume();					/*继续动画*/
	void restart();					/*重新播放动画*/
	
	bool is_paused();				/*返回是否暂停*/
private:
	int m_T; 						/*动画的周期*/
	long m_time;					/*动画执行时间*/
	bool m_b_pause;					/*是否暂停*/
	long m_nloop;					/*循环次数*/
	LARGE_INTEGER m_lasttime;		/*上一次换状态的时间*/
	long get_delay();				/*获得当前时间和上一次调用此函数的时间间隔*/
protected:
	friend class XSAnimatedPoint;
	friend class XSUpdater;
	friend class XSUpdaterLong;
};

#endif
