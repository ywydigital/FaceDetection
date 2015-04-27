#pragma once


// CCatchDlg 对话框

class CCatchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCatchDlg)

public:
	CCatchDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCatchDlg();

// 对话框数据
	enum { IDD = IDD_CATCH_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOut();
public:
	// 捕捉的人脸输出文件夹
	CString m_strDesPath;
	afx_msg void OnBnClickedOk();
};
