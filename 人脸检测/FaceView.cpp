
// FaceView.cpp : CFaceView 类的实现
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
#include "CatchDlg.h"
#include "CatchDireDlg.h"
#include "CatchVideoDlg.h"
#include "TrainDlg.h"
#include "DataDlg.h"
#include "SetDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 运行模式类型枚举
enum ShowType
{
	MODE_IMAGE		= 0,	// 图片检测模式
	MODE_DIRE		= 1,	// 目录监控模式
	MODE_VIDEO		= 2,	// 视频监控模式
};

// CFaceView

IMPLEMENT_DYNCREATE(CFaceView, CListView)

BEGIN_MESSAGE_MAP(CFaceView, CListView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_VIEW_REFRESH, &CFaceView::OnViewRefresh)
		ON_NOTIFY_REFLECT(NM_DBLCLK, &CFaceView::OnNMDblclk)
	ON_COMMAND(ID_FACE_CATCH, &CFaceView::OnFaceCatch)
	ON_COMMAND(ID_MODE_IMAGE, &CFaceView::OnModeImage)
	ON_UPDATE_COMMAND_UI(ID_MODE_IMAGE, &CFaceView::OnUpdateModeImage)
	ON_COMMAND(ID_MODE_DIRE, &CFaceView::OnModeDire)
	ON_UPDATE_COMMAND_UI(ID_MODE_DIRE, &CFaceView::OnUpdateModeDire)
	ON_COMMAND(ID_MODE_VIDEO, &CFaceView::OnModeVideo)
	ON_UPDATE_COMMAND_UI(ID_MODE_VIDEO, &CFaceView::OnUpdateModeVideo)
	ON_UPDATE_COMMAND_UI(ID_FILE_ADD, &CFaceView::OnUpdateFileAdd)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLEAN, &CFaceView::OnUpdateFileClean)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE, &CFaceView::OnUpdateFileDelete)
	ON_COMMAND(ID_SET, &CFaceView::OnSet)
	ON_COMMAND(ID_TRAIN, &CFaceView::OnTrain)
	ON_COMMAND(ID_DATABASE, &CFaceView::OnDatabase)
END_MESSAGE_MAP()

// CFaceView 构造/析构

CFaceView::CFaceView()
{
	// TODO: 在此处添加构造代码
	m_bRunning = FALSE;
	m_bTerminate = FALSE;
	m_hThreadLoad = NULL;
	m_nRunMode = MODE_IMAGE;

}

CFaceView::~CFaceView()
{
	::CloseHandle( m_hThreadLoad ); 
}

BOOL CFaceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CListView::PreCreateWindow(cs);
}

void CFaceView::OnDraw(CDC* pDC)
{
	CFaceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDC);

	BOOL bPrinting = pDC->IsPrinting();
}

void CFaceView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	//  从而可以用项填充 ListView。
	CListCtrl& ListCtrl = GetListCtrl();

	m_ImageListThumb.Create(THUMBNAIL_WIDTH,
							THUMBNAIL_HEIGHT,
							ILC_COLOR32,
							0,
							1);
	ListCtrl.SetImageList(&m_ImageListThumb,LVSIL_NORMAL);


}
void CFaceView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CFaceView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CtestListVewView 诊断

#ifdef _DEBUG
void CFaceView::AssertValid() const
{
	CListView::AssertValid();
}

void CFaceView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CFaceDoc* CFaceView::GetDocument() const// 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFaceDoc)));
	return (CFaceDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CFaceView::Create( LPCTSTR        lpszClassName, 
							 LPCTSTR        lpszWindowName, 
							 DWORD          dwStyle, 
							 const RECT&    rect, 
							 CWnd           *pParentWnd,
							 UINT           nID, 
							 CCreateContext *pContext) 
{
	return CListView::Create( lpszClassName, 
		                      _T("ListView"),
							  dwStyle           | 
							  LVS_SHOWSELALWAYS | 
							  LVS_ALIGNTOP      |
							  LVS_ICON          |
							  LVS_SINGLESEL     |
							  LVS_AUTOARRANGE, 
							  rect, 
							  pParentWnd, 
							  nID,
							  pContext );
}

BOOL CFaceView::TerminateLoadThread()
{
	if( m_bRunning == FALSE )
	{
		return TRUE;
	}

	m_bTerminate = TRUE;

	//让视图运行空进程等待新的命令
	
	while( m_bRunning )
	{
		MSG message;
		while (::PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) ) 
		{
			::TranslateMessage( &message );
			::DispatchMessage(  &message );
		}
	}
	
	::CloseHandle( m_hThreadLoad );

	return TRUE;
}

//加载缩略图线程
unsigned long _stdcall RunLoadThumbnailThread(LPVOID lpParam)
{
	int nIndex =0;
	std::vector<CString>::iterator iter;
	//获得当前的视图和文档
	CFaceView *pView = (CFaceView*)lpParam;
	CFaceDoc *pDoc = pView->GetDocument();

	CListCtrl& ListCtrl  = pView->GetListCtrl();
	CImageList *pImgList = &pView->m_ImageListThumb;

	int iImageCount = pImgList->GetImageCount();

	for(int i = iImageCount-1;i >= 0; i--)
	{
		pImgList->Remove(i);
	}
	ListCtrl.DeleteAllItems();
	pImgList->SetImageCount(pDoc->m_vFilesName.size());

	ListCtrl.SetRedraw(FALSE);

	for(iter = pDoc->m_vFilesName.begin();
		iter!=pDoc->m_vFilesName.end() && pView->m_bTerminate != TRUE;
		iter++,nIndex++)
	{
		HBITMAP hbmReturn = NULL;
		Bitmap *bmPhoto = NULL;
		CBitmap Bmp1;

		ListCtrl.InsertItem(nIndex,*iter,nIndex);
		CString path = *iter;
		Bitmap image(path.AllocSysString());

		int sourceWidth = image.GetWidth();
		int sourceHeight = image.GetHeight();

		int destX = 0,
			  destY = 0; 
		  
		  float nPercent  = 0;
		  float nPercentW = ((float)THUMBNAIL_WIDTH/(float)sourceWidth);;
		  float nPercentH = ((float)THUMBNAIL_HEIGHT/(float)sourceHeight);
			  
		  if(nPercentH < nPercentW)
		  {
			  nPercent = nPercentH;
			  destX    = (int)((THUMBNAIL_WIDTH - (sourceWidth * nPercent))/2);
		  }
		  else
		  {
			  nPercent = nPercentW;
			  destY    = (int)((THUMBNAIL_HEIGHT - (sourceHeight * nPercent))/2);
		  }
			  
		  int destWidth  = (int)(sourceWidth * nPercent);
		  int destHeight = (int)(sourceHeight * nPercent);

		  bmPhoto = new Bitmap( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, PixelFormat24bppRGB );
		  bmPhoto->SetResolution(image.GetHorizontalResolution(),image.GetVerticalResolution());

		  Graphics *grPhoto = Graphics::FromImage(bmPhoto);
		  Color colorW(255,255,255,255);
		  grPhoto->Clear(colorW);
		  grPhoto->SetInterpolationMode(InterpolationModeHighQualityBicubic);
		  grPhoto->DrawImage(&image,Rect(destX,destY,destWidth,destHeight));

		  bmPhoto->GetHBITMAP(colorW,&hbmReturn);

		  Bmp1.Attach(hbmReturn);
		  pImgList->Replace(nIndex,&Bmp1,NULL);

		  delete grPhoto;
		  delete bmPhoto;
		  Bmp1.Detach();
		  DeleteObject(hbmReturn);

	}

	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();

	pView->m_bRunning = FALSE;
	pView->m_bTerminate = FALSE;

	pView->m_hThreadLoad = NULL;
	::CloseHandle(pView->m_hThreadLoad);

	return 0;
}

//添加缩略图线程
unsigned long _stdcall RunAddThumbnailThread(LPVOID lpParam)
{
	std::vector<CString>::iterator iter;
	//获得当前的视图和文档
	CFaceView *pView = (CFaceView*)lpParam;
	CFaceDoc *pDoc = pView->GetDocument();

	CListCtrl& ListCtrl  = pView->GetListCtrl();
	CImageList *pImgList = &pView->m_ImageListThumb;

	int iImageCount = pImgList->GetImageCount();
	int nIndex = iImageCount;
	pImgList->SetImageCount(pDoc->m_vFilesName.size());

	ListCtrl.SetRedraw(FALSE);

	for(iter = pDoc->m_vFilesName.begin()+iImageCount;
		iter!=pDoc->m_vFilesName.end() && pView->m_bTerminate != TRUE;
		iter++,nIndex++)
	{
		HBITMAP hbmReturn = NULL;
		Bitmap *bmPhoto = NULL;
		CBitmap Bmp1;

		ListCtrl.InsertItem(nIndex,*iter,nIndex);
		CString path = *iter;
		Bitmap image(path.AllocSysString());

		int sourceWidth = image.GetWidth();
		int sourceHeight = image.GetHeight();

		int destX = 0,
			  destY = 0; 
		  
		  float nPercent  = 0;
		  float nPercentW = ((float)THUMBNAIL_WIDTH/(float)sourceWidth);;
		  float nPercentH = ((float)THUMBNAIL_HEIGHT/(float)sourceHeight);
			  
		  if(nPercentH < nPercentW)
		  {
			  nPercent = nPercentH;
			  destX    = (int)((THUMBNAIL_WIDTH - (sourceWidth * nPercent))/2);
		  }
		  else
		  {
			  nPercent = nPercentW;
			  destY    = (int)((THUMBNAIL_HEIGHT - (sourceHeight * nPercent))/2);
		  }
			  
		  int destWidth  = (int)(sourceWidth * nPercent);
		  int destHeight = (int)(sourceHeight * nPercent);

		  bmPhoto = new Bitmap( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, PixelFormat24bppRGB );
		  bmPhoto->SetResolution(image.GetHorizontalResolution(),image.GetVerticalResolution());

		  Graphics *grPhoto = Graphics::FromImage(bmPhoto);
		  Color colorW(255,255,255,255);
		  grPhoto->Clear(colorW);
		  grPhoto->SetInterpolationMode(InterpolationModeHighQualityBicubic);
		  grPhoto->DrawImage(&image,Rect(destX,destY,destWidth,destHeight));

		  bmPhoto->GetHBITMAP(colorW,&hbmReturn);

		  Bmp1.Attach(hbmReturn);
		  pImgList->Replace(nIndex,&Bmp1,NULL);

		  delete grPhoto;
		  delete bmPhoto;
		  Bmp1.Detach();
		  DeleteObject(hbmReturn);

	}

	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();

	pView->m_bRunning = FALSE;
	pView->m_bTerminate = FALSE;

	pView->m_hThreadLoad = NULL;
	::CloseHandle(pView->m_hThreadLoad);

	return 0;
}

//多线程编程
void CFaceView::LoadThumbnailImages(int iAddType)
{
	if(iAddType == 0)
	{
		m_hThreadLoad = CreateThread(NULL,0,RunLoadThumbnailThread,this,0,&m_dwThreadID);
		m_bRunning = TRUE;
	}
	else
	{
		m_hThreadLoad = CreateThread(NULL,0,RunAddThumbnailThread,this,0,&m_dwThreadID);
		m_bRunning = TRUE;
	}

}

void CFaceView::DeleteImage(int nIndex)
{
	CFaceView* pView = (CFaceView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	
	CImageList *pImgList = &pView->m_ImageListThumb;
	m_ImageListThumb.Remove(nIndex);

	//int i = pImgList->GetImageCount();

	CListCtrl& ListCtrl  = pView->GetListCtrl();
	
	
	ListCtrl.SetRedraw(FALSE);
	ListCtrl.DeleteItem(nIndex);
	for(int i = nIndex;i < ListCtrl.GetItemCount();i++)
	{
		ListCtrl.SetItem(i,0,LVIF_IMAGE,NULL,i,0,0,NULL,0);
	}
	ListCtrl.SetRedraw(TRUE);
}


void CFaceView::OnViewRefresh()
{
	// TODO: 在此添加命令处理程序代码
	//？？？怎么手动刷新视图
}


void CFaceView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CFaceView* pView = (CFaceView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	CListCtrl& ListCtrl  = pView->GetListCtrl();
	int   nIndex   =   ListCtrl.GetNextItem   (-1,   LVNI_SELECTED);
	CString str;
	str.Format(_T("%d"),nIndex);
	MessageBox("Doble Cliked "+str);
	*pResult = 0;
}


void CFaceView::OnFaceCatch()
{
	// TODO: 在此添加命令处理程序代码
	//图片检测模式运行
	if(m_nRunMode == MODE_IMAGE)
	{
		CCatchDlg dlg;
		dlg.DoModal();

	}

	if(m_nRunMode == MODE_DIRE)
	{
		CCatchDireDlg direDlg;
		direDlg.DoModal();
	}

	if(m_nRunMode == MODE_VIDEO)
	{
		CCatchVideoDlg videoDlg;

		videoDlg.DoModal();
	}


}


void CFaceView::OnModeImage()
{
	// TODO: 在此添加命令处理程序代码
	if(m_nRunMode != MODE_IMAGE)
	{
		m_nRunMode = MODE_IMAGE;
	}
}


void CFaceView::OnUpdateModeImage(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_nRunMode == MODE_IMAGE);
}


void CFaceView::OnModeDire()
{
	// TODO: 在此添加命令处理程序代码
	if(m_nRunMode != MODE_DIRE)
	{
		m_nRunMode = MODE_DIRE;
	}
}


void CFaceView::OnUpdateModeDire(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_nRunMode == MODE_DIRE);
}


void CFaceView::OnModeVideo()
{
	// TODO: 在此添加命令处理程序代码
	if(m_nRunMode != MODE_VIDEO)
	{
		m_nRunMode = MODE_VIDEO;
	}
}


void CFaceView::OnUpdateModeVideo(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_nRunMode == MODE_VIDEO);
}


void CFaceView::OnUpdateFileAdd(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(m_nRunMode == MODE_IMAGE)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void CFaceView::OnUpdateFileClean(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(m_nRunMode == MODE_IMAGE)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void CFaceView::OnUpdateFileDelete(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(m_nRunMode == MODE_IMAGE)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void CFaceView::OnSet()
{
	// TODO: 在此添加命令处理程序代码
	CSetDlg dlg;
	dlg.DoModal();
}


void CFaceView::OnTrain()
{
	// TODO: 在此添加命令处理程序代码
	CTrainDlg dlg;
	dlg.DoModal();
}


void CFaceView::OnDatabase()
{
	// TODO: 在此添加命令处理程序代码
	CDataDlg dlg;
	dlg.DoModal();
}
