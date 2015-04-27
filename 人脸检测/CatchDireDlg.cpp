// CatchDireDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Face.h"
#include "CatchDireDlg.h"
#include "afxdialogex.h"
#include <io.h>
#include <fcntl.h>

// CCatchDireDlg �Ի���

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


// CCatchDireDlg ��Ϣ�������


HBRUSH CCatchDireDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_TEXT)
	{
		pDC->SetTextColor(RGB(110,110,110));
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CCatchDireDlg::OnBnClickedButtonDire()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_detectDire.Empty();
	BROWSEINFO bi;  
	char name[MAX_PATH];  
	ZeroMemory(&bi,sizeof(BROWSEINFO));  
	bi.hwndOwner = GetSafeHwnd();  
	bi.pszDisplayName = name;  
	bi.lpszTitle = "ѡ������ļ���";  
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_desPath.Empty();
	BROWSEINFO bi;  
	char name[MAX_PATH];  
	ZeroMemory(&bi,sizeof(BROWSEINFO));  
	bi.hwndOwner = GetSafeHwnd();  
	bi.pszDisplayName = name;  
	bi.lpszTitle = "ѡ������ļ���";  
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
	printf("���ڵ��ÿ���̨�������Ŷ\n");
	FreeConsole();
}

void CCatchDireDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char   path[MAX_PATH];   
	GetCurrentDirectory(MAX_PATH,path);
	//��ü��exe����Ŀ¼
	CString temp(path);
	//CString exePath = (temp).Mid(0,(temp).ReverseFind('\\')+1) + "DetectDire\\Debug\\DetectDire.exe";
	CString exePath = "..\\DetectDire\\Debug\\DetectDire.exe";

	CString value;
	value.Format("%s %s %s %f %d %d %d",m_detectDire,m_desPath,g_cascade_name,g_scale_factor,g_min_neighbors,g_min_size_w,g_min_size_h);
	ShellExecute((HWND)AfxGetMainWnd(),"open",exePath,value,NULL,SW_SHOW );
	CDialogEx::OnOK();
}
