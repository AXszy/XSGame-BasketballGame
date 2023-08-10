/**
 * @file xsscene.cpp 
 * @brief: 场景类的实现
 * @author: XSSunet
 * @date: 2023-08-01 02:29:34
 * 
 **/
#include "xsscene.h"

XSScene::XSScene()
{
	on_timer = [](POINT){};
	on_lbuttondown = [](POINT){};
	on_lbuttonup = [](POINT){};
	on_keydown = [](WPARAM){};
	on_keyup = [](WPARAM){};
	on_mousemove = [](POINT){};
	on_paint = [](XSGraphic*){};
	m_b_used = false;
}

XSScene::~XSScene()
{
	
}

void XSScene::set_on_timer(std::function<void(POINT)> ot)
{
	this->on_timer = ot;
}

void XSScene::set_on_lbuttondown(std::function<void(POINT)> olbd)
{
	this->on_lbuttondown = olbd;
}

void XSScene::set_on_lbuttonup(std::function<void(POINT)> olbu)
{
	this->on_lbuttonup = olbu;
}

void XSScene::set_on_mousemove(std::function<void(POINT)> omm)
{
	this->on_mousemove = omm;
}

void XSScene::set_on_keydown(std::function<void(WPARAM)> okd)
{
	this->on_keydown = okd;
}

void XSScene::set_on_keyup(std::function<void(WPARAM)> oku)
{
	this->on_keyup = oku;
}

void XSScene::set_on_paint(std::function<void(XSGraphic*)> op)
{
	this->on_paint = op;
}


bool XSScene::is_used()
{
	return this->m_b_used;
}

void XSScene::use()
{
	this->m_b_used = true;
	for(std::pair<XSAnimate*, bool> &p : this->m_anims)
	{
		if(!p.second) 
			p.first->resume();
	}
}

void XSScene::unuse()
{
	this->m_b_used = false;
	for(std::pair<XSAnimate*, bool> &p : this->m_anims)
	{
		p.second = p.first->is_paused();
		p.first->pause();
	}
}

void XSScene::add_anim(XSAnimate *anim)
{
	this->m_anims.push_back(std::make_pair(anim, anim->is_paused()));
}
