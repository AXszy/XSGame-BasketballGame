/**
 * @file main.cpp 
 * @brief: C++程序入口处
 * @author: XSSunset
 * @date: 2023-07-09 10:30:39
 * 
 **/

#include "xappwindow.h"

int main()
{
	MSG msg;
	
	//创建程序窗口
	XAppWindow *app = new XAppWindow();
	app->create();
	ShowWindow(app->getHWND(), SW_SHOW);
	
	auto b_end = [app]() -> bool
	{
		return (app->getHWND());
	};
	
	//消息循环
	while(GetMessage(&msg, NULL, 0, 0) > 0 && b_end())
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	//释放程序窗口
	delete app;
	
	return 0;
}
