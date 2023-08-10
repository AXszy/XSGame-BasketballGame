/**
 * @file xsgraphic.cpp 
 * @brief: 绘图类的实现
 * @author: XSSunet
 * @date: 2023-07-09 10:33:04
 * 
 **/

#include "xsgraphic.h"
#include <windows.h>
#include <cmath>

XSGraphic::XSGraphic(HWND hwnd)
{
	this->m_hwnd = hwnd;
	this->m_hdc = ::BeginPaint(this->m_hwnd, &this->m_ps);
	
	//获得客户区宽度和高度
	RECT rcClient;
	GetClientRect(m_hwnd, &rcClient);
	this->m_width = rcClient.right - rcClient.left;
	this->m_height = rcClient.bottom - rcClient.top;
	
	//双缓冲绘图
	this->m_combmp = ::CreateCompatibleBitmap(
		this->m_hdc, 
		XSGRAPHIC_COMPATIBLEBMP_W, 
		XSGRAPHIC_COMPATIBLEBMP_H
		);
	this->m_comdc = ::CreateCompatibleDC(this->m_hdc);
	::SelectObject(this->m_comdc, this->m_combmp);
	
	::SetStretchBltMode(this->m_comdc, COLORONCOLOR);
	::SetBkMode(this->m_comdc, TRANSPARENT);
	
	this->m_comcomdc = ::CreateCompatibleDC(this->m_comdc);
}

XSGraphic::~XSGraphic()
{
	//将兼容DC的内容拷贝到DC上
	::StretchBlt(
		this->m_hdc, 
		0, 0, this->m_width, this->m_height, 
		this->m_comdc, 
		0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H,
		SRCCOPY
		);
	
	::DeleteDC(this->m_comdc);
	::DeleteDC(this->m_comcomdc);
	::DeleteObject(this->m_combmp);
	::EndPaint(this->m_hwnd, &this->m_ps);
}

void XSGraphic::select(HGDIOBJ ho)
{
	::SelectObject(this->m_comdc, ho);
}

void XSGraphic::rectangle(int l, int t, int r, int b)
{
	::Rectangle(this->m_comdc, l, t, r, b);
}

void XSGraphic::ellipse(int l, int t, int r, int b)
{
	::Ellipse(this->m_comdc, l, t, r, b);
}

void XSGraphic::arc(int l, int t, int r, int b, int x1, int y1, int x2, int y2)
{
	::Arc(this->m_comdc, l, t, r, b, x1, y1, x2,y2);
}

void XSGraphic::polyline(POINT *apt, int cpt)
{
	::Polyline(this->m_comdc, apt, cpt);
}

void XSGraphic::polygon(POINT *apt, int cpt)
{
	::Polygon(this->m_comdc, apt, cpt);
}

void XSGraphic::poly_bezier(POINT *apt, int cpt)
{
	::PolyBezier(this->m_comdc, apt, cpt);
}

void XSGraphic::gradient_fill(TRIVERTEX *pvertex, int nvertex, void *pmesh, int nmesh, long mode)
{
	::GradientFill(this->m_comdc, pvertex, nvertex, pmesh, nmesh, mode);
}

void XSGraphic::draw_image(HBITMAP hbm, int x, int y, int w, int h)
{
	::SelectObject(this->m_comcomdc, hbm);
	
	BITMAP bm;
	::GetObject(hbm, sizeof(bm), &bm);
	
	StretchBlt(this->m_comdc, x, y, w, h, this->m_comcomdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
}

void XSGraphic::draw_image(HBITMAP hbm, HBITMAP hbmMask, int x, int y, int w, int h)
{
	BITMAP bm;
	
	::SelectObject(this->m_comcomdc, hbmMask);
	::GetObject(hbmMask, sizeof(bm), &bm);
	StretchBlt(this->m_comdc, x, y, w, h, this->m_comcomdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCPAINT);
	
	::SelectObject(this->m_comcomdc, hbm);
	::GetObject(hbm, sizeof(bm), &bm);
	StretchBlt(this->m_comdc, x, y, w, h, this->m_comcomdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCAND);
}

void XSGraphic::draw_bits(void *lpbits, int bmw, int bmh, int x, int y, int w, int h, DWORD mode)
{
	BITMAPINFO binfo;
	
	ZeroMemory(&binfo,sizeof(BITMAPINFO));
	binfo.bmiHeader.biBitCount=24;      //每个像素多少位，也可直接写24(RGB)或者32(RGBA)
	binfo.bmiHeader.biCompression=0;
	binfo.bmiHeader.biHeight=bmh;
	binfo.bmiHeader.biPlanes=1;
	binfo.bmiHeader.biSizeImage=0;
	binfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	binfo.bmiHeader.biWidth=bmw;
	
	//SetDIBits(this->m_comdc,this->m_combmp,0,binfo.bmiHeader.biHeight,lpbits,(BITMAPINFO*)&binfo,DIB_RGB_COLORS);
	StretchDIBits(this->m_comdc, x, y, w, h, 0, 0, bmw, bmh - 1, lpbits, &binfo, DIB_RGB_COLORS, mode);
}

void XSGraphic::plg_blt(HBITMAP hbm, COLORREF crmask, POINT &lt, POINT &rt, POINT &ld)
{
	POINT apt[3] = {lt, rt, ld};
	BITMAP bm;
	
	HDC mdc = CreateCompatibleDC(m_comdc);
	::SelectObject(mdc, hbm);
	::GetObject(hbm, sizeof(bm), &bm);
	
	HBITMAP mbm = CreateCompatibleBitmap(this->m_comdc, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
	RECT rc = {0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H};
	HBRUSH hbr = CreateSolidBrush(crmask);
	SelectObject(this->m_comcomdc, mbm);
	FillRect(m_comcomdc, &rc, hbr);
	
	PlgBlt(this->m_comcomdc, apt, mdc, 0, 0, bm.bmWidth, bm.bmHeight, 0, 0, 0);
	TransparentBlt(this->m_comdc, 0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H, 
		this->m_comcomdc, 0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H, crmask);
	
	DeleteDC(mdc);
	DeleteObject(hbr);
	DeleteObject(mbm);
}

void XSGraphic::draw_text(int l, int t, int r, int b, TCHAR *str, DWORD align)
{
	RECT rc = {l, t, r, b};
	::DrawTextEx(this->m_comdc, str, lstrlen(str), &rc, align, NULL);
}

void XSGraphic::draw_image_part(HBITMAP hbm, double ksrcx, double ksrcy, double ksrcw, double ksrch, int dstx, int dsty, int dstw, int dsth)
{
	::SelectObject(this->m_comcomdc, hbm);
	
	BITMAP bm;
	::GetObject(hbm, sizeof(bm), &bm);
	
	StretchBlt(this->m_comdc, dstx, dsty, dstw, dsth, 
		this->m_comcomdc, bm.bmWidth * ksrcx, bm.bmHeight * ksrcy, 
		bm.bmWidth * ksrcw, bm.bmHeight * ksrch, SRCCOPY);
}

void XSGraphic::draw_image_part(HBITMAP hbm, HBITMAP hbmMask, double ksrcx, double ksrcy, double ksrcw, double ksrch, int dstx, int dsty, int dstw, int dsth)
{
	BITMAP bm;
	
	::SelectObject(this->m_comcomdc, hbmMask);
	::GetObject(hbmMask, sizeof(bm), &bm);
	StretchBlt(this->m_comdc, dstx, dsty, dstw, dsth, 
		this->m_comcomdc, bm.bmWidth * ksrcx, bm.bmHeight * ksrcy, 
		bm.bmWidth * ksrcw, bm.bmHeight * ksrch, SRCPAINT);
	
	::SelectObject(this->m_comcomdc, hbm);
	::GetObject(hbm, sizeof(bm), &bm);
	StretchBlt(this->m_comdc, dstx, dsty, dstw, dsth, 
		this->m_comcomdc, bm.bmWidth * ksrcx, bm.bmHeight * ksrcy, 
		bm.bmWidth * ksrcw, bm.bmHeight * ksrch, SRCAND);
}

