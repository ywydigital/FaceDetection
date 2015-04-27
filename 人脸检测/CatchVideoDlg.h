#pragma once
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"

// CCatchVideo 对话框
//传递给线程的信息，注：与人脸检测相关的都在后加了个1
struct threadInfo1
{
	IplImage* src;
};


void ThreadFunc1(LPVOID lpParam);

class CCatchVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCatchVideoDlg)

public:
	CCatchVideoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCatchVideoDlg();
	void resize();

	void run();//打开摄像头
	void pause();//暂停摄像头

	IplImage* getFrame();//获得某一帧的图像
	void UpdateFaces();

// 对话框数据
	enum { IDD = IDD_CATCH_VIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	POINT m_pOld;
	

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
};
