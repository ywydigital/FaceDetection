#pragma once
#include "afxwin.h"


// CSetDlg �Ի���

class CSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDlg)

public:
	CSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetDlg();

// �Ի�������
	enum { IDD = IDD_SET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	// �������ڱ���
	double m_scale_factor;
	// ��С���ھ�����
	int m_min_neighbors;
	// ��ⴰ����С�ߴ��
	int m_min_size_w;
	// ��ⴰ����С�ߴ��
	int m_min_size_h;
	// ������·��
	CString m_cascade_name;
	// �Ƿ���б�Ե���
	int m_flags;
	// �����������б�ؼ�
	CComboBox m_classifier;
	afx_msg void OnCbnDropdownCombo2();
	afx_msg void OnBnClickedOk();
};
