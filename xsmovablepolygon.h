/**
 * @file xsmovablepolygon.h 
 * @brief: 可移动多边形类
 * @author: XSSunet
 * @date: 2023-07-23 10:41:28
 * 
 **/
#ifndef XSMOVABLEPOLYGON_H
#define XSMOVABLEPOLYGON_H

#include "xsmovable.h"
#include "xsmovablepoint.h"

class XSMovablePolygon : public XSMovable {
public:
	XSMovablePolygon() = delete;
	XSMovablePolygon(POINT *apt, int cpt);						/*构造函数*/
	~XSMovablePolygon();										/*析构函数*/
	void lbuttondown();											/*左键按下时通知对象*/
	void lbuttonup();											/*左键抬起时通知对象*/
private:
	XSMovablePoint *m_mvbl_pt;
	int m_cpt;
	bool m_blbuttondown, m_blbuttonup;
	
	virtual bool m_delta(int x, int y);							/*判别式*/
	virtual void m_draw(XSGraphic *g, bool bin, bool bpush);	/*绘制*/
	virtual bool m_b_in_subobject_range(int x, int y);			/*判断是否在子对象中*/
	virtual void m_subobjects_move_to(int x, int y);			/*子对象的 move_to 函数*/
	virtual void m_move_to(int x, int y);						/*移动对象到目标位置*/
};

#endif
