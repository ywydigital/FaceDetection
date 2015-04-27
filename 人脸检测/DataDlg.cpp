// DataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "DataDlg.h"
#include "afxdialogex.h"


// CDataDlg 对话框

IMPLEMENT_DYNAMIC(CDataDlg, CDialogEx)

CDataDlg::CDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataDlg::IDD, pParent)
{

}

CDataDlg::~CDataDlg()
{
}

void CDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ctrlTab);
}

BOOL CDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ctrlTab.InsertItem(0, _T("分类器")); m_ctrlTab.InsertItem(1, _T("人脸库"));
	//创建两个对话框
	m_page1.Create(IDD_DATABASE_DLG, &m_ctrlTab);
	m_page2.Create(IDD_DATABASE_DLG2, &m_ctrlTab); 
	//设定在Tab内显示的范围 
	CRect rc;
	m_ctrlTab.GetClientRect(rc);
	rc.top += 20; 
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;
	m_page1.MoveWindow(&rc);
	m_page2.MoveWindow(&rc);
	//把对话框对象指针保存起来
	pDlg[0] = &m_page1;
	pDlg[1] = &m_page2;
	//显示初始页面
	pDlg[0]->ShowWindow(SW_SHOW);
	pDlg[1]->ShowWindow(SW_HIDE); 
	//保存当前选择 
	m_CurSelTab = 0;
	return true;
}

BEGIN_MESSAGE_MAP(CDataDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDataDlg::OnSelchangeTab1)
END_MESSAGE_MAP()


// CDataDlg 消息处理程序


void CDataDlg::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	 //把当前的页面隐藏起来  
	pDlg[m_CurSelTab]->ShowWindow(SW_HIDE);  
	//得到新的页面索引  
	m_CurSelTab = m_ctrlTab.GetCurSel(); 
	//把新的页面显示出来  
	pDlg[m_CurSelTab]->ShowWindow(SW_SHOW); 
	*pResult = 0;
	*pResult = 0;
}
