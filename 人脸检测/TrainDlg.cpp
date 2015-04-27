// TrainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Face.h"
#include "TrainDlg.h"
#include "afxdialogex.h"
#include "cv.h"
#include "highgui.h"


// CTrainDlg �Ի���

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


// CTrainDlg ��Ϣ�������


void CTrainDlg::OnBnClickedTrainHelp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString path = "..\\help\\help.doc";
	ShellExecute((HWND)AfxGetMainWnd(),"open",path,NULL,NULL,SW_SHOW);

}


void CTrainDlg::OnBnClickedOpenClass()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString path = "..\\classifier";
	ShellExecute((HWND)AfxGetMainWnd(),"explore",path,NULL,NULL,SW_SHOW);
}


void CTrainDlg::OnBnClickedInputPositive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_positive.Empty();
	BROWSEINFO bi;  
	char name[MAX_PATH];  
	ZeroMemory(&bi,sizeof(BROWSEINFO));  
	bi.hwndOwner = GetSafeHwnd();  
	bi.pszDisplayName = name;  
	bi.lpszTitle = "ѡ���������ļ���";  
	//bi.ulFlags = BIF_USENEWUI;  
	bi.ulFlags = BIF_RETURNFSANCESTORS;  
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);  
	if(idl != NULL)  
	{
		SHGetPathFromIDList(idl, m_positive.GetBuffer(MAX_PATH));  
		m_positive.ReleaseBuffer();
		UpdateData(FALSE);
	 }

	MessageBox("�������ļ��У�"+m_positive);
}


void CTrainDlg::OnBnClickedInputNegetive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	m_negetive.Empty();
	BROWSEINFO bi;  
	char name[MAX_PATH];  
	ZeroMemory(&bi,sizeof(BROWSEINFO));  
	bi.hwndOwner = GetSafeHwnd();  
	bi.pszDisplayName = name;  
	bi.lpszTitle = "ѡ�������ļ���";  
	//bi.ulFlags = BIF_USENEWUI;  
	bi.ulFlags = BIF_RETURNFSANCESTORS;  
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);  
	if(idl != NULL)  
	{
		SHGetPathFromIDList(idl, m_negetive.GetBuffer(MAX_PATH));  
		m_negetive.ReleaseBuffer();
		UpdateData(FALSE);
	 }

	//���������������ļ�
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

	MessageBox("�������ļ��У�"+m_negetive);

}


void CTrainDlg::OnBnClickedNormal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

	//���������������ļ�
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
			//��������ͬһת��Ϊ 24*24��С���ļ�
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
	MessageBox("������һ���ɹ�");
}


void CTrainDlg::OnBnClickedCreateInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString exePath = "..\\createsamples\\Debug\\createsamples.exe";

	CString value;
	value.Format("-info %s -bg %s -vec %s -num %d -w %d -h %d",m_info,m_bg,m_vec,m_iPositiveNum,m_iImgWidth,m_iImgHeight);
	ShellExecute((HWND)AfxGetMainWnd(),"open",exePath,value,NULL,SW_SHOW );
}


void CTrainDlg::OnBnClickedTraining()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString exePath = "..\\haartraining\\Debug\\haartraining.exe";

	CString value;

	m_iSplits = 2;
	m_iStages = 4;

	value.Format("-data %s -vec %s -bg %s -npos %d -nneg %d -nsplits %d -nstages %d  -nonsym -mem 512 -w %d -h %d -mode all",
		m_data,m_vec,m_bg,m_iPositiveNum,m_iNegetiveNum,m_iSplits,m_iStages,m_iImgWidth,m_iImgHeight);
	ShellExecute((HWND)AfxGetMainWnd(),"open",exePath,value,NULL,SW_SHOW );
 
	CFileFind filefinder;
 
	CString SourcePath;
	//SourcePath.Format("E:\\ѵ��������\\result.xml");
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
		 desc = "ѵ��������";

		 if(name == "" || path == "" || desc == "")
		 {
			 MessageBox("�������Բ���Ϊ��");
			 db.Close();
			 return;
		 }
	 
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
		 MessageBox("��ӳɹ�");
 
	}
 
	filefinder.Close();

}
