/**
 * @file xsmovable.h 
 * @brief: 可移动的所有对象的基类
 * @author: XSSunet
 * @date: 2023-07-16 00:59:49
 * 
 **/

#ifndef XSMOVABLE_H
#define XSMOVABLE_H

#include "xsgraphic.h"

class XSMovable {
public:
	XSMovable();								/*构造函数*/
	XSMovable(int x, int y);					/*构造函数*/
	virtual ~XSMovable();								/*析构函数*/
	
	void move_to(int x, int y);					/*移动*/
	void push();								/*按下*/
	void pop();									/*弹起*/
	bool b_in_range(int x, int y);				/*返回鼠标是否在对象内*/
	void draw(XSGraphic *g);					/*绘制*/
	POINT *get_pos();							/*获得对象坐标*/
	void set_pos(int x, int y);					/*设置对象坐标*/
private:
	POINT m_pos;
	bool b_in;
	bool b_push;
	virtual bool m_delta(int x, int y) = 0;		/*判别式*/
	virtual void m_draw(XSGraphic *g, bool bin, bool bpush) = 0;	/*绘制*/
	virtual bool m_b_in_subobject_range(int x, int y);		/*是否在子对象内*/
	virtual void m_subobjects_move_to(int x, int y);			/*调用子对象的move_to函数*/
	virtual void m_move_to(int x, int y);		/*主对象在移动时需要进行的额外操作坐标变动前)*/
};

#endif
