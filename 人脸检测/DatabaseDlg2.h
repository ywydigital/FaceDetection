#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDatabaseDlg2 对话框

class CDatabaseDlg2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDatabaseDlg2)

public:
	CDatabaseDlg2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDatabaseDlg2();

// 对话框数据
	enum { IDD = IDD_DATABASE_DLG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
