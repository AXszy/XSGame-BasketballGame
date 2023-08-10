/**
 * @file xsmusic.cpp 
 * @brief: 音乐类的实现
 * @author: XSSunet
 * @date: 2023-07-31 12:45:17
 * 
 **/
#include "xsmusic.h"
#include <mmsystem.h>

XSMusic::XSMusic(const TCHAR *filename)
{
	MCI_OPEN_PARMS open;
	open.lpstrDeviceType = NULL;
	open.lpstrElementName = filename;
	mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_ELEMENT,(DWORD)(LPVOID)&open);
	
	this->m_w_device_id = open.wDeviceID;	//设备id
}

XSMusic::~XSMusic()
{
	mciSendCommand(this->m_w_device_id,MCI_CLOSE,NULL,NULL);
}

void XSMusic::play()
{
	mciSendCommand(this->m_w_device_id, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&this->m_play);
}

void XSMusic::stop()
{
	MCI_GENERIC_PARMS generic;
	generic.dwCallback = NULL;
	mciSendCommand(this->m_w_device_id, MCI_STOP, MCI_WAIT, (DWORD)(LPVOID)&generic);
}

void XSMusic::pause()
{
	mciSendCommand(this->m_w_device_id,MCI_PAUSE,MCI_FROM,(DWORD)(LPVOID)&this->m_play);
}

void XSMusic::resume()
{
	mciSendCommand(this->m_w_device_id,MCI_RESUME,MCI_FROM,(DWORD)(LPVOID)&this->m_play);
}

DWORD XSMusic::get_mode()
{
	MCI_STATUS_PARMS status;
	status.dwItem = MCI_STATUS_MODE;
	mciSendCommand(this->m_w_device_id, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID)&status);
	return status.dwReturn;
}

DWORD XSMusic::get_length()
{
	MCI_STATUS_PARMS status;
	status.dwItem = MCI_STATUS_LENGTH;
	mciSendCommand(this->m_w_device_id, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID)&status);
	return status.dwReturn;
}

DWORD XSMusic::get_status(DWORD item)
{
	MCI_STATUS_PARMS status;
	status.dwItem = item;
	mciSendCommand(this->m_w_device_id, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID)&status);
	return status.dwReturn;
}

int XSMusic::get_volume()
{
	MCI_DGV_STATUS_PARMS status;
	status.dwItem = MCI_DGV_STATUS_VOLUME;
	mciSendCommand(this->m_w_device_id, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID)&status);
	return status.dwReturn;
}

void XSMusic::set_volume(int vol)
{
	MCI_DGV_SETAUDIO_PARMS setvolume; 			//设置音量的参数结构体
	setvolume.dwCallback = NULL;
	setvolume.dwItem = MCI_DGV_SETAUDIO_VOLUME; //动作是设置音量
	setvolume.dwValue = vol; 					//音量值是vol
	mciSendCommand(this->m_w_device_id, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD)(LPVOID)&setvolume);
}

DWORD XSMusic::get_position()
{
	MCI_STATUS_PARMS status;
	status.dwItem = MCI_STATUS_POSITION;
	mciSendCommand(this->m_w_device_id, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID)&status);
	return status.dwReturn;
}

void XSMusic::set_position(DWORD millisecond)
{
	MCI_SEEK_PARMS seek;
	seek.dwTo = millisecond;
	seek.dwCallback = NULL;
	mciSendCommand(this->m_w_device_id, MCI_SEEK, MCI_TO | MCI_WAIT | MCI_NOTIFY, (DWORD)(LPVOID)&seek);
}

void XSMusic::seek_start()
{
	mciSendCommand(this->m_w_device_id, MCI_SEEK, MCI_SEEK_TO_START, NULL);
}

void XSMusic::seek_end()
{
	mciSendCommand(this->m_w_device_id, MCI_SEEK, MCI_SEEK_TO_END, NULL);
}
double XSMusic::get_speed()
{
	MCI_STATUS_PARMS status;
	status.dwItem = MCI_DGV_STATUS_SPEED;
	mciSendCommand(this->m_w_device_id, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID)&status);
	return status.dwReturn / 1000.0;
}

void XSMusic::set_speed(double scalar)
{
	MCI_DGV_SET_PARMS set;
	set.dwSpeed = scalar * 1000;
	mciSendCommand(this->m_w_device_id, MCI_SET, MCI_DGV_SET_SPEED, (DWORD)(LPVOID)&set);
}

