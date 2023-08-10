/**
 * @file xappwindow.h 
 * @brief: 应用程序窗口类
 * @author: XSSunet
 * @date: 2023-07-09 10:32:18
 * 
 **/

#ifndef XAPPWINDOW_H
#define XAPPWINDOW_H

#include "xswindow.h"
#include "xsgraphic.h"
#include "xsmovablepolygon.h"
#include "xspolygonbutton.h"
#include "xsanimatedpoint.h"
#include "xsmusic.h"
#include "xsscene.h"
#include "xsupdater.h"

class XAppWindow : public XSWindow {
public:
	XAppWindow();										/*构造函数*/
	XAppWindow(HINSTANCE hInst);						/*构造函数*/
	~XAppWindow();										/*析构函数*/
	int handle_message(HWND, UINT, WPARAM, LPARAM);		/*具体实现App功能的消息处理*/	
	void get_cursor_pos(HWND hwnd, POINT *p);			/*获得鼠标在窗口中的坐标*/
	
	void on_create();									/*创建窗口时的消息处理*/
private:
	//具体实现App功能的私有成员变量
	XSScene m_sc_main_menu;								/*场景 : 主菜单*/
	XSScene m_sc_play;									/*场景 : 正常的投篮游戏*/
	XSScene m_sc_play_bt;								/*场景 : 变态的投篮游戏*/
	XSScene m_sc_end;									/*场景 : 命没了, 结束游戏了*/
	
	XSAnimatedPoint *m_anim_basket;						/*动画对象 : 篮筐*/
	XSAnimatedPoint *m_anim_ball;						/*动画对象 : 篮球*/
	XSAnimatedPoint *m_anim_strength;					/*动画对象 : 标志力量的条和标志发射角的箭头*/
	
	XSUpdater *m_upd_score;								/*更新器 : 加分*/
	XSUpdater *m_upd_dec_r;								/*更新器 : 减小篮筐*/
	XSUpdater *m_upd_scoring;							/*更新器 : 进球动画*/
	XSUpdater *m_upd_missing;							/*更新器 : 没投中*/
	XSUpdater *m_upd_super;								/*更新器 : 放大招*/
	XSUpdater *m_upd_boommm;							/*更新器 : boommmm!!!!球爆炸了!*/
	
	XSUpdaterLong *m_upd_getready_x;					/*更新器 : 小球的 x */
	XSUpdaterLong *m_upd_getready_y;					/*更新器 : 小球的 y */
	XSUpdaterLong* m_upd_basket_x;						/*更新器 : 篮筐 x 坐标*/
	
	XSPolygonButton *m_but_start;						/*按钮 : 开始游戏*/
	XSPolygonButton *m_but_return_main_menu;			/*按钮 : 返回主菜单*/
	XSPolygonButton *m_but_restart;						/*按钮 : 重新游戏*/
	
	double m_strength;									/*出手力度*/
	double m_angle;										/*出手角*/
	double m_score;										/*游戏分数*/
	double m_basket_r;									/*篮球框宽度*/
	double m_basket_speed;								/*篮球框速度*/
	double m_k_scoring;									/*进球动画进行程度*/
	double m_k_missing;									/*没投中的动画进行程度*/
	double m_k_super;									/*释放大招*/
	double m_k_boom;									/*爆炸的参数*/
	double m_qqhead_angle;								/*qq头像旋转的角度*/
	
	int m_combo;										/*投篮连击*/
	int m_n_super;										/*大招数量*/
	int m_n_scored_ball;								/*总共进了几个球*/
	int m_n_life;										/*剩余的生命数*/
	
	bool m_bspacedown;									/*空格键是否按下*/
	bool m_bscoring;									/*是否正在得分*/
	bool m_b_decreasing_r;								/*是否正在减小篮筐的半径*/
	bool m_bflying;										/*球是否正在飞*/
	bool m_bscored;										/*这回合是否进球*/
	bool m_bscored_lastround;							/*上回合是否进球*/
	bool m_b_ready;										/*是否准备好了*/
	bool m_b_pressed_s;									/*S 键是否按下*/
	bool m_bt_played;									/*变态关卡是否投出了一次了*/
	bool m_bsuper;										/*本回合是否开大招*/
	bool m_b_endwith_bt;								/*游戏结束是否是变态关*/
	
	POINT m_initial_pos_ball;							/*篮球执行动画开始时球的位置*/
	POINT m_initial_v_ball;								/*篮球执行动画开始时球的速度矢量*/
	POINT m_launch_point;								/*发射点坐标*/
	
	HBRUSH m_hbr_ball_normal;							/*篮球的画刷*/
	HBRUSH m_hbr_bkground_normal;						/*背景的画刷*/
	HBRUSH m_hbr_floor_normal;							/*地板的画刷*/
	HBRUSH m_hbr_ball_bt;								/*变态篮球的画刷*/
	HBRUSH m_hbr_bkground_bt;							/*背景的画刷*/
	HBRUSH m_hbr_floor_bt;								/*地板的画刷*/
	HBRUSH m_hbr_ball_super;							/*大招篮球的画刷*/
	HBRUSH m_hbr_bkground_super;						/*背景的画刷*/
	HBRUSH m_hbr_floor_super;							/*地板的画刷*/
	HBRUSH m_hbr_score_bk;								/*显示分数背景的画刷*/
	HBRUSH m_hbr_score_fg;								/*显示分数前景的画刷*/
	HBRUSH m_hbr_restart_button_normal;					/*重新玩的按键一般情况下的颜色画刷*/
	
	HBITMAP m_hbm_normal_basket;						/*普通篮筐图片*/
	HBITMAP m_hbm_bt_basket;							/*变态篮筐图片*/
	HBITMAP m_hbm_super_basket;							/*大招篮筐图片*/
	HBITMAP m_hbm_basket_mask;							/*篮筐的mask*/
	HBITMAP m_hbm_life;									/*生命图片*/
	HBITMAP m_hbm_life_mask;							/*生命图片的mask*/
	HBITMAP m_hbm_replay;								/*重新玩图片*/
	HBITMAP m_hbm_replay_mask;							/*重新玩图片的mask*/
	HBITMAP m_hbm_qqhead;								/*我的QQ头像*/
	
	HFONT m_hf_game;									/*游戏字体*/
	HFONT m_hf_combo;									/*显示连击数用的小字体*/
	HFONT m_hf_title;									/*显示标题用的大字体*/
	
	void m_init_game();				/*初始化每一局游戏的函数*/
};

#endif
