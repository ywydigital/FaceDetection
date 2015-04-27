// TrainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "TrainDlg.h"
#include "afxdialogex.h"
#include "cv.h"
#include "highgui.h"


// CTrainDlg 对话框

IMPLEMENT_DYNAMIC(CTrainDlg, CDialogEx)

CTrainDlg::CTrainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrainDlg::IDD, pParent)
{

}

CTrainDlg::~CTrainDlg()
{
}

void CTrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTrainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_TRAIN_HELP, &CTrainDlg::OnBnClickedTrainHelp)
	ON_BN_CLICKED(IDC_OPEN_CLASS, &CTrainDlg::OnBnClickedOpenClass)
	ON_BN_CLICKED(IDC_INPUT_POSITIVE, &CTrainDlg::OnBnClickedInputPositive)
	ON_BN_CLICKED(IDC_INPUT_NEGETIVE, &CTrainDlg::OnBnClickedInputNegetive)
	ON_BN_CLICKED(IDC_NORMAL, &CTrainDlg::OnBnClickedNormal)
	ON_BN_CLICKED(IDC_CREATE_INFO, &CTrainDlg::OnBnClickedCreateInfo)
	ON_BN_CLICKED(IDC_TRAINING, &CTrainDlg::OnBnClickedTraining)
END_MESSAGE_MAP()


// CTrainDlg 消息处理程序


void CTrainDlg::OnBnClickedTrainHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path = "..\\help\\help.doc";
	ShellExecute((HWND)AfxGetMainWnd(),"open",path,NULL,NULL,SW_SHOW);

}


void CTrainDlg::OnBnClickedOpenClass()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path = "..\\classifier";
	ShellExecute((HWND)AfxGetMainWnd(),"explore",path,NULL,NULL,SW_SHOW);
}


void CTrainDlg::OnBnClickedInputPositive()
{
	// TODO: 在此添加控件通知处理程序代码
	m_positive.Empty();
	BROWSEINFO bi;  
	char name[MAX_PATH];  
	ZeroMemory(&bi,sizeof(BROWSEINFO));  
	bi.hwndOwner = GetSafeHwnd();  
	bi.pszDisplayName = name;  
	bi.lpszTitle = "选择正样本文件夹";  
	//bi.ulFlags = BIF_USENEWUI;  
	bi.ulFlags = BIF_RETURNFSANCESTORS;  
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);  
	if(idl != NULL)  
	{
		SHGetPathFromIDList(idl, m_positive.GetBuffer(MAX_PATH));  
		m_positive.ReleaseBuffer();
		UpdateData(FALSE);
	 }

	MessageBox("正样本文件夹："+m_positive);
}


void CTrainDlg::OnBnClickedInputNegetive()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_negetive.Empty();
	BROWSEINFO bi;  
	char name[MAX_PATH];  
	ZeroMemory(&bi,sizeof(BROWSEINFO));  
	bi.hwndOwner = GetSafeHwnd();  
	bi.pszDisplayName = name;  
	bi.lpszTitle = "选择负样本文件夹";  
	//bi.ulFlags = BIF_USENEWUI;  
	bi.ulFlags = BIF_RETURNFSANCESTORS;  
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);  
	if(idl != NULL)  
	{
		SHGetPathFromIDList(idl, m_negetive.GetBuffer(MAX_PATH));  
		m_negetive.ReleaseBuffer();
		UpdateData(FALSE);
	 }

	//产生负样本描述文件
	CString filePath;
	filePath.Format("%s\\bg.txt",m_negetive.Mid(0,m_negetive.ReverseFind('\\')));
	CFile file(filePath,CFile::modeCreate | CFile::modeWrite);
	
	m_bg = filePath;
	m_iNegetiveNum = 0;

	CString direName = m_negetive.Mid(m_negetive.ReverseFind('\\')+1,m_negetive.GetLength()-m_negetive.ReverseFind('\\')-1);

    CFileFind finder;  
    BOOL bWorking = finder.FindFile(m_negetive+"\\*.*");  
    while(bWorking)  
    {  
	    bWorking=finder.FindNextFile();  
		CString str;
		if(finder.GetFileName() != '.' && finder.GetFileName() != "..")
		{
			if(bWorking)
			{
				str.Format("%s\\%s\r\n",direName,finder.GetFileName());
			}
			else
			{
				str.Format("%s\\%s",direName,finder.GetFileName());
			}
			file.Write(str,str.GetLength());
			m_iNegetiveNum++;
		}
	}  
	UpdateData(FALSE);
	file.Close();
	finder.Close();

	MessageBox("负样本文件夹："+m_negetive);

}


void CTrainDlg::OnBnClickedNormal()
{
	// TODO: 在此添加控件通知处理程序代码
	

	//产生正样本描述文件
	CString filePath;
	filePath.Format("%s\\info.dat",m_positive.Mid(0,m_positive.ReverseFind('\\')));
	CFile file(filePath,CFile::modeCreate | CFile::modeWrite);
	
	m_info = filePath;
	m_vec.Format("%s\\positive.vec",m_positive.Mid(0,m_positive.ReverseFind('\\')));
	m_data.Format("%s\\result",m_positive.Mid(0,m_positive.ReverseFind('\\')));
	m_iPositiveNum = 0;
	m_iImgWidth = 24;
	m_iImgHeight = 24;

	CString direName = m_positive.Mid(m_positive.ReverseFind('\\')+1,m_positive.GetLength()-m_positive.ReverseFind('\\')-1);

    CFileFind finder;  
    BOOL bWorking = finder.FindFile(m_positive+"\\*.*");  
    while(bWorking)  
    {  
	    bWorking=finder.FindNextFile();  
		CString str;
		if(finder.GetFileName() != '.' && finder.GetFileName() != "..")
		{
			//将正样本同一转换为 24*24大小的文件
			IplImage* src;
			if( (src = cvLoadImage(finder.GetFilePath(),CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR))!= 0)
			{
				IplImage* small_img = cvCreateImage(cvSize(24,24),src->depth,src->nChannels);
				cvResize( src, small_img, CV_INTER_LINEAR );
				cvSaveImage(finder.GetFilePath(),small_img);
				if(bWorking)
				{
					str.Format("%s\\%s 1 0 0 24 24\r\n",direName,finder.GetFileName());
				}
				else
				{
					str.Format("%s\\%s 1 0 0 24 24",direName,finder.GetFileName());
				}
				file.Write(str,str.GetLength());
				m_iPositiveNum++;
				
				cvReleaseImage( &small_img );
			}
			cvReleaseImage( &src );
		}
	}  
	UpdateData(FALSE);
	file.Close();
	finder.Close();
	MessageBox("样本归一化成功");
}


void CTrainDlg::OnBnClickedCreateInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString exePath = "..\\createsamples\\Debug\\createsamples.exe";

	CString value;
	value.Format("-info %s -bg %s -vec %s -num %d -w %d -h %d",m_info,m_bg,m_vec,m_iPositiveNum,m_iImgWidth,m_iImgHeight);
	ShellExecute((HWND)AfxGetMainWnd(),"open",exePath,value,NULL,SW_SHOW );
}


void CTrainDlg::OnBnClickedTraining()
{
	// TODO: 在此添加控件通知处理程序代码
	CString exePath = "..\\haartraining\\Debug\\haartraining.exe";

	CString value;

	m_iSplits = 2;
	m_iStages = 4;

	value.Format("-data %s -vec %s -bg %s -npos %d -nneg %d -nsplits %d -nstages %d  -nonsym -mem 512 -w %d -h %d -mode all",
		m_data,m_vec,m_bg,m_iPositiveNum,m_iNegetiveNum,m_iSplits,m_iStages,m_iImgWidth,m_iImgHeight);
	ShellExecute((HWND)AfxGetMainWnd(),"open",exePath,value,NULL,SW_SHOW );
 
	CFileFind filefinder;
 
	CString SourcePath;
	//SourcePath.Format("E:\\训练分类器\\result.xml");
	SourcePath.Format("%s\\result.xml",m_data.Mid(0,m_positive.ReverseFind('\\')));
 
	CString filename;
 
	BOOL bfind = filefinder.FindFile(SourcePath);
 
	CString DisPath;
 
	if (bfind)
 
	{
 
	   bfind = filefinder.FindNextFile();
 
	   //filename = filefinder.GetFileName();
 
	   DisPath = "..\\classifier\\result.xml";
 
	   CopyFile(SourcePath,DisPath,1);

	    CDatabase db;

		 db.Open(NULL, FALSE, FALSE, _T("ODBC;DSN=face;UID=root;PWD=root"));
		 CString name, path, desc,str;
		 name = "result.xml";
		 path = "classifier\\\\result.xml";
		 desc = "训练分类器";

		 if(name == "" || path == "" || desc == "")
		 {
			 MessageBox("所有属性不能为空");
			 db.Close();
			 return;
		 }
	 
		 //判断该目录下分类器是否存在
		 CFileFind finder;
		 if( !finder.FindFile("..\\"+path))
		 {
			 MessageBox("该分类器不存在");
			 db.Close();
			 return;
		 }

		 str.Format("insert into classifier(cname,cpath,cdescribe)  values('%s','%s','%s')", name, path, desc);
		 db.ExecuteSQL(str);
		 db.Close();
		 MessageBox("添加成功");
 
	}
 
	filefinder.Close();

}
