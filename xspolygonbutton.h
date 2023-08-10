/**
 * @file xspolygonbutton.h 
 * @brief: 多边形按钮
 * @author: XSSunet
 * @date: 2023-07-25 01:42:19
 * 
 **/
#ifndef XSPOLYGONBUTTON_H
#define XSPOLYGONBUTTON_H

#include "xsbutton.h"
#include <windows.h>
#include <functional>

class XSPolygonButton : public XSButton {
public:
	XSPolygonButton() = delete;				
	XSPolygonButton(POINT *apt, int cpt);   /*构造函数*/
	~XSPolygonButton();						/*析构函数*/
	
	void set_pen_normal(HPEN hp);			/*设置一般情况的画笔*/
	void set_pen_in(HPEN hp);				/*设置鼠标在按钮里的画笔*/
	void set_pen_pushed(HPEN hp);			/*设置按下后的画笔*/
	
	void set_brush_normal(HBRUSH hbr);			/*设置一般情况的画刷*/
	void set_brush_in(HBRUSH hbr);				/*设置鼠标在按钮里的画刷*/
	void set_brush_pushed(HBRUSH hbr);			/*设置按下后的画刷*/
	void set_function(std::function<VOID()> func);	/*设置按钮按下之后执行的代码内容*/
private:
	POINT *m_apt;
	int m_cpt;
	HPEN m_hp_normal, m_hp_in, m_hp_pushed;
	HBRUSH m_hbr_normal, m_hbr_in, m_hbr_pushed;
	std::function<VOID()> m_function;
	
	virtual bool m_delta(int x, int y);
	virtual void m_draw(XSGraphic *g, bool bin, bool bpush);
	virtual void m_func();
};

#endif
