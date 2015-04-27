// DatabaseDlg2.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "DatabaseDlg2.h"
#include "afxdialogex.h"


// CDatabaseDlg2 对话框

IMPLEMENT_DYNAMIC(CDatabaseDlg2, CDialogEx)

CDatabaseDlg2::CDatabaseDlg2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDatabaseDlg2::IDD, pParent)
{

}

CDatabaseDlg2::~CDatabaseDlg2()
{
}



void CDatabaseDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FACE_LIST, m_list);
	DDX_Control(pDX, IDC_SNAME, m_sname);
	DDX_Control(pDX, IDC_FNAME, m_fname);
	DDX_Control(pDX, IDC_SPATH, m_spath);
	DDX_Control(pDX, IDC_FPATH, m_fpath);
}

BOOL CDatabaseDlg2::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_list.InsertColumn(0,"ID",LVCFMT_LEFT,40);
	m_list.InsertColumn(1,"源图像名",LVCFMT_LEFT,90);
	m_list.InsertColumn(2,"源图像路径",LVCFMT_LEFT,120);
	m_list.InsertColumn(3,"人脸图像名",LVCFMT_LEFT,90);
	m_list.InsertColumn(4,"人脸图像路径",LVCFMT_LEFT,120);
	m_list.InsertColumn(5,"生成时间",LVCFMT_LEFT,120);
	CString cmdStr = "Select * from face order by ID";
	ShowDatabase(cmdStr);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDatabaseDlg2, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FACE_LIST, &CDatabaseDlg2::OnItemchangedFaceList)
	ON_BN_CLICKED(IDC_DELETE_FACE, &CDatabaseDlg2::OnBnClickedDeleteFace)
	ON_BN_CLICKED(IDC_ALTER_FACE, &CDatabaseDlg2::OnBnClickedAlterFace)
END_MESSAGE_MAP()


// CDatabaseDlg2 消息处理程序
void CDatabaseDlg2::ShowDatabase(CString cmdStr)
{
	m_list.DeleteAllItems();
	//打开数据库
	CDatabase db;
	db.Open(NULL,FALSE,FALSE,"ODBC;DSN=face;UID=root;PWD=root");
	//创建查询结果接收集
	CRecordset rs(&db);
	rs.Open(CRecordset::forwardOnly,(L"%s",cmdStr));
	int i = 0;
	//操作查询结果
	while(!rs.IsEOF())
	{
		CString id;
		rs.GetFieldValue("ID",id);
		m_list.InsertItem(i,id);

		CString simg;
		rs.GetFieldValue("SourceImg",simg);
		m_list.SetItemText(i,1,simg);

		CString spath;
		rs.GetFieldValue("SourcePath",spath);
		m_list.SetItemText(i,2,spath);

		CString fimg;
		rs.GetFieldValue("FaceName",fimg);
		m_list.SetItemText(i,3,fimg);

		CString fpath;
		rs.GetFieldValue("FacePath",fpath);
		m_list.SetItemText(i,4,fpath);

		CString date;
		rs.GetFieldValue("Time",date);
		m_list.SetItemText(i,5,date);
		i++;

		rs.MoveNext();
		
	}
	m_list.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//关闭数据库
	rs.Close();
	db.Close();
}

void CDatabaseDlg2::OnItemchangedFaceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

#if 0
	LPNMITEMACTIVATE lpNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	if(lpNMItemActivate != NULL)
	{
		nItem = lpNMItemActivate ->iTtem;
	}
	lvitem.iItem =nItem;
	lvitem.iSubItem = 0;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	m_list.GetItem(&lvitem);
#endif
	int nIndex;
	CString sname,spath,fname,fpath;
	nIndex = m_list.GetNextItem(-1,LVNI_ALL| LVNI_SELECTED);
	if(-1 == nIndex)
		nIndex = 0;
	sname = m_list.GetItemText(nIndex,1);
	spath = m_list.GetItemText(nIndex,2);
	fname = m_list.GetItemText(nIndex,3);
	fpath = m_list.GetItemText(nIndex,4);
	
	m_sname.SetWindowText(sname);
	m_spath.SetWindowText(spath);
	m_fname.SetWindowText(fname);
	m_fpath.SetWindowText(fpath);
	*pResult = 0;
}


void CDatabaseDlg2::OnBnClickedDeleteFace()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex;
	CString id,fpath;
	CDatabase db;
	db.Open(NULL, FALSE, FALSE, _T("ODBC;DSN=face;UID=root;PWD=root"));
	nIndex = m_list.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	//if(-1 == nIndex)
	//    nIndex = 0;
	if(-1 != nIndex)
	{
		id = m_list.GetItemText(nIndex,0);
		fpath = m_list.GetItemText(nIndex,4);
	}
	else     
	{   
		MessageBox("请选择要删除人脸!");
		db.Close();
		return;
	}

	//删除文件
	CFileFind finder;
	if(finder.FindFile(fpath))
	{
		CFile TempFile; 
		TempFile.Remove(fpath);
	}


	CString sql;
	sql.Format("delete from face where ID=%s", id);
	db.ExecuteSQL(sql);
	db.Close();
	
	CString cmdStr = "Select * from face order by ID";
	m_sname.SetWindowText("");
	m_spath.SetWindowText("");
	m_fname.SetWindowText("");
	m_fpath.SetWindowText("");
	ShowDatabase(cmdStr);
	MessageBox("删除成功");
}


void CDatabaseDlg2::OnBnClickedAlterFace()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex;
	CString sname,spath,fname,fpath,id;
	CDatabase db;
	db.Open(NULL, FALSE, FALSE, _T("ODBC;DSN=face;UID=root;PWD=root"));
	nIndex = m_list.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	//if(-1 == nIndex)
	//    nIndex = 0;
	if(-1 == nIndex)
	{
		MessageBox("请选择要修改的列");
		db.Close();
		return;
	}
	else     
	{  
		id = m_list.GetItemText(nIndex,0);
		m_sname.GetWindowText(sname);
		m_spath.GetWindowText(spath);
		m_fname.GetWindowText(fname);
		m_fpath.GetWindowText(fpath);
	}
	spath.Replace("\\","\\\\");
	fpath.Replace("\\","\\\\");
	CString sql;
	sql.Format("update face set SourceImg='%s' , SourcePath='%s' ,FaceName='%s',FacePath='%s' where ID=%s", sname, spath,fname,fpath,id);
	db.ExecuteSQL(sql);
	db.Close();
	CString cmdStr = "Select * from face order by ID";
	m_sname.SetWindowText("");
	m_spath.SetWindowText("");
	m_fname.SetWindowText("");
	m_fpath.SetWindowText("");
	ShowDatabase(cmdStr);
	MessageBox("修改成功");
}
