#pragma once


// CCatchDireDlg 对话框

class CCatchDireDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCatchDireDlg)

public:
	CCatchDireDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCatchDireDlg();

// 对话框数据
	enum { IDD = IDD_CATCH_DIRE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// 要检测的目标目录
	CString m_detectDire;
	// 输出文件夹
	CString m_desPath;
	afx_msg void OnBnClickedButtonDire();
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnBnClickedOk();
};
