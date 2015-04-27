#pragma once
#include "afxwin.h"


// CSetDlg 对话框

class CSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDlg)

public:
	CSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetDlg();

// 对话框数据
	enum { IDD = IDD_SET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	// 搜索窗口比例
	double m_scale_factor;
	// 最小相邻矩形数
	int m_min_neighbors;
	// 检测窗口最小尺寸宽
	int m_min_size_w;
	// 检测窗口最小尺寸高
	int m_min_size_h;
	// 分类器路径
	CString m_cascade_name;
	// 是否进行边缘检测
	int m_flags;
	// 分类器下拉列表控件
	CComboBox m_classifier;
	afx_msg void OnCbnDropdownCombo2();
	afx_msg void OnBnClickedOk();
};
