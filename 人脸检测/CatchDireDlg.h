#pragma once


// CCatchDireDlg �Ի���

class CCatchDireDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCatchDireDlg)

public:
	CCatchDireDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCatchDireDlg();

// �Ի�������
	enum { IDD = IDD_CATCH_DIRE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// Ҫ����Ŀ��Ŀ¼
	CString m_detectDire;
	// ����ļ���
	CString m_desPath;
	afx_msg void OnBnClickedButtonDire();
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnBnClickedOk();
};
