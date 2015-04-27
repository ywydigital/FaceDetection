#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDatabaseDlg2 �Ի���

class CDatabaseDlg2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDatabaseDlg2)

public:
	CDatabaseDlg2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDatabaseDlg2();

// �Ի�������
	enum { IDD = IDD_DATABASE_DLG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void ShowDatabase(CString cmdStr);
public:
	CListCtrl m_list;
	CEdit m_sname;
	CEdit m_fname;
	CEdit m_spath;
	CEdit m_fpath;
	afx_msg void OnItemchangedFaceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDeleteFace();
	afx_msg void OnBnClickedAlterFace();
};
