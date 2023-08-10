/**
 * @file xsmusic.h 
 * @brief: 音乐类
 * @author: XSSunet
 * @date: 2023-07-31 12:45:02
 * 
 **/
#ifndef XSMUSIC_H
#define XSMUSIC_H

#include <windows.h>
#include <digitalv.h>
#include <mmsystem.h>

class XSMusic {
public:
	XSMusic() = delete;
	XSMusic(const TCHAR *filename);			/*构造函数*/
	~XSMusic();								/*析构函数*/
	
	void play();		/*播放*/
	void stop();							/*停止播放*/
	void pause();							/*暂停播放*/
	void resume();							/*继续播放*/
	DWORD get_status(DWORD item);			/*获得歌曲播放信息*/
	
	DWORD get_mode();						/*获得当前播放模式(播放, 暂停...)*/
	DWORD get_length();						/*获得文件长度*/
	
	int get_volume();						/*获得媒体音量*/
	void set_volume(int vol);				/*设置媒体音量*/
	
	DWORD get_position();					/*获得文件当前播放位置*/
	void set_position(DWORD millisecond);	/*跳转到音乐文件某位置*/
	void seek_start();						/*跳转到文件开头*/
	void seek_end();						/*跳转到音乐文件结尾*/
	
	double get_speed();						/*获得当前播放倍速*/
	void set_speed(double scalar);			/*设置倍速*/
private:
	UINT m_w_device_id;
	MCI_PLAY_PARMS m_play;
};

#endif
