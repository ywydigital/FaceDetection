
// Face.h : Face 应用程序的主头文件
//

#if !defined(AFX_IMAGETOOL_H__8465860F_D179_43D7_B7C4_EB66D21162FE__INCLUDED_)
#define AFX_IMAGETOOL_H__8465860F_D179_43D7_B7C4_EB66D21162FE__INCLUDED_

#if _MSC_VER > 1000

#pragma once
#endif

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#define	THUMBNAIL_WIDTH		100
#define	THUMBNAIL_HEIGHT	90

// CFaceApp:
// 有关此类的实现，请参阅 Face.cpp
//

class CFaceApp : public CWinAppEx
{
public:
	CFaceApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFaceApp theApp;

#endif
