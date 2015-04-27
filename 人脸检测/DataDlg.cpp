// DataDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Face.h"
#include "DataDlg.h"
#include "afxdialogex.h"


// CDataDlg �Ի���

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
	m_ctrlTab.InsertItem(0, _T("������")); m_ctrlTab.InsertItem(1, _T("������"));
	//���������Ի���
	m_page1.Create(IDD_DATABASE_DLG, &m_ctrlTab);
	m_page2.Create(IDD_DATABASE_DLG2, &m_ctrlTab); 
	//�趨��Tab����ʾ�ķ�Χ 
	CRect rc;
	m_ctrlTab.GetClientRect(rc);
	rc.top += 20; 
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;
	m_page1.MoveWindow(&rc);
	m_page2.MoveWindow(&rc);
	//�ѶԻ������ָ�뱣������
	pDlg[0] = &m_page1;
	pDlg[1] = &m_page2;
	//��ʾ��ʼҳ��
	pDlg[0]->ShowWindow(SW_SHOW);
	pDlg[1]->ShowWindow(SW_HIDE); 
	//���浱ǰѡ�� 
	m_CurSelTab = 0;
	return true;
}

BEGIN_MESSAGE_MAP(CDataDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDataDlg::OnSelchangeTab1)
END_MESSAGE_MAP()


// CDataDlg ��Ϣ�������


void CDataDlg::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 //�ѵ�ǰ��ҳ����������  
	pDlg[m_CurSelTab]->ShowWindow(SW_HIDE);  
	//�õ��µ�ҳ������  
	m_CurSelTab = m_ctrlTab.GetCurSel(); 
	//���µ�ҳ����ʾ����  
	pDlg[m_CurSelTab]->ShowWindow(SW_SHOW); 
	*pResult = 0;
	*pResult = 0;
}
