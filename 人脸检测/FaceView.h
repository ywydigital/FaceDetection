
// FaceView.h : CFaceView ��Ľӿ�

#pragma once


class CFaceView : public CListView
{
	protected: // �������л�����
	CFaceView();
	DECLARE_DYNCREATE(CFaceView)

// ����
public:
	CFaceDoc* GetDocument() const;
	BOOL m_bRunning;					//��־��ͼ�Ƿ�������
	BOOL m_bTerminate;					//��־��������ͼ�Ƿ���ֹ
	HANDLE m_hThreadLoad;			    //�����߳̾��
	CImageList    m_ImageListThumb;     //ͼƬ�б�
	int m_nRunMode;						//����ģʽ

// ����
public:
	BOOL IsRunning(){return m_bRunning;}
	BOOL TerminateLoadThread();
	void LoadThumbnailImages(int iAddType);   //��������ͼ�б�
	void DeleteImage(int nIndex);             //ɾ��ָ��ͼƬ
	void SetRunMode(int nType);               //��������ģʽ
// ��д
public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CFaceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DWORD m_dwThreadID;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewRefresh();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);

	//�����������Ϣ����
	afx_msg void OnFaceCatch();
	afx_msg void OnUpdateFileAdd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileClean(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileDelete(CCmdUI *pCmdUI);
	//��ON_COMMAND��Ӧ��������Doc����

	//������ģʽ���˵������Ϣ����
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

#ifndef _DEBUG  // FaceView.cpp �еĵ��԰汾
inline CFaceDoc* CFaceView::GetDocument() const
   { return reinterpret_cast<CFaceDoc*>(m_pDocument); }
#endif

