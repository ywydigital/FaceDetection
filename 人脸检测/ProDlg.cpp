// ProDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Face.h"
#include "ProDlg.h"
#include "afxdialogex.h"


// CProDlg �Ի���

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


// CProDlg ��Ϣ�������
