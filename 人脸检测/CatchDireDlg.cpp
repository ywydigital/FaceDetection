// CatchDireDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "CatchDireDlg.h"
#include "afxdialogex.h"
#include <io.h>
#include <fcntl.h>

// CCatchDireDlg 对话框

IMPLEMENT_DYNAMIC(CCatchDireDlg, CDialogEx)

CCatchDireDlg::CCatchDireDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCatchDireDlg::IDD, pParent)
	, m_detectDire(_T(""))
	, m_desPath(_T(""))
{

}

CCatchDireDlg::~CCatchDireDlg()
{
}

BOOL CCatchDireDlg::OnInitDialog()
{
	return CDialogEx::OnInitDialog();
}
void CCatchDireDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DETECT_DIRE, m_detectDire);
	DDX_Text(pDX, IDC_OUT_PATH, m_desPath);
}


BEGIN_MESSAGE_MAP(CCatchDireDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_DIRE, &CCatchDireDlg::OnBnClickedButtonDire)
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CCatchDireDlg::OnBnClickedButtonPath)
	ON_BN_CLICKED(IDOK, &CCatchDireDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCatchDireDlg 消息处理程序


HBRUSH CCatchDireDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_TEXT)
	{
		pDC->SetTextColor(RGB(110,110,110));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CCatchDireDlg::OnBnClickedButtonDire()
{
	// TODO: 在此添加控件通知处理程序代码
	m_detectDire.Empty();
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
		SHGetPathFromIDList(idl, m_detectDire.GetBuffer(MAX_PATH));  
		m_detectDire.ReleaseBuffer();
		UpdateData(FALSE);
	 }	

}


void CCatchDireDlg::OnBnClickedButtonPath()
{
	// TODO: 在此添加控件通知处理程序代码
	m_desPath.Empty();
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
		SHGetPathFromIDList(idl, m_desPath.GetBuffer(MAX_PATH));  
		m_desPath.ReleaseBuffer();
		UpdateData(FALSE);
	 }	
}

void detect_and_catch()
{
	AllocConsole();
	freopen( "CONOUT$","w",stdout);
	freopen("CONIN$","r",stdin);
	printf("我在调用控制台输出程序哦\n");
	FreeConsole();
}

void CCatchDireDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	char   path[MAX_PATH];   
	GetCurrentDirectory(MAX_PATH,path);
	//获得监控exe所在目录
	CString temp(path);
	//CString exePath = (temp).Mid(0,(temp).ReverseFind('\\')+1) + "DetectDire\\Debug\\DetectDire.exe";
	CString exePath = "..\\DetectDire\\Debug\\DetectDire.exe";

	CString value;
	value.Format("%s %s %s %f %d %d %d",m_detectDire,m_desPath,g_cascade_name,g_scale_factor,g_min_neighbors,g_min_size_w,g_min_size_h);
	ShellExecute((HWND)AfxGetMainWnd(),"open",exePath,value,NULL,SW_SHOW );
	CDialogEx::OnOK();
}
