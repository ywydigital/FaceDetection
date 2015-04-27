// CatchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "CatchDlg.h"
#include "afxdialogex.h"
#include "FaceCatch.h"
#include "ProDlg.h"
#include "MainFrm.h"
#include "FaceDoc.h"
#include "FaceView.h"


// CCatchDlg 对话框

IMPLEMENT_DYNAMIC(CCatchDlg, CDialogEx)

CCatchDlg::CCatchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCatchDlg::IDD, pParent)
	, m_strDesPath(_T(""))
{

	m_strDesPath = _T("");
}

CCatchDlg::~CCatchDlg()
{
}

void CCatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DESPATH, m_strDesPath);
}


BEGIN_MESSAGE_MAP(CCatchDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OUT, &CCatchDlg::OnBnClickedButtonOut)
	ON_BN_CLICKED(IDOK, &CCatchDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCatchDlg 消息处理程序


void CCatchDlg::OnBnClickedButtonOut()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strDesPath.Empty();
	BROWSEINFO bi;  
	char name[MAX_PATH];  
	ZeroMemory(&bi,sizeof(BROWSEINFO));  
	bi.hwndOwner = GetSafeHwnd();  
	bi.pszDisplayName = name;  
	bi.lpszTitle = "选择输出文件夹";  
	//bi.ulFlags = BIF_USENEWUI;  
	bi.ulFlags = BIF_RETURNFSANCESTORS;  
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);  
	if(idl != NULL)  
	{
		SHGetPathFromIDList(idl, m_strDesPath.GetBuffer(MAX_PATH));  
		m_strDesPath.ReleaseBuffer();
		UpdateData(FALSE);
	 }	
}


void CCatchDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//获得当前的视图和文档
	CFaceView *pView = (CFaceView*)(CFaceView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	CFaceDoc *pDoc = pView->GetDocument();

	this->ShowWindow(SW_HIDE);
	CFaceCatch face(pDoc->m_vFilesName,m_strDesPath);
	
	if(pDoc->m_vFilesName.size() == 0)
	{
		MessageBox("Error：没有可检测的图片！");
		CDialogEx::OnOK();
		return;
	}

	CProDlg *pDlg = new CProDlg();
	pDlg->Create(IDD_PRO_DIALOG,this);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->RedrawWindow();
	face.FaceDetection();

	pDlg->ShowWindow(SW_HIDE);
	MessageBox("执行完毕");
	CDialogEx::OnOK();
}
