#pragma once


// CCatchDlg �Ի���

class CCatchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCatchDlg)

public:
	CCatchDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCatchDlg();

// �Ի�������
	enum { IDD = IDD_CATCH_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOut();
public:
	// ��׽����������ļ���
	CString m_strDesPath;
	afx_msg void OnBnClickedOk();
};
