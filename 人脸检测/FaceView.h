
// FaceView.h : CFaceView 类的接口

#pragma once


class CFaceView : public CListView
{
	protected: // 仅从序列化创建
	CFaceView();
	DECLARE_DYNCREATE(CFaceView)

// 特性
public:
	CFaceDoc* GetDocument() const;
	BOOL m_bRunning;					//标志视图是否在运行
	BOOL m_bTerminate;					//标志加载缩略图是否终止
	HANDLE m_hThreadLoad;			    //加载线程句柄
	CImageList    m_ImageListThumb;     //图片列表
	int m_nRunMode;						//运行模式

// 操作
public:
	BOOL IsRunning(){return m_bRunning;}
	BOOL TerminateLoadThread();
	void LoadThumbnailImages(int iAddType);   //加载缩略图列表
	void DeleteImage(int nIndex);             //删除指定图片
	void SetRunMode(int nType);               //设置运行模式
// 重写
public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CFaceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DWORD m_dwThreadID;

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewRefresh();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);

	//工具栏相关消息函数
	afx_msg void OnFaceCatch();
	afx_msg void OnUpdateFileAdd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileClean(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileDelete(CCmdUI *pCmdUI);
	//其ON_COMMAND响应函数放在Doc里了

	//“运行模式”菜单相关消息函数
	afx_msg void OnModeImage();
	afx_msg void OnModeDire();
	afx_msg void OnModeVideo();
	afx_msg void OnUpdateModeImage(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeDire(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeVideo(CCmdUI *pCmdUI);
	afx_msg void OnSet();
	afx_msg void OnTrain();
	afx_msg void OnDatabase();
};

#ifndef _DEBUG  // FaceView.cpp 中的调试版本
inline CFaceDoc* CFaceView::GetDocument() const
   { return reinterpret_cast<CFaceDoc*>(m_pDocument); }
#endif

