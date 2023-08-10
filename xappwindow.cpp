/**
 * @file xappwindow.cpp
 * @brief: 应用程序窗口的实现
 * @author: XSSunet
 * @date: 2023-07-09 10:32:35
 *
 **/

#include "xappwindow.h"
#include "xsgraphic.h"
#include "xsmovablepolygon.h"
#include "xspolygonbutton.h"
#include "xsanimatedpoint.h"
#include "xsmusic.h"
#include "xsscene.h"
#include "resource.h"
#include <math.h>
#include <cstdio>

#define XS_MAIN_TIMER 1

XAppWindow::XAppWindow() : XSWindow() {

}

XAppWindow::XAppWindow(HINSTANCE hInst) : XSWindow(hInst) {

}

XAppWindow::~XAppWindow() {
	//释放资源
	delete this->m_anim_ball;
	delete this->m_anim_basket;
	delete this->m_anim_strength;
	delete this->m_upd_score;
	delete this->m_upd_dec_r;
	delete this->m_upd_scoring;
	delete this->m_upd_missing;
	delete this->m_upd_getready_x;
	delete this->m_upd_getready_y;
	delete this->m_upd_super;
	delete this->m_upd_basket_x;
	delete this->m_upd_boommm;
	delete this->m_but_restart;
	delete this->m_but_return_main_menu;
	delete this->m_but_start;
	
	//释放GDI资源
	DeleteObject(this->m_hbr_ball_normal);
	DeleteObject(this->m_hbr_bkground_normal);
	DeleteObject(this->m_hbr_floor_normal);
	DeleteObject(this->m_hbr_ball_bt);
	DeleteObject(this->m_hbr_bkground_bt);
	DeleteObject(this->m_hbr_floor_bt);
	DeleteObject(this->m_hbr_ball_super);
	DeleteObject(this->m_hbr_bkground_super);
	DeleteObject(this->m_hbr_floor_super);
	DeleteObject(this->m_hbr_score_bk);
	DeleteObject(this->m_hbr_score_fg);
	DeleteObject(this->m_hbr_restart_button_normal);
	
	DeleteObject(this->m_hbm_normal_basket);
	DeleteObject(this->m_hbm_bt_basket);
	DeleteObject(this->m_hbm_super_basket);
	DeleteObject(this->m_hbm_basket_mask);
	DeleteObject(this->m_hbm_life);
	DeleteObject(this->m_hbm_life_mask);
	DeleteObject(this->m_hbm_replay);
	DeleteObject(this->m_hbm_replay_mask);
	DeleteObject(this->m_hbm_qqhead);
	
	DeleteObject(this->m_hf_game);
	DeleteObject(this->m_hf_combo);
	DeleteObject(this->m_hf_title);
	
	this->release();
}

int XAppWindow::handle_message(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp) {

	switch (wm) {
		case WM_CREATE: {
			this->on_create();
			SetWindowText(hwnd, "投篮游戏");			/*设置窗口标题*/
			SetTimer(hwnd, XS_MAIN_TIMER, 5, NULL);		/*设置计时器*/
			break;
		}
		case WM_PAINT: {
			XSGraphic *g = new XSGraphic(hwnd);
			
			this->m_sc_main_menu.on_paint(g);
			this->m_sc_play.on_paint(g);
			this->m_sc_play_bt.on_paint(g);
			this->m_sc_end.on_paint(g);

			delete g;
			break;
		}
		case WM_MOUSEMOVE: {
			POINT p_cur;
			this->get_cursor_pos(this->getHWND(), &p_cur);

			this->m_sc_main_menu.on_mousemove(p_cur);
			this->m_sc_play.on_mousemove(p_cur);
			this->m_sc_play_bt.on_mousemove(p_cur);
			this->m_sc_end.on_mousemove(p_cur);
			
			break;
		}
		case WM_LBUTTONDOWN: {
			POINT p_cur;
			this->get_cursor_pos(this->getHWND(), &p_cur);

			this->m_sc_main_menu.on_lbuttondown(p_cur);
			this->m_sc_play.on_lbuttondown(p_cur);
			this->m_sc_play_bt.on_lbuttondown(p_cur);
			this->m_sc_end.on_lbuttondown(p_cur);

			InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
		case WM_LBUTTONUP: {
			POINT p_cur;
			this->get_cursor_pos(this->getHWND(), &p_cur);

			this->m_sc_main_menu.on_lbuttonup(p_cur);
			this->m_sc_play.on_lbuttonup(p_cur);
			this->m_sc_play_bt.on_lbuttonup(p_cur);
			this->m_sc_end.on_lbuttonup(p_cur);
			
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
		case WM_TIMER: {
			switch (wp) {
				case XS_MAIN_TIMER: {
					InvalidateRect(hwnd, NULL, FALSE);
					POINT p_cur;
					this->get_cursor_pos(this->getHWND(), &p_cur);

					this->m_sc_main_menu.on_timer(p_cur);
					this->m_sc_play.on_timer(p_cur);
					this->m_sc_play_bt.on_timer(p_cur);
					this->m_sc_end.on_timer(p_cur);
					
					break;
				}
			}
			break;
		}
		case WM_KEYDOWN: {
			this->m_sc_main_menu.on_keydown(wp);
			this->m_sc_play.on_keydown(wp);
			this->m_sc_play_bt.on_keydown(wp);
			this->m_sc_end.on_keydown(wp);
			break;
		}
		case WM_KEYUP: {
			this->m_sc_main_menu.on_keyup(wp);
			this->m_sc_play.on_keyup(wp);
			this->m_sc_play_bt.on_keyup(wp);
			this->m_sc_end.on_keyup(wp);
			break;
		}
		default: {
			return 0;
			break;
		}
	}
	return 1;
}

void XAppWindow::get_cursor_pos(HWND hwnd, POINT *p) {
	GetCursorPos(p);
	ScreenToClient(hwnd, p);

	//获得客户区宽度和高度
	RECT rcClient;
	GetClientRect(hwnd, &rcClient);
	int client_w = rcClient.right - rcClient.left;
	int client_h = rcClient.bottom - rcClient.top;

	//计算实际坐标
	if (client_w > 0 && client_h > 0) {
		p->x = p->x * XSGRAPHIC_COMPATIBLEBMP_W / client_w;
		p->y = p->y * XSGRAPHIC_COMPATIBLEBMP_H / client_h;
	} else {
		p->x = p->y = 0;
	}
}

void XAppWindow::on_create() {
	this->m_basket_speed = 1;
	this->m_launch_point = {XSGRAPHIC_COMPATIBLEBMP_W / 2, 350};
	this->m_basket_r = 80;
	this->m_combo = 0;
	this->m_n_super = 1;
	this->m_score = 0;
	this->m_strength = 20;
	this->m_angle = 0;
	this->m_bspacedown = false;
	this->m_bscoring = false;
	this->m_b_decreasing_r = false;
	this->m_bflying = false;
	this->m_initial_pos_ball = this->m_launch_point;
	this->m_initial_v_ball = {0, 0};
	this->m_k_scoring = 0;
	this->m_k_missing = 0;
	this->m_bscored = false;
	this->m_bscored_lastround = false;
	this->m_b_ready = true;
	this->m_b_pressed_s = false;
	this->m_bt_played = true;
	this->m_n_scored_ball = 0;
	this->m_bsuper = false;
	this->m_n_life = 15;
	this->m_b_endwith_bt = false;
	this->m_k_boom = 0;
	this->m_qqhead_angle = 0;
	
	this->m_hf_game = CreateFont(
		XSGRAPHIC_COMPATIBLEBMP_H / 10, 0, 0, 0, 400, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY,
		FF_DONTCARE,
		"Cambria"
		);
	
	this->m_hf_title = CreateFont(
		XSGRAPHIC_COMPATIBLEBMP_H / 7, 0, 0, 0, 400, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY,
		FF_DONTCARE,
		"Cambria"
		);
	
	this->m_hf_combo = CreateFont(
		XSGRAPHIC_COMPATIBLEBMP_H / 13, 0, 0, 0, 400, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY,
		FF_DONTCARE,
		"Segoe Print"
		);
	
	this->m_hbr_ball_normal = CreateSolidBrush(RGB(175, 92, 18));
	this->m_hbr_bkground_normal = CreateSolidBrush(RGB(164, 242, 242));
	this->m_hbr_floor_normal = CreateSolidBrush(RGB(83, 41, 0));
	this->m_hbr_ball_bt = CreateSolidBrush(RGB(155, 53, 51));
	this->m_hbr_bkground_bt = CreateSolidBrush(RGB(127, 127, 127));
	this->m_hbr_floor_bt = CreateSolidBrush(RGB(66, 66, 66));
	this->m_hbr_ball_super = CreateSolidBrush(RGB(248, 240, 150));
	this->m_hbr_bkground_super = CreateSolidBrush(RGB(255, 247, 89));
	this->m_hbr_floor_super = CreateSolidBrush(RGB(251, 210, 47));
	this->m_hbr_score_bk = CreateSolidBrush(RGB(244, 244, 244));
	this->m_hbr_score_fg = CreateSolidBrush(RGB(200, 200, 200));
	this->m_hbr_restart_button_normal = CreateSolidBrush(RGB(153, 217, 234));
	
	//获得模块句柄
	HINSTANCE hinst_module = GetModuleHandle(NULL);
	
	//加载图片
	this->m_hbm_normal_basket = (HBITMAP)LoadImage(hinst_module, 
		MAKEINTRESOURCE(IDB_NORMAL_BASKET), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	this->m_hbm_bt_basket = (HBITMAP)LoadImage(hinst_module, 
		MAKEINTRESOURCE(IDB_BT_BASKET), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	this->m_hbm_super_basket = (HBITMAP)LoadImage(hinst_module, 
		MAKEINTRESOURCE(IDB_SUPER_BASKET), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	this->m_hbm_basket_mask = (HBITMAP)LoadImage(hinst_module, 
		MAKEINTRESOURCE(IDB_BASKET_MASK), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	this->m_hbm_life = (HBITMAP)LoadImage(hinst_module, 
		MAKEINTRESOURCE(IDB_LIFE), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	this->m_hbm_life_mask = (HBITMAP)LoadImage(hinst_module, 
		MAKEINTRESOURCE(IDB_LIFE_MASK), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	this->m_hbm_replay = (HBITMAP)LoadImage(hinst_module, 
		MAKEINTRESOURCE(IDB_REPLAY), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	this->m_hbm_replay_mask = (HBITMAP)LoadImage(hinst_module, 
		MAKEINTRESOURCE(IDB_REPLAY_MASK), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	this->m_hbm_qqhead = (HBITMAP)LoadImage(hinst_module, 
		MAKEINTRESOURCE(IDB_QQHEAD), IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
	
	//小球
	this->m_anim_ball = new XSAnimatedPoint(this->m_initial_pos_ball.x, 
		this->m_initial_pos_ball.y, 10);
	this->m_anim_ball->set_func_x([this](long t) {
		return this->m_initial_pos_ball.x + this->m_initial_v_ball.x * t / 100.0;
	});
	this->m_anim_ball->set_func_y([this](long t) {
		return this->m_initial_pos_ball.y + t * t / 3000.0 + 
			this->m_initial_v_ball.y * t / 100.0;
	});
	this->m_anim_ball->pause();

	//篮筐
	this->m_anim_basket = new XSAnimatedPoint(200, 120, 10);
	this->m_anim_basket->set_func_x([this](int t) {
		return XSGRAPHIC_COMPATIBLEBMP_W / 2 * (1 + sin(t / 1000.0 * this->m_basket_speed))
		       - this->m_basket_r * sin(t / 1000.0 * this->m_basket_speed);
	});

	//力度
	this->m_anim_strength = new XSAnimatedPoint(400, 350, 10);
	this->m_anim_strength->set_func_x([this](long t) {
		static long t_last = 0;
		if (this->m_bspacedown) {
			this->m_strength = (t - t_last) / 30 % 40 + 50;
			if (this->m_strength >= 70)
				this->m_strength = 70;
		} else t_last = t;
		return this->m_anim_ball->get_pos()->x + 30 * cos(this->m_angle);
	});
	this->m_anim_strength->set_func_y([this](long t) {
		return this->m_anim_ball->get_pos()->y + 30 * sin(this->m_angle);
	});

	//更新器
	this->m_upd_score = new XSUpdater(&this->m_score, 2, 2000);
	this->m_upd_dec_r = new XSUpdater(&this->m_basket_r, -0.1 * this->m_basket_r, 2000);
	this->m_upd_scoring = new XSUpdater(&this->m_k_scoring, 1, 3000);
	this->m_upd_missing = new XSUpdater(&this->m_k_missing, 1, 3000);
	this->m_upd_getready_x = new XSUpdaterLong(&this->m_anim_ball->get_pos()->x, 0, 3000);
	this->m_upd_getready_y = new XSUpdaterLong(&this->m_anim_ball->get_pos()->y, 0, 3000);
	this->m_upd_super = new XSUpdater(&this->m_k_super, 1, 10000);
	this->m_upd_basket_x = new XSUpdaterLong(&this->m_anim_basket->get_pos()->x, 0, 3000);
	this->m_upd_boommm = new XSUpdater(&this->m_k_boom, 1, 20000);
	
	//主界面的开始按钮
	POINT apt_but_start[4] = {
		{XSGRAPHIC_COMPATIBLEBMP_W / 2 - 160, XSGRAPHIC_COMPATIBLEBMP_H / 2 - 30},
		{XSGRAPHIC_COMPATIBLEBMP_W / 2 + 160, XSGRAPHIC_COMPATIBLEBMP_H / 2 - 30},
		{XSGRAPHIC_COMPATIBLEBMP_W / 2 + 160, XSGRAPHIC_COMPATIBLEBMP_H / 2 + 50},
		{XSGRAPHIC_COMPATIBLEBMP_W / 2 - 160, XSGRAPHIC_COMPATIBLEBMP_H / 2 + 50}	
	};
	this->m_but_start = new XSPolygonButton(apt_but_start, 4);
	this->m_but_start->set_function([this](){
		this->m_init_game();
		this->m_sc_main_menu.unuse();
		this->m_sc_play.use();
	});
	
	//返回主菜单按钮
	POINT apt_but_return_main_menu[4] = {
		{XSGRAPHIC_COMPATIBLEBMP_W / 2 - 80, XSGRAPHIC_COMPATIBLEBMP_H / 2 + 40},
		{XSGRAPHIC_COMPATIBLEBMP_W / 2 + 80, XSGRAPHIC_COMPATIBLEBMP_H / 2 + 40},
		{XSGRAPHIC_COMPATIBLEBMP_W / 2 + 80, XSGRAPHIC_COMPATIBLEBMP_H / 2 + 80},
		{XSGRAPHIC_COMPATIBLEBMP_W / 2 - 80, XSGRAPHIC_COMPATIBLEBMP_H / 2 + 80}	
	};
	this->m_but_return_main_menu = new XSPolygonButton(apt_but_return_main_menu, 4);
	this->m_but_return_main_menu->set_function([this](){
		this->m_sc_end.unuse();
		this->m_sc_main_menu.use();
	});
	
	//重新开始游戏的按钮
	POINT apt_but_restart[4] = {
		{XSGRAPHIC_COMPATIBLEBMP_W / 2 - 50, XSGRAPHIC_COMPATIBLEBMP_H / 2 - 60},
		{XSGRAPHIC_COMPATIBLEBMP_W / 2 + 50, XSGRAPHIC_COMPATIBLEBMP_H / 2 - 60},
		{XSGRAPHIC_COMPATIBLEBMP_W / 2 + 50, XSGRAPHIC_COMPATIBLEBMP_H / 2 + 20},
		{XSGRAPHIC_COMPATIBLEBMP_W / 2 - 50, XSGRAPHIC_COMPATIBLEBMP_H / 2 + 20}
	};
	this->m_but_restart = new XSPolygonButton(apt_but_restart, 4);
	this->m_but_restart->set_function([this](){
		this->m_init_game();
		this->m_sc_end.unuse();
		this->m_sc_play.use();
	});
	this->m_but_restart->set_brush_normal(this->m_hbr_restart_button_normal);
	
	//设置主菜单场景
	{
		this->m_sc_main_menu = XSScene();
		this->m_sc_main_menu.set_on_paint([this, apt_but_start](XSGraphic *g){
			if(this->m_sc_main_menu.is_used())
			{
				//绘制背景
				if(this->m_b_endwith_bt)
					g->select(this->m_hbr_bkground_bt);
				else
					g->select(this->m_hbr_bkground_normal);
				g->rectangle(0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
				
				
				//绘制地板
				if(this->m_b_endwith_bt)
					g->select(this->m_hbr_floor_bt);
				else
					g->select(this->m_hbr_floor_normal);
				g->rectangle(0, 400, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
				
				//绘制我的qq头像
				POINT lt, rt, ld;
				lt.x = 260 - 40 * cos(this->m_qqhead_angle);
				lt.y = 150 + 40 * sin(this->m_qqhead_angle);
				rt.x = 260 + 40 * sin(this->m_qqhead_angle);
				rt.y = 150 + 40 * cos(this->m_qqhead_angle);
				ld.x = 260 - 40 * sin(this->m_qqhead_angle);
				ld.y = 150 - 40 * cos(this->m_qqhead_angle);
				g->plg_blt(this->m_hbm_qqhead, RGB(255, 255, 255), 
					lt, rt, ld);
				
				//绘制标题
				g->select(this->m_hf_title);
				TCHAR szTitle[] = TEXT("投篮游戏");
				g->draw_text(0, 0, XSGRAPHIC_COMPATIBLEBMP_W, 170, 
					szTitle, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
				g->select(this->m_hf_combo);
				TCHAR szSubTitle[] = TEXT("By  故宫的落日.");
				g->draw_text(300, 110, 500, 190, szSubTitle, 
					DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				
				//绘制按钮
				g->select(this->m_hf_game);
				this->m_but_start->draw(g);
				TCHAR szText[] =TEXT("开始游戏");
				g->draw_text(apt_but_start[0].x, apt_but_start[0].y,
					apt_but_start[2].x, apt_but_start[2].y, szText, 
					DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			}
		});
		this->m_sc_main_menu.set_on_lbuttondown([this](POINT p_cur){
			if(this->m_sc_main_menu.is_used())
			{
				if(this->m_but_start->b_in_range(p_cur.x, p_cur.y))
					this->m_but_start->push();
			}
		});
		this->m_sc_main_menu.set_on_lbuttonup([this](POINT p_cur){
			if(this->m_sc_main_menu.is_used())
			{
				this->m_but_start->b_in_range(p_cur.x, p_cur.y);
				this->m_but_start->pop();
			}
		});
		this->m_sc_main_menu.set_on_mousemove([this](POINT p_cur){
			if(this->m_sc_main_menu.is_used())
			{
				this->m_but_start->b_in_range(p_cur.x, p_cur.y);
			}
		});		
		this->m_sc_main_menu.set_on_timer([this](POINT p_cur){
			if(this->m_sc_main_menu.is_used())
				m_qqhead_angle += 0.02;	
		});
	}
	
	//设置投篮场景
	{
		this->m_sc_play = XSScene();
		this->m_sc_play.set_on_paint([this](XSGraphic * g) {
			if(this->m_sc_play.is_used())
			{
				//获得动画点坐标
				POINT *p_ball = this->m_anim_ball->get_pos();
				POINT *p_basket = this->m_anim_basket->get_pos();
				POINT *p_strength = this->m_anim_strength->get_pos();
				
				//大招下的界面
				if(this->m_bsuper)
				{
					//绘制背景
					g->select(this->m_hbr_bkground_super);
					g->rectangle(0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
					
					//绘制框上半部分
					g->draw_image_part(
						this->m_hbm_super_basket,
						this->m_hbm_basket_mask, 
						0, 0, 1, 0.5,
						p_basket->x - this->m_basket_r,
						p_basket->y - 16,
						2 * this->m_basket_r,
						16
						);
					
					//绘制球
					g->select(this->m_hbr_ball_super);
					g->ellipse(p_ball->x - 14, p_ball->y - 14,
						p_ball->x + 14, p_ball->y + 14);
					
					if (this->m_bspacedown && this->m_b_ready) {
						g->ellipse(p_strength->x - 5, p_strength->y - 5,
							p_strength->x + 5, p_strength->y + 5);
						
						//力度条
						g->rectangle(p_ball->x - 40, p_ball->y + 9,
							p_ball->x - 40 + (this->m_strength - 50) * 4, p_ball->y + 19);
					}
					
					//绘制框下半部分
					g->draw_image_part(
						this->m_hbm_super_basket,
						this->m_hbm_basket_mask, 
						0, 0.5, 1, 0.5,
						p_basket->x - this->m_basket_r,
						p_basket->y,
						2 * this->m_basket_r,
						16
						);
					
					//绘制地板
					g->select(this->m_hbr_floor_super);
					g->rectangle(0, 400, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
				}
				//普通模式的界面
				else
				{
					//绘制背景
					g->select(this->m_hbr_bkground_normal);
					g->rectangle(0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
					
					//绘制框上半部分
					g->draw_image_part(
						this->m_hbm_normal_basket,
						this->m_hbm_basket_mask, 
						0, 0, 1, 0.5,
						p_basket->x - this->m_basket_r,
						p_basket->y - 16,
						2 * this->m_basket_r,
						16
						);
					
					//绘制球
					g->select(this->m_hbr_ball_normal);
					g->ellipse(p_ball->x - 14, p_ball->y - 14,
						p_ball->x + 14, p_ball->y + 14);
					
					if (this->m_bspacedown && this->m_b_ready) {
						g->ellipse(p_strength->x - 5, p_strength->y - 5,
							p_strength->x + 5, p_strength->y + 5);
						
						//力度条
						g->rectangle(p_ball->x - 40, p_ball->y + 9,
							p_ball->x - 40 + (this->m_strength - 50) * 4, p_ball->y + 19);
					}
					
					//绘制框下半部分
					g->draw_image_part(
						this->m_hbm_normal_basket,
						this->m_hbm_basket_mask, 
						0, 0.5, 1, 0.5,
						p_basket->x - this->m_basket_r,
						p_basket->y,
						2 * this->m_basket_r,
						16
						);
					
					//绘制地板
					g->select(this->m_hbr_floor_normal);
					g->rectangle(0, 400, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
				}
				
				//显示分数
				g->select(this->m_hf_game);
				g->select(this->m_hbr_score_bk);
				g->rectangle(5, 5, 115, 55);
				g->select(this->m_hbr_score_fg);
				g->rectangle(10, 10, 110, 50);
				TCHAR szScore[260];
				wsprintf(szScore, TEXT("%d"), int(this->m_score));
				g->draw_text(10, 10, 110, 50, szScore, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				
				//显示连击
				g->select(this->m_hf_combo);
				TCHAR szCombo[260];
				wsprintf(szCombo, TEXT("COMBO %d"), int(this->m_combo));
				g->draw_text(10, 60, 130, 100, szCombo, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				
				//显示大招
				for(int i = 0; i < this->m_n_super; i++)
				{
					TRIVERTEX vtx[2];
					vtx[0].x = 10;		
					vtx[0].y = 110 + 35 * i;  
					vtx[0].Alpha = 0;  
					vtx[0].Red = 0x0004;  
					vtx[0].Green = 0x0017;  
					vtx[0].Blue = 0xff00;  
					vtx[1].x = 40;  
					vtx[1].y = 140 + 35 * i;
					vtx[1].Alpha = 0;  
					vtx[1].Red = 0xfac0;  
					vtx[1].Green = 0xcccc;  
					vtx[1].Blue = 0xcef0;
					
					GRADIENT_RECT grc[1];
					grc[0].UpperLeft = 0;
					grc[0].LowerRight = 1;
					
					g->gradient_fill(vtx, 2, grc, 1, GRADIENT_FILL_RECT_V);
				}
				
				//显示生命
				for(int i = 0; i < this->m_n_life; i++)
				{
					g->draw_image(this->m_hbm_life, this->m_hbm_life_mask,
						180 + 10 * i, 10, 28, 28);
				}
				
				//绘制进球动画
				g->rectangle(
					XSGRAPHIC_COMPATIBLEBMP_W / 2 - 60 * this->m_k_scoring,
					XSGRAPHIC_COMPATIBLEBMP_H / 2 - 40 * this->m_k_scoring,
					XSGRAPHIC_COMPATIBLEBMP_W / 2 + 60 * this->m_k_scoring,
					XSGRAPHIC_COMPATIBLEBMP_H / 2 + 40 * this->m_k_scoring
					);
				
				//绘制不进球动画
				g->ellipse(
					XSGRAPHIC_COMPATIBLEBMP_W / 2 - 60 * this->m_k_missing,
					XSGRAPHIC_COMPATIBLEBMP_H / 2 - 40 * this->m_k_missing,
					XSGRAPHIC_COMPATIBLEBMP_W / 2 + 60 * this->m_k_missing,
					XSGRAPHIC_COMPATIBLEBMP_H / 2 + 40 * this->m_k_missing
					);
				
				//绘制大招动画
				TRIVERTEX vtx[2];
				vtx[0].x = XSGRAPHIC_COMPATIBLEBMP_W / 2 - 60 * this->m_k_super;		
				vtx[0].y = XSGRAPHIC_COMPATIBLEBMP_H / 2 - 40 * this->m_k_super;  
				vtx[0].Alpha = 0;  
				vtx[0].Red = 0x0004;  
				vtx[0].Green = 0x0017;  
				vtx[0].Blue = 0xff00;  
				vtx[1].x = XSGRAPHIC_COMPATIBLEBMP_W / 2 + 60 * this->m_k_super;  
				vtx[1].y = XSGRAPHIC_COMPATIBLEBMP_H / 2 + 40 * this->m_k_super;  
				vtx[1].Alpha = 0;  
				vtx[1].Red = 0xfac0;  
				vtx[1].Green = 0xcccc;  
				vtx[1].Blue = 0xcef0;
				
				GRADIENT_RECT grc[1];
				grc[0].UpperLeft = 0;
				grc[0].LowerRight = 1;
				
				g->gradient_fill(vtx, 2, grc, 1, GRADIENT_FILL_RECT_V);
			}
		});
		this->m_sc_play.set_on_timer([this](POINT p_cur) {
			if(this->m_sc_play.is_used())
			{
				this->m_anim_ball->on_timer();
				this->m_anim_basket->on_timer();
				this->m_anim_strength->on_timer();
				this->m_upd_score->on_timer();
				this->m_upd_dec_r->on_timer();
				this->m_upd_scoring->on_timer();
				this->m_upd_missing->on_timer();
				this->m_upd_getready_x->on_timer();
				this->m_upd_getready_y->on_timer();
				this->m_upd_super->on_timer();
				this->m_upd_basket_x->on_timer();
				POINT *p_ball = this->m_anim_ball->get_pos();
				POINT *p_basket = this->m_anim_basket->get_pos();
				//更新角度
				if (this->m_bspacedown) {
					if (p_cur.y == this->m_anim_ball->get_pos()->x)
						this->m_angle = M_PI / 2;
					else
						this->m_angle = atan2((p_cur.y - p_ball->y),
							(p_cur.x - p_ball->x));
				}
				//小球的出界判断(回合结束)
				if (p_ball->y > 400 || p_ball->x < 0 || p_ball->x > XSGRAPHIC_COMPATIBLEBMP_W) {
					this->m_anim_ball->set_func_x([p_ball, this](long t) {
						if(this->m_bflying)
						{
							this->m_bflying = false;
							//大招的结束
							if(this->m_bsuper)
							{
								this->m_bsuper = false;
								this->m_anim_basket->restart();
								this->m_anim_basket->resume();
								this->m_k_super = 0;
								this->m_n_super --;
							}
							//如果本回合没得分, 也就是没投中
							if(!this->m_bscored)
							{
								//打断连击
								this->m_combo = 0;
								//执行没投中动画
								this->m_upd_missing->set_initial_num(0);
								this->m_upd_missing->resume();
							}
							else
							{
								//连击
								this->m_combo ++;
								//三个连击换取一个大招
								if(this->m_combo >= 3)
								{
									this->m_combo -= 3;
									this->m_n_super++;
								}
							}
						}
						if (p_ball->x < 0)
							return 0;
						if (p_ball->x > XSGRAPHIC_COMPATIBLEBMP_W)
							return XSGRAPHIC_COMPATIBLEBMP_W;
						return (int)p_ball->x;
					});
					if (p_ball->y > 400)
						this->m_anim_ball->set_func_y([p_ball, this](long t) {
							if(this->m_bflying)
							{
								this->m_bflying = false;
								//大招的结束
								if(this->m_bsuper)
								{
									this->m_bsuper = false;
									this->m_anim_basket->restart();
									this->m_anim_basket->resume();
									this->m_k_super = 0;
									this->m_n_super --;
								}
								//如果本回合没得分, 也就是没投中
								if(!this->m_bscored)
								{
									//打断连击
									this->m_combo = 0;
									//执行没投中动画
									this->m_upd_missing->set_initial_num(0);
									this->m_upd_missing->resume();
								}
								else
								{
									//连击
									this->m_combo ++;
									//三个连击换取一个大招
									if(this->m_combo >= 3)
									{
										this->m_combo -= 3;
										this->m_n_super++;
									}
								}
								this->m_bscored_lastround = this->m_bscored;
							}
							if(t > 2000)
							{
								//使小球归位
								this->m_anim_ball->pause();
								this->m_upd_getready_x->set_initial_num(this->m_anim_ball->get_pos()->x);
								this->m_upd_getready_x->set_delta_num(
									this->m_launch_point.x - this->m_anim_ball->get_pos()->x);
								this->m_upd_getready_x->resume();
								
								this->m_upd_getready_y->set_initial_num(this->m_anim_ball->get_pos()->y);
								this->m_upd_getready_y->set_delta_num(
									this->m_launch_point.y - this->m_anim_ball->get_pos()->y);
								this->m_upd_getready_y->resume();
							}
							return 400;
						});
				}
				//小球的进球判断
				//如果从底下上去不能得分
				
				if (p_ball->x >= p_basket->x - this->m_basket_r - 1
					&& p_ball->x <= p_basket->x + this->m_basket_r + 1
					&& p_ball->y <= p_basket->y + 16
					&& p_ball->y >= p_basket->y) {
					if(this->m_bscoring)
					{
						this->m_bscoring = false;
						double theta = atan2(
							p_ball->y - p_basket->y, p_ball->x - p_basket->x);
						this->m_initial_v_ball.x =  20 * cos(theta);
						this->m_initial_v_ball.y =  20 * sin(theta);
						this->m_initial_pos_ball.x = p_ball->x;
						this->m_initial_pos_ball.y = p_ball->y;
						this->m_anim_ball->restart();
					}
				}
				//在得分区可以得分
				if (abs(p_ball->x - p_basket->x) < this->m_basket_r * 4 / 5 && abs(p_ball->y - p_basket->y) < 10) {
					if (this->m_bscoring) {
						this->m_bscoring = false;
						this->m_n_scored_ball ++;
						if(m_n_scored_ball % 6 == 5)
							this->m_bt_played = false;
						this->m_upd_score->set_delta_num(2);
						this->m_upd_score->resume();
						if(this->m_n_scored_ball % 5 == 0)
							this->m_b_decreasing_r = true;
						
						this->m_bscored = true;
						this->m_upd_scoring->set_initial_num(0);
						this->m_upd_scoring->resume();
					}
				}
				if (this->m_b_decreasing_r) {
					this->m_b_decreasing_r = false;
					this->m_upd_dec_r->set_delta_num(-0.1 * this->m_basket_r);
					this->m_upd_dec_r->resume();
				}
				//在发射点时, 设置为已经准备好发射了
				if(abs(p_ball->x - this->m_launch_point.x) < 5
					&& abs(p_ball->y - this->m_launch_point.y) < 5
					&& !this->m_bflying
					&& !this->m_b_ready)
				{
					this->m_b_ready = true;
				}
				//按下空格键时
				if(this->m_bspacedown)
				{
					if (!this->m_bflying) {
						if(this->m_b_ready)
						{
							this->m_initial_pos_ball = this->m_launch_point;
							this->m_anim_ball->set_func_x([this](long t) {
								return this->m_initial_pos_ball.x + this->m_initial_v_ball.x * t / 100.0;
							});
							this->m_anim_ball->set_func_y([this](long t) {
								return this->m_initial_pos_ball.y + t * t / 3000.0 + 
								this->m_initial_v_ball.y * t / 100.0;
							});
							this->m_anim_ball->restart();
							this->m_anim_ball->pause();
							this->m_bscoring = true;
							this->m_bscored = false;
						}
					}
				}
				//按下大招键 'S' 时
				if(this->m_b_pressed_s)
				{
					if(this->m_n_super > 0)
					{
						if (!this->m_bflying) {
							if(this->m_b_ready)
							{
								this->m_initial_pos_ball = this->m_launch_point;
								this->m_anim_ball->set_func_x([this](long t) {
									return this->m_initial_pos_ball.x + this->m_initial_v_ball.x * t / 100.0;
								});
								this->m_anim_ball->set_func_y([this](long t) {
									return this->m_initial_pos_ball.y + t * t / 3000.0 + 
									this->m_initial_v_ball.y * t / 100.0;
								});
								this->m_anim_ball->restart();
								this->m_anim_ball->pause();
								this->m_bscoring = true;
								this->m_bscored = false;
							}
						}
					}
				}
				if(this->m_k_scoring == 1)
				{
					this->m_k_scoring = 0;
				}
				if(this->m_k_missing == 1)
				{
					this->m_k_missing = 0;
					this->m_n_life--;
					if(this->m_n_life <= 0)
					{
						this->m_b_endwith_bt = false;
						this->m_sc_end.use();
						this->m_sc_play.unuse();
						this->m_upd_boommm->set_initial_num(0);
						this->m_upd_boommm->resume();
					}
				}
				if(!this->m_bt_played && this->m_b_ready)
				{
					this->m_sc_play.unuse();
					this->m_sc_play_bt.use();
					this->m_anim_basket->set_func_x([](long t){
						return XSGRAPHIC_COMPATIBLEBMP_W / 4 + 
							rand() % (XSGRAPHIC_COMPATIBLEBMP_W / 2);
					});
				}
			}
		});
		this->m_sc_play.set_on_keydown([this](WPARAM key) {
			if(this->m_sc_play.is_used())
			{
				switch (key) {
					case VK_SPACE: {
						this->m_bspacedown = true;
						break;
					}
					case 'S' : {
						this->m_b_pressed_s = true;
						break;
					} 
				}
			}
		});
		this->m_sc_play.set_on_keyup([this](WPARAM key) {
			if(this->m_sc_play.is_used())
			{
				switch (key) {
					case VK_SPACE: {
						this->m_bspacedown = false;
						if (!this->m_bflying) {
							if(this->m_b_ready)
							{
								this->m_bflying = true;
								this->m_b_ready = false;
								this->m_initial_v_ball = {
									(LONG)(this->m_strength * cos(this->m_angle)),
									(LONG)(this->m_strength * sin(this->m_angle))
								};
								this->m_anim_ball->resume();
							}
						}
						break;
					}
					case 'S': {
						this->m_b_pressed_s = false;
						if(this->m_n_super > 0)
						{
							if(this->m_b_ready)
							{
								this->m_bflying = true;
								this->m_b_ready = false;
								this->m_bsuper = true;
								this->m_initial_v_ball = {
									(LONG)(70 * cos( - M_PI * 0.4)),
									(LONG)(70 * sin( - M_PI * 0.4))
								};
								this->m_anim_ball->resume();
								
								//将篮筐移动到指定位置
								this->m_anim_basket->pause();
								this->m_anim_basket->set_func_x([this](int t) {
									return XSGRAPHIC_COMPATIBLEBMP_W / 2 * (1 + cos(t / 1000.0 * this->m_basket_speed))
									- this->m_basket_r * cos(t / 1000.0 * this->m_basket_speed);
								});
								
								this->m_upd_basket_x->set_initial_num(this->m_anim_basket->get_pos()->x);
								this->m_upd_basket_x->set_delta_num(
									XSGRAPHIC_COMPATIBLEBMP_W - this->m_basket_r - this->m_anim_basket->get_pos()->x);
								this->m_upd_basket_x->resume();
								
								//启动更新器
								this->m_upd_super->set_initial_num(0);
								this->m_upd_super->resume();
							}
						}
						break;
					}
				}
			}
		});
		
	}
	
	//设置变态场景
	{
		this->m_sc_play_bt = XSScene();
		this->m_sc_play_bt.set_on_paint([this](XSGraphic * g) {
			if(this->m_sc_play_bt.is_used())
			{
				//获得动画点坐标
				POINT *p_ball = this->m_anim_ball->get_pos();
				POINT *p_basket = this->m_anim_basket->get_pos();
				POINT *p_strength = this->m_anim_strength->get_pos();
				
				//大招下的界面
				if(this->m_bsuper)
				{
					//绘制背景
					g->select(this->m_hbr_bkground_super);
					g->rectangle(0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
					
					//绘制框上半部分
					g->draw_image_part(
						this->m_hbm_super_basket,
						this->m_hbm_basket_mask, 
						0, 0, 1, 0.5,
						p_basket->x - this->m_basket_r,
						p_basket->y - 16,
						2 * this->m_basket_r,
						16
						);
					
					//绘制球
					g->select(this->m_hbr_ball_super);
					g->ellipse(p_ball->x - 14, p_ball->y - 14,
						p_ball->x + 14, p_ball->y + 14);
					
					if (this->m_bspacedown && this->m_b_ready) {
						g->ellipse(p_strength->x - 5, p_strength->y - 5,
							p_strength->x + 5, p_strength->y + 5);
						
						//力度条
						g->rectangle(p_ball->x - 40, p_ball->y + 9,
							p_ball->x - 40 + (this->m_strength - 50) * 4, p_ball->y + 19);
					}
					
					//绘制框下半部分
					g->draw_image_part(
						this->m_hbm_super_basket,
						this->m_hbm_basket_mask, 
						0, 0.5, 1, 0.5,
						p_basket->x - this->m_basket_r,
						p_basket->y,
						2 * this->m_basket_r,
						16
						);
					
					//绘制地板
					g->select(this->m_hbr_floor_super);
					g->rectangle(0, 400, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
				}
				//变态模式的界面
				else
				{
					//绘制背景
					g->select(this->m_hbr_bkground_bt);
					g->rectangle(0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
					
					//绘制框上半部分
					g->draw_image_part(
						this->m_hbm_bt_basket,
						this->m_hbm_basket_mask, 
						0, 0, 1, 0.5,
						p_basket->x - this->m_basket_r,
						p_basket->y - 16,
						2 * this->m_basket_r,
						16
						);
					
					//绘制球
					g->select(this->m_hbr_ball_bt);
					g->ellipse(p_ball->x - 14, p_ball->y - 14,
						p_ball->x + 14, p_ball->y + 14);
					
					if (this->m_bspacedown && this->m_b_ready) {
						g->ellipse(p_strength->x - 5, p_strength->y - 5,
							p_strength->x + 5, p_strength->y + 5);
						
						//力度条
						g->rectangle(p_ball->x - 40, p_ball->y + 9,
							p_ball->x - 40 + (this->m_strength - 50) * 4, p_ball->y + 19);
					}
					
					//绘制框下半部分
					g->draw_image_part(
						this->m_hbm_bt_basket,
						this->m_hbm_basket_mask, 
						0, 0.5, 1, 0.5,
						p_basket->x - this->m_basket_r,
						p_basket->y,
						2 * this->m_basket_r,
						16
						);
					
					//绘制地板
					g->select(this->m_hbr_floor_bt);
					g->rectangle(0, 400, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
				}
				
				//显示分数
				g->select(this->m_hf_game);
				g->select(this->m_hbr_score_bk);
				g->rectangle(5, 5, 115, 55);
				g->select(this->m_hbr_score_fg);
				g->rectangle(10, 10, 110, 50);
				TCHAR szScore[260];
				wsprintf(szScore, TEXT("%d"), int(this->m_score));
				g->draw_text(10, 10, 110, 50, szScore, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				
				//显示连击
				g->select(this->m_hf_combo);
				TCHAR szCombo[260];
				wsprintf(szCombo, TEXT("COMBO %d"), int(this->m_combo));
				g->draw_text(10, 60, 130, 100, szCombo, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				
				//显示大招
				for(int i = 0; i < this->m_n_super; i++)
				{
					TRIVERTEX vtx[2];
					vtx[0].x = 10;		
					vtx[0].y = 110 + 35 * i;  
					vtx[0].Alpha = 0;  
					vtx[0].Red = 0x0004;  
					vtx[0].Green = 0x0017;  
					vtx[0].Blue = 0xff00;  
					vtx[1].x = 40;  
					vtx[1].y = 140 + 35 * i;
					vtx[1].Alpha = 0;  
					vtx[1].Red = 0xfac0;  
					vtx[1].Green = 0xcccc;  
					vtx[1].Blue = 0xcef0;
					
					GRADIENT_RECT grc[1];
					grc[0].UpperLeft = 0;
					grc[0].LowerRight = 1;
					
					g->gradient_fill(vtx, 2, grc, 1, GRADIENT_FILL_RECT_V);
				}
				
				//显示生命
				for(int i = 0; i < this->m_n_life; i++)
				{
					g->draw_image(this->m_hbm_life, this->m_hbm_life_mask,
						180 + 10 * i, 10, 28, 28);
				}
				
				//绘制进球动画
				g->rectangle(
					XSGRAPHIC_COMPATIBLEBMP_W / 2 - 60 * this->m_k_scoring,
					XSGRAPHIC_COMPATIBLEBMP_H / 2 - 40 * this->m_k_scoring,
					XSGRAPHIC_COMPATIBLEBMP_W / 2 + 60 * this->m_k_scoring,
					XSGRAPHIC_COMPATIBLEBMP_H / 2 + 40 * this->m_k_scoring
					);
				
				//绘制不进球动画
				g->ellipse(
					XSGRAPHIC_COMPATIBLEBMP_W / 2 - 60 * this->m_k_missing,
					XSGRAPHIC_COMPATIBLEBMP_H / 2 - 40 * this->m_k_missing,
					XSGRAPHIC_COMPATIBLEBMP_W / 2 + 60 * this->m_k_missing,
					XSGRAPHIC_COMPATIBLEBMP_H / 2 + 40 * this->m_k_missing
					);
				
				//绘制大招动画
				TRIVERTEX vtx[2];
				vtx[0].x = XSGRAPHIC_COMPATIBLEBMP_W / 2 - 60 * this->m_k_super;		
				vtx[0].y = XSGRAPHIC_COMPATIBLEBMP_H / 2 - 40 * this->m_k_super;  
				vtx[0].Alpha = 0;  
				vtx[0].Red = 0x0004;  
				vtx[0].Green = 0x0017;  
				vtx[0].Blue = 0xff00;  
				vtx[1].x = XSGRAPHIC_COMPATIBLEBMP_W / 2 + 60 * this->m_k_super;  
				vtx[1].y = XSGRAPHIC_COMPATIBLEBMP_H / 2 + 40 * this->m_k_super;  
				vtx[1].Alpha = 0;  
				vtx[1].Red = 0xfac0;  
				vtx[1].Green = 0xcccc;  
				vtx[1].Blue = 0xcef0;
				
				GRADIENT_RECT grc[1];
				grc[0].UpperLeft = 0;
				grc[0].LowerRight = 1;
				
				g->gradient_fill(vtx, 2, grc, 1, GRADIENT_FILL_RECT_V);
			}
		});
		this->m_sc_play_bt.set_on_timer([this](POINT p_cur) {
			if(this->m_sc_play_bt.is_used())
			{
				this->m_anim_ball->on_timer();
				this->m_anim_basket->on_timer();
				this->m_anim_strength->on_timer();
				this->m_upd_score->on_timer();
				this->m_upd_dec_r->on_timer();
				this->m_upd_scoring->on_timer();
				this->m_upd_missing->on_timer();
				this->m_upd_getready_x->on_timer();
				this->m_upd_getready_y->on_timer();
				this->m_upd_super->on_timer();
				this->m_upd_basket_x->on_timer();
				POINT *p_ball = this->m_anim_ball->get_pos();
				POINT *p_basket = this->m_anim_basket->get_pos();
				//更新角度
				if (this->m_bspacedown) {
					if (p_cur.y == this->m_anim_ball->get_pos()->x)
						this->m_angle = M_PI / 2;
					else
						this->m_angle = atan2((p_cur.y - p_ball->y),
							(p_cur.x - p_ball->x));
				}
				//小球的出界判断
				if (p_ball->y > 400 || p_ball->x < 0 || p_ball->x > XSGRAPHIC_COMPATIBLEBMP_W) {
					this->m_anim_ball->set_func_x([p_ball, this](long t) {
						
						if(this->m_bflying)
						{
							this->m_bflying = false;
							//变态模式下投过球了
							this->m_bt_played = true;
							//大招的结束
							if(this->m_bsuper)
							{
								this->m_bsuper = false;
								this->m_anim_basket->restart();
								this->m_anim_basket->resume();
								this->m_k_super = 0;
								this->m_n_super --;
							}
							//如果本回合没得分, 也就是没投中
							if(!this->m_bscored)
							{
								//打断连击
								this->m_combo = 0;
								//执行没投中动画
								this->m_upd_missing->set_initial_num(0);
								this->m_upd_missing->resume();
							}
							else
							{
								//连击
								this->m_combo ++;
								//三个连击换取一个大招
								if(this->m_combo >= 3)
								{
									this->m_combo -= 3;
									this->m_n_super++;
								}
							}
						}
						if (p_ball->x < 0)
							return 0;
						if (p_ball->x > XSGRAPHIC_COMPATIBLEBMP_W)
							return XSGRAPHIC_COMPATIBLEBMP_W;
						return (int)p_ball->x;
					});
					if (p_ball->y > 400)
						this->m_anim_ball->set_func_y([p_ball, this](long t) {
							
							if(this->m_bflying)
							{
								this->m_bflying = false;
								//变态模式下投过球了
								this->m_bt_played = true;
								//大招的结束
								if(this->m_bsuper)
								{
									this->m_bsuper = false;
									this->m_anim_basket->restart();
									this->m_anim_basket->resume();
									this->m_k_super = 0;
									this->m_n_super --;
								}
								//如果本回合没得分, 也就是没投中
								if(!this->m_bscored)
								{
									//打断连击
									this->m_combo = 0;
									//执行没投中动画
									this->m_upd_missing->set_initial_num(0);
									this->m_upd_missing->resume();
								}
								else
								{
									//连击
									this->m_combo ++;
									//三个连击换取一个大招
									if(this->m_combo >= 3)
									{
										this->m_combo -= 3;
										this->m_n_super++;
									}
								}
								this->m_bscored_lastround = this->m_bscored;
							}
							if(t > 2000)
							{
								//使小球归位
								this->m_anim_ball->pause();
								this->m_upd_getready_x->set_initial_num(this->m_anim_ball->get_pos()->x);
								this->m_upd_getready_x->set_delta_num(
									this->m_launch_point.x - this->m_anim_ball->get_pos()->x);
								this->m_upd_getready_x->resume();
								
								this->m_upd_getready_y->set_initial_num(this->m_anim_ball->get_pos()->y);
								this->m_upd_getready_y->set_delta_num(
									this->m_launch_point.y - this->m_anim_ball->get_pos()->y);
								this->m_upd_getready_y->resume();
							}
							return 400;
						});
				}
				//小球的进球判断
				//如果从底下上去不能得分
				
				if (p_ball->x >= p_basket->x - this->m_basket_r
					&& p_ball->x <= p_basket->x + this->m_basket_r 
					&& p_ball->y <= p_basket->y + 16
					&& p_ball->y >= p_basket->y) {
					if(this->m_bscoring)
					{
						this->m_bscoring = false;
						double theta = atan2(
							p_ball->y - p_basket->y, p_ball->x - p_basket->x);
						this->m_initial_v_ball.x =  20 * cos(theta);
						this->m_initial_v_ball.y =  20 * sin(theta);
						this->m_initial_pos_ball.x = p_ball->x;
						this->m_initial_pos_ball.y = p_ball->y;
						this->m_anim_ball->restart();
					}
				}
				//在得分区可以得分
				if (abs(p_ball->x - p_basket->x) < this->m_basket_r * 4 / 5 && abs(p_ball->y - p_basket->y) < 10) {
					if (this->m_bscoring) {
						this->m_bscoring = false;
						this->m_n_scored_ball ++;
						this->m_upd_score->set_delta_num(10);
						this->m_upd_score->resume();
						if(m_n_scored_ball % 5 == 0)
							this->m_b_decreasing_r = true;
						
						this->m_bscored = true;
						this->m_upd_scoring->set_initial_num(0);
						this->m_upd_scoring->set_delta_num(1);
						this->m_upd_scoring->resume();
					}
				}
				if (this->m_b_decreasing_r) {
					this->m_b_decreasing_r = false;
					this->m_upd_dec_r->set_delta_num(-0.1 * this->m_basket_r);
					this->m_upd_dec_r->resume();
				}
				//在发射点时, 设置为已经准备好发射了
				if(abs(p_ball->x - this->m_launch_point.x) < 5
					&& abs(p_ball->y - this->m_launch_point.y) < 5
					&& !this->m_bflying
					&& !this->m_b_ready)
				{
					this->m_b_ready = true;
				}
				//按下空格键时
				if(this->m_bspacedown)
				{
					if (!this->m_bflying) {
						if(this->m_b_ready)
						{
							this->m_initial_pos_ball = this->m_launch_point;
							this->m_anim_ball->set_func_x([this](long t) {
								return this->m_initial_pos_ball.x + this->m_initial_v_ball.x * t / 100.0;
							});
							this->m_anim_ball->set_func_y([this](long t) {
								return this->m_initial_pos_ball.y + t * t / 3000.0 + 
								this->m_initial_v_ball.y * t / 100.0;
							});
							this->m_anim_ball->restart();
							this->m_anim_ball->pause();
							this->m_bscoring = true;
							this->m_bscored = false;
						}
					}
				}
				//按下大招键 'S' 时
				if(this->m_b_pressed_s)
				{
					if(this->m_n_super > 0)
					{
						if (!this->m_bflying) {
							if(this->m_b_ready)
							{
								this->m_initial_pos_ball = this->m_launch_point;
								this->m_anim_ball->set_func_x([this](long t) {
									return this->m_initial_pos_ball.x + this->m_initial_v_ball.x * t / 100.0;
								});
								this->m_anim_ball->set_func_y([this](long t) {
									return this->m_initial_pos_ball.y + t * t / 3000.0 + 
									this->m_initial_v_ball.y * t / 100.0;
								});
								this->m_anim_ball->restart();
								this->m_anim_ball->pause();
								this->m_bscoring = true;
								this->m_bscored = false;
							}
						}
					}
				}
				if(this->m_k_super == 1)
				{
					this->m_anim_basket->restart();
					this->m_anim_basket->resume();
					this->m_k_super = 0;
				}
				if(this->m_k_scoring == 1)
				{
					this->m_k_scoring = 0;
				}
				if(this->m_k_missing == 1)
				{
					this->m_k_missing = 0;
					this->m_n_life--;
					if(this->m_n_life <= 0)
					{
						this->m_b_endwith_bt = true;
						this->m_sc_end.use();
						this->m_sc_play_bt.unuse();
						this->m_upd_boommm->set_initial_num(0);
						this->m_upd_boommm->resume();
					}
				}
				if(this->m_bt_played)
				{
					this->m_anim_basket->set_func_x([this](int t) {
						return XSGRAPHIC_COMPATIBLEBMP_W / 2 * (1 + sin(t / 1000.0 * this->m_basket_speed))
						- this->m_basket_r * sin(t / 1000.0 * this->m_basket_speed);
					});
					this->m_sc_play_bt.unuse();
					this->m_sc_play.use();
				}
			}
		});
		this->m_sc_play_bt.set_on_keydown([this](WPARAM key) {
			if(this->m_sc_play_bt.is_used())
			{
				switch (key) {
					case VK_SPACE: {
						this->m_bspacedown = true;
						break;
					}
					case 'S' : {
						this->m_b_pressed_s = true;
						break;
					} 
				}
			}
		});
		this->m_sc_play_bt.set_on_keyup([this](WPARAM key) {
			if(this->m_sc_play_bt.is_used())
			{
				switch (key) {
					case VK_SPACE: {
						this->m_bspacedown = false;
						if (!this->m_bflying) {
							if(this->m_b_ready)
							{
								this->m_bflying = true;
								this->m_b_ready = false;
								this->m_initial_v_ball = {
									(LONG)(this->m_strength * cos(this->m_angle)),
									(LONG)(this->m_strength * sin(this->m_angle))
								};
								this->m_anim_ball->resume();
							}
						}
						break;
					}
					case 'S': {
						this->m_b_pressed_s = false;
						if(this->m_n_super > 0)
						{
							if(this->m_b_ready)
							{
								this->m_bflying = true;
								this->m_b_ready = false;
								this->m_bsuper = true;
								this->m_initial_v_ball = {
									(LONG)(70 * cos( - M_PI * 0.4)),
									(LONG)(70 * sin( - M_PI * 0.4))
								};
								this->m_anim_ball->resume();
								
								//将篮筐移动到指定位置
								this->m_anim_basket->pause();
								this->m_anim_basket->set_func_x([this](int t) {
									return XSGRAPHIC_COMPATIBLEBMP_W / 2 * (1 + cos(t / 1000.0 * this->m_basket_speed))
									- this->m_basket_r * cos(t / 1000.0 * this->m_basket_speed);
								});
								
								this->m_upd_basket_x->set_initial_num(this->m_anim_basket->get_pos()->x);
								this->m_upd_basket_x->set_delta_num(
									XSGRAPHIC_COMPATIBLEBMP_W - this->m_basket_r - this->m_anim_basket->get_pos()->x);
								this->m_upd_basket_x->resume();
								
								//启动更新器
								this->m_upd_super->set_initial_num(0);
								this->m_upd_super->resume();
							}
						}
						break;
					}
				}
			}
		});
		
	}
	
	//设置结束场景
	{
		this->m_sc_end = XSScene();
		this->m_sc_end.set_on_paint([this, apt_but_restart, apt_but_return_main_menu](XSGraphic *g){
			if(this->m_sc_end.is_used())
			{
				//获得动画点坐标
				POINT *p_ball = this->m_anim_ball->get_pos();
				POINT *p_basket = this->m_anim_basket->get_pos();
				POINT *p_strength = this->m_anim_strength->get_pos();
				
				double t = this->m_k_boom;
				
				//绘制背景
				if(this->m_b_endwith_bt)
					g->select(this->m_hbr_bkground_bt);
				else
					g->select(this->m_hbr_bkground_normal);
				g->rectangle(0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
				
				//绘制框上半部分
				if(this->m_b_endwith_bt)
					g->draw_image_part(
						this->m_hbm_bt_basket,
						this->m_hbm_basket_mask, 
						0, 0, 1, 0.5,
						p_basket->x - this->m_basket_r,
						p_basket->y - 16,
						2 * this->m_basket_r,
						16);
				else
					g->draw_image_part(
						this->m_hbm_normal_basket,
						this->m_hbm_basket_mask, 
						0, 0, 1, 0.5,
						p_basket->x - this->m_basket_r,
						p_basket->y - 16,
						2 * this->m_basket_r,
						16);
				
				if(this->m_k_boom == 0)
				{
					//绘制球
					if(this->m_b_endwith_bt) 
						g->select(this->m_hbr_ball_bt);
					else
						g->select(this->m_hbr_ball_normal);
					g->ellipse(p_ball->x - 14, p_ball->y - 14,
						p_ball->x + 14, p_ball->y + 14);
					
					if (this->m_bspacedown && this->m_b_ready) {
						g->ellipse(p_strength->x - 5, p_strength->y - 5,
							p_strength->x + 5, p_strength->y + 5);
						
						//力度条
						g->rectangle(p_ball->x - 40, p_ball->y + 9,
							p_ball->x - 40 + (this->m_strength - 50) * 4, p_ball->y + 19);
					}
				}
				
				//绘制框下半部分
				if(this->m_b_endwith_bt)
					g->draw_image_part(
						this->m_hbm_bt_basket,
						this->m_hbm_basket_mask, 
						0, 0.5, 1, 0.5,
						p_basket->x - this->m_basket_r,
						p_basket->y,
						2 * this->m_basket_r,
						16);
				else
					g->draw_image_part(
						this->m_hbm_normal_basket,
						this->m_hbm_basket_mask, 
						0, 0.5, 1, 0.5,
						p_basket->x - this->m_basket_r,
						p_basket->y,
						2 * this->m_basket_r,
						16);
				
				//绘制地板
				if(this->m_b_endwith_bt)
					g->select(this->m_hbr_floor_bt);
				else
					g->select(this->m_hbr_floor_normal);
				g->rectangle(0, 400, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
				
				//显示分数
				RECT rc;
				rc.left = 10 + t * (XSGRAPHIC_COMPATIBLEBMP_W / 2 - 80);
				rc.top = 10 + t * (XSGRAPHIC_COMPATIBLEBMP_H / 2 - 150);
				rc.right = 110 + t * (XSGRAPHIC_COMPATIBLEBMP_W / 2 - 40);
				rc.bottom = 50 + t * (XSGRAPHIC_COMPATIBLEBMP_H / 2 - 140);
				
				g->select(this->m_hf_game);
				g->select(this->m_hbr_score_bk);
				g->rectangle(rc.left - 5, rc.top - 5, rc.right + 5, rc.bottom + 5);
				g->select(this->m_hbr_score_fg);
				g->rectangle(rc.left, rc.top, rc.right, rc.bottom);
				TCHAR szScore[260];
				wsprintf(szScore, TEXT("%d"), int(this->m_score));
				g->draw_text(rc.left, rc.top, rc.right, rc.bottom, szScore, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				
				if(this->m_k_boom > 0)
				{
					//爆炸动画
					if(this->m_b_endwith_bt) 
						g->select(this->m_hbr_ball_bt);
					else
						g->select(this->m_hbr_ball_normal);
					
					POINT boom_unit[5];
					boom_unit[0].x = p_ball->x - 100 * t;
					boom_unit[0].y = p_ball->y - 7 - 300 * t + 1000 * t * t;
					
					boom_unit[1].x = p_ball->x + 7 + 300 * t;
					boom_unit[1].y = p_ball->y - 2 - 200 * t + 1000 * t * t;
					
					boom_unit[2].x = p_ball->x - 7 - 170 * t;
					boom_unit[2].y = p_ball->y - 2 + 30 * t + 1000 * t * t;
					
					boom_unit[3].x = p_ball->x + 4 + t * 200;
					boom_unit[3].y = p_ball->y + 6 + 1000 * t * t;
					
					boom_unit[4].x = p_ball->x - 4 - t * 400;
					boom_unit[4].y = p_ball->y + 6 - 20 * t + 1000 * t * t;
					
					for(int i = 0; i < 5; i++)
					{
						g->ellipse(boom_unit[i].x - 7, boom_unit[i].y - 7,
							boom_unit[i].x + 7, boom_unit[i].y + 7);
					}
				}
				
				//绘制按钮
				this->m_but_restart->draw(g);
				g->draw_image(this->m_hbm_replay, this->m_hbm_replay_mask,
					apt_but_restart[0].x, apt_but_restart[0].y,
					apt_but_restart[2].x - apt_but_restart[0].x, 
					apt_but_restart[2].y - apt_but_restart[0].y);
				
				this->m_but_return_main_menu->draw(g);
				
				TCHAR szReturnMainMenu[] = TEXT("返回开始界面");
				g->select(this->m_hf_combo);
				g->draw_text(apt_but_return_main_menu[0].x, apt_but_return_main_menu[0].y,
					apt_but_return_main_menu[2].x, apt_but_return_main_menu[2].y,
					szReturnMainMenu, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			}
		});
		this->m_sc_end.set_on_timer([this](POINT p_cur){
			this->m_upd_boommm->on_timer();
		});
		this->m_sc_end.set_on_lbuttondown([this](POINT p_cur){
			if(this->m_sc_end.is_used())
			{
				if(this->m_but_restart->b_in_range(p_cur.x, p_cur.y))	
				{
					this->m_but_restart->push();
				}
				
				if(this->m_but_return_main_menu->b_in_range(p_cur.x, p_cur.y))	
				{
					this->m_but_return_main_menu->push();
				}
			}
		});
		this->m_sc_end.set_on_lbuttonup([this](POINT p_cur){
			if(this->m_sc_end.is_used())
			{
				this->m_but_return_main_menu->b_in_range(p_cur.x, p_cur.y);
				this->m_but_return_main_menu->pop();
				
				this->m_but_restart->b_in_range(p_cur.x, p_cur.y);
				this->m_but_restart->pop();
			}
		});
		this->m_sc_end.set_on_mousemove([this](POINT p_cur){
			if(this->m_sc_end.is_used())
			{
				this->m_but_restart->b_in_range(p_cur.x, p_cur.y);
				this->m_but_return_main_menu->b_in_range(p_cur.x, p_cur.y);
			}
		});
	}
	
	this->m_sc_main_menu.use();
	this->m_sc_play.unuse();
	this->m_sc_play_bt.unuse();
	this->m_sc_end.unuse();
}

void XAppWindow::m_init_game()
{
	this->m_basket_r = 80;
	this->m_combo = 0;
	this->m_n_super = 1;
	this->m_score = 0;
	this->m_strength = 20;
	this->m_angle = 0;
	this->m_bspacedown = false;
	this->m_bscoring = false;
	this->m_b_decreasing_r = false;
	this->m_bflying = false;
	this->m_initial_pos_ball = this->m_launch_point;
	this->m_initial_v_ball = {0, 0};
	this->m_k_scoring = 0;
	this->m_k_missing = 0;
	this->m_bscored = false;
	this->m_bscored_lastround = false;
	this->m_b_ready = true;
	this->m_b_pressed_s = false;
	this->m_bt_played = true;
	this->m_n_scored_ball = 0;
	this->m_bsuper = false;
	this->m_n_life = 15;
	this->m_b_endwith_bt = false;
	this->m_k_boom = 0;
	this->m_upd_score->set_initial_num(0);
	this->m_upd_boommm->set_initial_num(0);
	this->m_upd_scoring->set_initial_num(0);
	this->m_upd_dec_r->set_initial_num(this->m_basket_r);
}
