/**
 * @file xsscene.h 
 * @brief: 场景类
 * @author: XSSunet
 * @date: 2023-08-01 02:26:08
 * 
 **/
#ifndef XSSCENE_H
#define XSSCENE_H

#include <windows.h>
#include "xsgraphic.h"
#include "xsanimate.h"
#include <vector>
#include <functional>

class XSScene {
public:
	XSScene();							/*构造函数*/
	~XSScene();							/*析构函数*/
	
	void set_on_timer(std::function<void(POINT)> ot);			/*响应计时器*/
	void set_on_lbuttondown(std::function<void(POINT)> olbd);	/*响应鼠标按下*/
	void set_on_lbuttonup(std::function<void(POINT)> olbu);		/*响应鼠标抬起*/
	void set_on_mousemove(std::function<void(POINT)> omm);		/*响应鼠标移动*/
	void set_on_keydown(std::function<void(WPARAM)> okd);		/*响应键盘按下*/
	void set_on_keyup(std::function<void(WPARAM)> oku);			/*响应键盘抬起*/
	void set_on_paint(std::function<void(XSGraphic*)> op);	/*响应绘图*/
	
	bool is_used();						/*场景是否在使用中*/
	void use();							/*使用*/
	void unuse();						/*废用*/
	
	void add_anim(XSAnimate *anim);		/*完全归场景操控的动画对象*/
	
	std::function<void(POINT)> on_timer;
	std::function<void(POINT)> on_lbuttondown;
	std::function<void(POINT)> on_lbuttonup;
	std::function<void(POINT)> on_mousemove;
	std::function<void(WPARAM)> on_keydown;
	std::function<void(WPARAM)> on_keyup;
	std::function<void(XSGraphic*)> on_paint;
private:
	bool m_b_used;
	std::vector<std::pair<XSAnimate*, bool>> m_anims;
};

#endif
