/**
 * @file xsbutton.h 
 * @brief: 按钮类
 * @author: XSSunet
 * @date: 2023-07-25 01:23:28
 * 
 **/
#ifndef XBUTTON_H
#define XBUTTON_H

#include "xsgraphic.h"

class XSButton {
public:
	XSButton();									/*构造函数*/
	virtual ~XSButton();						/*析构函数*/
	void push();								/*按下*/
	void pop();									/*弹起*/
	bool b_in_range(int x, int y);				/*返回鼠标是否在对象内*/
	void draw(XSGraphic *g);					/*绘制*/
private:
	bool b_in;
	bool b_push;
	virtual bool m_delta(int x, int y) = 0;		/*判别式*/
	virtual void m_draw(XSGraphic *g, bool bin, bool bpush) = 0;	/*绘制*/
	virtual void m_func() = 0;					/*按钮按下时执行的代码*/
};

#endif
