// SetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "SetDlg.h"
#include "afxdialogex.h"


// CSetDlg 对话框

IMPLEMENT_DYNAMIC(CSetDlg, CDialogEx)

CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetDlg::IDD, pParent)
	, m_scale_factor(0)
	, m_min_neighbors(0)
	, m_min_size_w(0)
	, m_min_size_h(0)
	, m_cascade_name(_T(""))
	, m_flags(0)
{
	
}

CSetDlg::~CSetDlg()
{
}

void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_scale_factor);
	DDX_Text(pDX, IDC_EDIT2, m_min_neighbors);
	DDX_Text(pDX, IDC_EDIT3, m_min_size_w);
	DDX_Text(pDX, IDC_EDIT4, m_min_size_h);
	DDX_Check(pDX, IDC_CHECK1, m_flags);
	DDX_Control(pDX, IDC_COMBO2, m_classifier);
}

BOOL CSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CComboBox*)GetDlgItem(IDC_COMBO2))->ResetContent();

	//从数据库中读取分类器列表，给用户选择
	CString sql = "select CName from classifier Order by ClassifierID";
	CDatabase db;
	db.Open(NULL,FALSE,FALSE,"ODBC;DSN=face;UID=root;PWD=root");
	CRecordset rs(&db);
	rs.Open(CRecordset::forwardOnly,(L"%s",sql));
	while(!rs.IsEOF())
	{
		CString name;
		rs.GetFieldValue("CName",name);
		((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(name);
		rs.MoveNext();
		
	}

	rs.Close();
	db.Close();

	m_scale_factor = ::g_scale_factor;
	m_min_neighbors = ::g_min_neighbors;
	m_min_size_w = ::g_min_size_w;
	m_min_size_h = ::g_min_size_h;
	m_cascade_name = (g_cascade_name).Mid((g_cascade_name).ReverseFind('\\')+1,
			(g_cascade_name).GetLength()-(g_cascade_name).ReverseFind('\\')-1);

	//设置cascade在下拉列表中，选中对应行
	int nIndex = m_classifier.SelectString(0,m_cascade_name);

	if(g_flags == 0)
		m_flags = true;
	else
		m_flags = false;
	UpdateData(FALSE);
	return true;
}

BEGIN_MESSAGE_MAP(CSetDlg, CDialogEx)
	ON_CBN_DROPDOWN(IDC_COMBO2, &CSetDlg::OnCbnDropdownCombo2)
	ON_BN_CLICKED(IDOK, &CSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetDlg 消息处理程序


void CSetDlg::OnCbnDropdownCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	int iCount = ((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCount();

	if(iCount<1)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO2))->ResetContent();
		//从数据库中读取分类器列表，给用户选择
		CString sql = "select CName from classifier Order by ClassifierID";
		CDatabase db;
		db.Open(NULL,FALSE,FALSE,"ODBC;DSN=face;UID=root;PWD=root");
		CRecordset rs(&db);
		rs.Open(CRecordset::forwardOnly,(L"%s",sql));
		while(!rs.IsEOF())
		{
			CString name;
			rs.GetFieldValue("CName",name);
			((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(name);
			rs.MoveNext();
		
		}

		rs.Close();
		db.Close();
	}
}


void CSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//根据设置，设定全局变量的值

	CString tmp;
	GetDlgItem(IDC_EDIT1)->GetWindowText(tmp);
	m_scale_factor = _tcstod(tmp,NULL);

	GetDlgItem(IDC_EDIT2)->GetWindowText(tmp);
	m_min_neighbors = _ttoi(tmp);

	GetDlgItem(IDC_EDIT3)->GetWindowText(tmp);
	m_min_size_w = _ttoi(tmp);

	GetDlgItem(IDC_EDIT4)->GetWindowText(tmp);
	m_min_size_h = _ttoi(tmp);

	GetDlgItem(IDC_CHECK1)->GetWindowText(tmp);
	m_flags = _ttoi(tmp);


	g_scale_factor = m_scale_factor;
	g_min_neighbors = m_min_neighbors;
	g_min_size_w = m_min_size_w;
	g_min_size_h = m_min_size_h;

	int index = m_classifier.GetCurSel();
	GetDlgItem(IDC_COMBO2)->GetWindowText(tmp);

	m_cascade_name = tmp;
	//g_cascade_name.Empty();
	g_cascade_name.Format("..\\classifier\\%s",m_cascade_name);

	g_flags = 0;  
	//始终要进行边缘检测
	CDialogEx::OnOK();
}
