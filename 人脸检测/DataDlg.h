#pragma once
#include "afxcmn.h"
#include "DatabaseDlg.h"
#include "DatabaseDlg2.h"


// CDataDlg �Ի���

class CDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataDlg)

public:
	CDataDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDataDlg();

// �Ի�������
	enum { IDD = IDD_DATA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_ctrlTab;
	CDatabaseDlg m_page1;
	CDatabaseDlg2 m_page2;
	CDialogEx* pDlg[2];
	int m_CurSelTab;
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
