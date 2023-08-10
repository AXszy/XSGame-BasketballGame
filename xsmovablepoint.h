/**
 * @file xsmovablepoint.h 
 * @brief: 可移动的点类
 * @author: XSSunet
 * @date: 2023-07-16 11:27:46
 * 
 **/
#ifndef XSMOVABLEPOINT_H
#define XSMOVABLEPOINT_H

#include "xsmovable.h"

class XSMovablePoint : public XSMovable {
public:
	XSMovablePoint();									/*构造函数*/
	XSMovablePoint(int x, int y, int r);				/*构造函数*/
	~XSMovablePoint();									/*析构函数*/
private:
	int m_r;											/*半径*/
	bool m_delta(int x, int y);							/*判别式*/
	void m_draw(XSGraphic *g, bool bin, bool bpush);	/*绘制*/
};

#endif
