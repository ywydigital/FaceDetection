#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDatabaseDlg 对话框

class CDatabaseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDatabaseDlg)

public:
	CDatabaseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDatabaseDlg();

// 对话框数据
	enum { IDD = IDD_DATABASE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	void ShowDatabase(CString cmdStr);
public:
	CListCtrl m_list;
	afx_msg void OnItemchangedClassifier(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CEdit m_name;
	CEdit m_path;
	CEdit m_desc;
public:
	afx_msg void OnBnClickedAddClassifier();
	afx_msg void OnBnClickedDeleteClassifier2();
	afx_msg void OnBnClickedAlterClassifier();
};
