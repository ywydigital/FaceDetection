
// FaceDoc.cpp : CFaceDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Face.h"
#endif

#include "FaceDoc.h"
#include "FaceView.h"
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFaceDoc

IMPLEMENT_DYNCREATE(CFaceDoc, CDocument)

BEGIN_MESSAGE_MAP(CFaceDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CFaceDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_ADD, &CFaceDoc::OnFileAdd)
	ON_COMMAND(ID_FILE_DELETE, &CFaceDoc::OnFileDelete)
	ON_COMMAND(ID_FILE_CLEAN, &CFaceDoc::OnFileClean)
END_MESSAGE_MAP()


// CFaceDoc 构造/析构

CFaceDoc::CFaceDoc()
{
	// TODO: 在此添加一次性构造代码

}

CFaceDoc::~CFaceDoc()
{
}

BOOL CFaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CFaceDoc 序列化

void CFaceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CFaceDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CFaceDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CFaceDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CFaceDoc 诊断

#ifdef _DEBUG
void CFaceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFaceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFaceDoc 命令


void CFaceDoc::OnFileOpen()
{
	//！！！！选择多个文件时，要设置缓冲区大小，要不不能选太多文件
	CFileDialog dlgOpen(TRUE,NULL , NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
         "JPG文件(*.jpg)|*.jpg|",NULL);
	//最多可以打开1000个文件
	dlgOpen.m_ofn.nMaxFile = 1000 * MAX_PATH; 
	dlgOpen.m_ofn.lpstrFile = new TCHAR[dlgOpen.m_ofn.nMaxFile];  
	ZeroMemory(dlgOpen.m_ofn.lpstrFile, sizeof(TCHAR) * dlgOpen.m_ofn.nMaxFile); 

	// TODO: 在此添加命令处理程序代码
	//CFileDialog dlgOpen(TRUE,NULL,NULL,
		//OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		//"JPG文件(*.jpg)|*.jpg|",NULL,0,TRUE);
	if(dlgOpen.DoModal()== IDOK)
	{
		m_vFilesName.clear();
		POSITION p = dlgOpen.GetStartPosition();
		while(p!=NULL)
		{
			CString strTemp = dlgOpen.GetNextPathName(p);
			m_vFilesName.push_back(strTemp);
		}
	}
	if(m_vFilesName.size()!=0)
		ShowSelectedImages(FILE_OPEN );

	delete[] dlgOpen.m_ofn.lpstrFile; 
}

void CFaceDoc::ShowSelectedImages(int iAddType)
{
	//获得当前的文档对应的视图
	CFaceView* pView = (CFaceView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	
	//终止缩略图加载线程
	if(pView->IsRunning())
	{
		pView->TerminateLoadThread();
	}

	pView->LoadThumbnailImages(iAddType);
}


void CFaceDoc::OnFileAdd()
{

	// TODO: 在此添加命令处理程序代码

	CFileDialog dlgOpen(TRUE,NULL , NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
         "JPG文件(*.jpg)|*.jpg|",NULL);
	//最多可以打开1000个文件
	dlgOpen.m_ofn.nMaxFile = 1000 * MAX_PATH; 
	dlgOpen.m_ofn.lpstrFile = new TCHAR[dlgOpen.m_ofn.nMaxFile];  
	ZeroMemory(dlgOpen.m_ofn.lpstrFile, sizeof(TCHAR) * dlgOpen.m_ofn.nMaxFile); 

	if(dlgOpen.DoModal()== IDOK)
	{
		POSITION p = dlgOpen.GetStartPosition();
		while(p!=NULL)
		{
			CString strTemp = dlgOpen.GetNextPathName(p);
			m_vFilesName.push_back(strTemp);
		}
	}
	//CString str="";
	//str.Format("%d",m_vFilesName.size());
	//AfxMessageBox(str);
	if(m_vFilesName.size()!=0)
		ShowSelectedImages(FILE_ADD);

	delete[] dlgOpen.m_ofn.lpstrFile; 

}


void CFaceDoc::OnFileDelete()
{
	// TODO: 在此添加命令处理程序代码
	CFaceView* pView = (CFaceView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	CListCtrl& ListCtrl  = pView->GetListCtrl();

	int   nIndex   =   ListCtrl.GetNextItem   (-1,   LVNI_SELECTED);
	std::vector<CString>::iterator iter = m_vFilesName.begin();
	iter = iter + nIndex;
	m_vFilesName.erase(iter);
	pView->DeleteImage(nIndex);
}


void CFaceDoc::OnFileClean()
{
	CFaceView* pView = (CFaceView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	CListCtrl& ListCtrl  = pView->GetListCtrl();
	CImageList *pImgList = &pView->m_ImageListThumb;
	// TODO: 在此添加命令处理程序代码
	int iImageCount = pImgList->GetImageCount();

	for(int i = iImageCount-1;i >= 0; i--)
	{
		pImgList->Remove(i);
	}
	ListCtrl.DeleteAllItems();
	m_vFilesName.clear();
}
