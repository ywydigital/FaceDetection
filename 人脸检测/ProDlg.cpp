// ProDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "ProDlg.h"
#include "afxdialogex.h"


// CProDlg 对话框

IMPLEMENT_DYNAMIC(CProDlg, CDialogEx)

CProDlg::CProDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProDlg::IDD, pParent)
{
}

CProDlg::~CProDlg()
{
	
}

void CProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CProDlg::OnOK()
{
	DestroyWindow();
	CDialogEx::OnOK();
}

BEGIN_MESSAGE_MAP(CProDlg, CDialogEx)
END_MESSAGE_MAP()


// CProDlg 消息处理程序
