
// Face.h : Face Ӧ�ó������ͷ�ļ�
//

#if !defined(AFX_IMAGETOOL_H__8465860F_D179_43D7_B7C4_EB66D21162FE__INCLUDED_)
#define AFX_IMAGETOOL_H__8465860F_D179_43D7_B7C4_EB66D21162FE__INCLUDED_

#if _MSC_VER > 1000

#pragma once
#endif

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#define	THUMBNAIL_WIDTH		100
#define	THUMBNAIL_HEIGHT	90

// CFaceApp:
// �йش����ʵ�֣������ Face.cpp
//

class CFaceApp : public CWinAppEx
{
public:
	CFaceApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
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
