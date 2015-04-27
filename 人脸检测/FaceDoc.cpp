
// FaceDoc.cpp : CFaceDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CFaceDoc ����/����

CFaceDoc::CFaceDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CFaceDoc::~CFaceDoc()
{
}

BOOL CFaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CFaceDoc ���л�

void CFaceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CFaceDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CFaceDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// CFaceDoc ���

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


// CFaceDoc ����


void CFaceDoc::OnFileOpen()
{
	//��������ѡ�����ļ�ʱ��Ҫ���û�������С��Ҫ������ѡ̫���ļ�
	CFileDialog dlgOpen(TRUE,NULL , NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
         "JPG�ļ�(*.jpg)|*.jpg|",NULL);
	//�����Դ�1000���ļ�
	dlgOpen.m_ofn.nMaxFile = 1000 * MAX_PATH; 
	dlgOpen.m_ofn.lpstrFile = new TCHAR[dlgOpen.m_ofn.nMaxFile];  
	ZeroMemory(dlgOpen.m_ofn.lpstrFile, sizeof(TCHAR) * dlgOpen.m_ofn.nMaxFile); 

	// TODO: �ڴ���������������
	//CFileDialog dlgOpen(TRUE,NULL,NULL,
		//OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		//"JPG�ļ�(*.jpg)|*.jpg|",NULL,0,TRUE);
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
	//��õ�ǰ���ĵ���Ӧ����ͼ
	CFaceView* pView = (CFaceView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	
	//��ֹ����ͼ�����߳�
	if(pView->IsRunning())
	{
		pView->TerminateLoadThread();
	}

	pView->LoadThumbnailImages(iAddType);
}


void CFaceDoc::OnFileAdd()
{

	// TODO: �ڴ���������������

	CFileDialog dlgOpen(TRUE,NULL , NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
         "JPG�ļ�(*.jpg)|*.jpg|",NULL);
	//�����Դ�1000���ļ�
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	int iImageCount = pImgList->GetImageCount();

	for(int i = iImageCount-1;i >= 0; i--)
	{
		pImgList->Remove(i);
	}
	ListCtrl.DeleteAllItems();
	m_vFilesName.clear();
}
