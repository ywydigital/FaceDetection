// DatabaseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Face.h"
#include "DatabaseDlg.h"
#include "afxdialogex.h"


// CDatabaseDlg �Ի���

IMPLEMENT_DYNAMIC(CDatabaseDlg, CDialogEx)

CDatabaseDlg::CDatabaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDatabaseDlg::IDD, pParent)
{

}

CDatabaseDlg::~CDatabaseDlg()
{
}

void CDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLASSIFIER, m_list);
	DDX_Control(pDX, IDC_CNAME, m_name);
	DDX_Control(pDX, IDC_CPATH, m_path);
	DDX_Control(pDX, IDC_CDESC, m_desc);
}

BOOL CDatabaseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_list.InsertColumn(0,"ID",LVCFMT_LEFT,40);
	m_list.InsertColumn(1,"��������",LVCFMT_LEFT,150);
	m_list.InsertColumn(2,"�洢·��",LVCFMT_LEFT,190);
	m_list.InsertColumn(3,"����",LVCFMT_LEFT,150);
	CString cmdStr = "Select * from classifier order by ClassifierID";
	ShowDatabase(cmdStr);
	return true;
}
BEGIN_MESSAGE_MAP(CDatabaseDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CLASSIFIER, &CDatabaseDlg::OnItemchangedClassifier)
	ON_BN_CLICKED(IDC_ADD_CLASSIFIER, &CDatabaseDlg::OnBnClickedAddClassifier)
	ON_BN_CLICKED(IDC_DELETE_CLASSIFIER2, &CDatabaseDlg::OnBnClickedDeleteClassifier2)
	ON_BN_CLICKED(IDC_ALTER_CLASSIFIER, &CDatabaseDlg::OnBnClickedAlterClassifier)
END_MESSAGE_MAP()


// CDatabaseDlg ��Ϣ�������

void CDatabaseDlg::ShowDatabase(CString cmdStr)
{
	m_list.DeleteAllItems();
	CDatabase db;
	db.Open(NULL,FALSE,FALSE,"ODBC;DSN=face;UID=root;PWD=root");
	CRecordset rs(&db);
	rs.Open(CRecordset::forwardOnly,(L"%s",cmdStr));
	int i = 0;
	while(!rs.IsEOF())
	{
		CString id;
		rs.GetFieldValue("ClassifierID",id);
		m_list.InsertItem(i,id);
		CString cname;
		rs.GetFieldValue("cname",cname);
		m_list.SetItemText(i,1,cname);
		CString path;
		rs.GetFieldValue("cpath",path);
		m_list.SetItemText(i,2,path);
		CString describe;
		rs.GetFieldValue("cdescribe",describe);
		m_list.SetItemText(i,3,describe);
		i++;

		rs.MoveNext();
		
	}
	m_list.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	rs.Close();
	db.Close();
}


void CDatabaseDlg::OnItemchangedClassifier(NMHDR *pNMHDR, LRESULT *pResult)
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
	m_control_edit_name.SetWindowText(lvitem.pszText);
#endif
	int nIndex;
	CString id,name,path,describe;
	nIndex = m_list.GetNextItem(-1,LVNI_ALL| LVNI_SELECTED);
	if(-1 == nIndex)
		nIndex = 0;
	id = m_list.GetItemText(nIndex,0);
	name = m_list.GetItemText(nIndex,1);
	path = m_list.GetItemText(nIndex,2);
	describe = m_list.GetItemText(nIndex,3);
	
	m_name.SetWindowText(name);
	m_path.SetWindowText(path);
	m_desc.SetWindowText(describe);
	*pResult = 0;
}


void CDatabaseDlg::OnBnClickedAddClassifier()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 CDatabase db;
	 db.Open(NULL, FALSE, FALSE, _T("ODBC;DSN=face;UID=root;PWD=root"));
	 CString name, path, desc,str;
	 m_name.GetWindowText(name);
	 m_path.GetWindowText(path);
	 m_desc.GetWindowText(desc);
	 if(name == "" || path == "" || desc == "")
	 {
		 MessageBox("�������Բ���Ϊ��");
		 db.Close();
		 return;
	 }
	 path.Replace("\\","\\\\");
	 
	 //�жϸ�Ŀ¼�·������Ƿ����
	 CFileFind finder;
	 if( !finder.FindFile("..\\"+path))
	 {
		 MessageBox("�÷�����������");
		 db.Close();
		 return;
	 }

	 str.Format("insert into classifier(cname,cpath,cdescribe)  values('%s','%s','%s')", name, path, desc);
	 db.ExecuteSQL(str);
	 db.Close();
	 CString cmdStr = "Select * from classifier order by ClassifierID";
	 ShowDatabase(cmdStr);
	 MessageBox("��ӳɹ�");
}


void CDatabaseDlg::OnBnClickedDeleteClassifier2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex;
	CString path,id;
	CDatabase db;
	db.Open(NULL, FALSE, FALSE, _T("ODBC;DSN=face;UID=root;PWD=root"));
	nIndex = m_list.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	//if(-1 == nIndex)
	//    nIndex = 0;
	if(-1 != nIndex)
	{
		id = m_list.GetItemText(nIndex,0);
		path = m_list.GetItemText(nIndex,2);
	}
	else     
	{     
		MessageBox("��ѡ��Ҫɾ���ķ�����!");
		db.Close();
		return;
	}

	//ɾ���ļ�
	CFileFind finder;
	if(finder.FindFile("..\\"+path))
	{
		CFile TempFile; 
		TempFile.Remove("..\\"+path);
	}

	CString sql;
	sql.Format("delete from classifier where ClassifierID=%s", id);
	db.ExecuteSQL(sql);
	db.Close();
	CString cmdStr = "Select * from classifier order by ClassifierID";
	m_name.SetWindowText("");
	m_path.SetWindowText("");
	m_desc.SetWindowText("");
	ShowDatabase(cmdStr);
	MessageBox("ɾ���ɹ�");
}


void CDatabaseDlg::OnBnClickedAlterClassifier()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex;
	CString name,path,desc,id;
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
		m_name.GetWindowText(name);
		m_path.GetWindowText(path);
		m_desc.GetWindowText(desc);
	}
	path.Replace("\\","\\\\");

	CFileFind finder;
	if( !finder.FindFile("..\\"+path))
	{
		MessageBox("�÷�����������");
		db.Close();
		return;
	}
	
	CString sql;
	sql.Format("update classifier set cname='%s' , cpath='%s' ,cdescribe='%s' where ClassifierID=%s", name, path,desc,id);
	db.ExecuteSQL(sql);
	db.Close();
	CString cmdStr = "Select * from classifier order by ClassifierID";
	m_name.SetWindowText("");
	m_path.SetWindowText("");
	m_desc.SetWindowText("");
	ShowDatabase(cmdStr);
	MessageBox("�޸ĳɹ�");
}
