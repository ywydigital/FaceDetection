#pragma once


// CProDlg �Ի���

class CProDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProDlg)

public:
	CProDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProDlg();
	virtual void OnOK();

// �Ի�������
	enum { IDD = IDD_PRO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
};
