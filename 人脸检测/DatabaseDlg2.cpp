// DatabaseDlg2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Face.h"
#include "DatabaseDlg2.h"
#include "afxdialogex.h"


// CDatabaseDlg2 �Ի���

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
	m_list.InsertColumn(1,"Դͼ����",LVCFMT_LEFT,90);
	m_list.InsertColumn(2,"Դͼ��·��",LVCFMT_LEFT,120);
	m_list.InsertColumn(3,"����ͼ����",LVCFMT_LEFT,90);
	m_list.InsertColumn(4,"����ͼ��·��",LVCFMT_LEFT,120);
	m_list.InsertColumn(5,"����ʱ��",LVCFMT_LEFT,120);
	CString cmdStr = "Select * from face order by ID";
	ShowDatabase(cmdStr);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDatabaseDlg2, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FACE_LIST, &CDatabaseDlg2::OnItemchangedFaceList)
	ON_BN_CLICKED(IDC_DELETE_FACE, &CDatabaseDlg2::OnBnClickedDeleteFace)
	ON_BN_CLICKED(IDC_ALTER_FACE, &CDatabaseDlg2::OnBnClickedAlterFace)
END_MESSAGE_MAP()


// CDatabaseDlg2 ��Ϣ�������
void CDatabaseDlg2::ShowDatabase(CString cmdStr)
{
	m_list.DeleteAllItems();
	//�����ݿ�
	CDatabase db;
	db.Open(NULL,FALSE,FALSE,"ODBC;DSN=face;UID=root;PWD=root");
	//������ѯ������ռ�
	CRecordset rs(&db);
	rs.Open(CRecordset::forwardOnly,(L"%s",cmdStr));
	int i = 0;
	//������ѯ���
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
	//�ر����ݿ�
	rs.Close();
	db.Close();
}

void CDatabaseDlg2::OnItemchangedFaceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("��ѡ��Ҫɾ������!");
		db.Close();
		return;
	}

	//ɾ���ļ�
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
	MessageBox("ɾ���ɹ�");
}


void CDatabaseDlg2::OnBnClickedAlterFace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex;
	CString sname,spath,fname,fpath,id;
	CDatabase db;
	db.Open(NULL, FALSE, FALSE, _T("ODBC;DSN=face;UID=root;PWD=root"));
	nIndex = m_list.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	//if(-1 == nIndex)
	//    nIndex = 0;
	if(-1 == nIndex)
	{
		MessageBox("��ѡ��Ҫ�޸ĵ���");
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
	MessageBox("�޸ĳɹ�");
}
