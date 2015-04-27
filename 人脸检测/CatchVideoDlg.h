#pragma once
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"

// CCatchVideo �Ի���
//���ݸ��̵߳���Ϣ��ע�������������صĶ��ں���˸�1
struct threadInfo1
{
	IplImage* src;
};


void ThreadFunc1(LPVOID lpParam);

class CCatchVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCatchVideoDlg)

public:
	CCatchVideoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCatchVideoDlg();
	void resize();

	void run();//������ͷ
	void pause();//��ͣ����ͷ

	IplImage* getFrame();//���ĳһ֡��ͼ��
	void UpdateFaces();

// �Ի�������
	enum { IDD = IDD_CATCH_VIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
