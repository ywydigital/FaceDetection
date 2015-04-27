#pragma once


// CProDlg 对话框

class CProDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProDlg)

public:
	CProDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProDlg();
	virtual void OnOK();

// 对话框数据
	enum { IDD = IDD_PRO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
};
