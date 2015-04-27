// CatchVideo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Face.h"
#include "CatchVideoDlg.h"
#include "afxdialogex.h"

//����ͷ������ر���
CvvImage m_CvvImage;
CvCapture * m_Video;//��Ƶ�ṹ��
IplImage* m_Frame;//���ڴ洢һ֡
IplImage* m_GrabFrame;//���ڴ洢һ֡
CRect clientRect; //�ͻ���
CDC *pDC; //���ڻ�ͼ���豸������
HDC hDC;  //DC���
CWnd *pWnd;   //��ʾ��Ƶpicture ctrl�ؼ�
bool b_flagProcess = false; //��־����ͷ�Ƿ��

//����Face������ر���
IplImage* defFace;
CvvImage tempFace;
CWnd *pFaceWnd;
CDC *pFaceDC;
HDC hFaceDC;
CRect faceRect;
std::vector<IplImage*> m_vFaces;//���ڴ�����10��face


threadInfo1 Info1;
CCriticalSection g_clsCriticalSection1; //������ٽ���
CCriticalSection g_clsCriticalSection2; //������ٽ���

// CCatchVideo �Ի���

IMPLEMENT_DYNAMIC(CCatchVideoDlg, CDialogEx)

CCatchVideoDlg::CCatchVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCatchVideoDlg::IDD, pParent)
{
}

CCatchVideoDlg::~CCatchVideoDlg()
{
}

void CCatchVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CCatchVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ShowWindow(SW_MAXIMIZE);//�����ʾ�Ի���

	//��ʼ������ͷ��ʾ����
	pWnd = GetDlgItem(IDC_SHOW_VIDEO);
	pDC = pWnd->GetDC();
	hDC = pDC->GetSafeHdc();
	pWnd ->GetClientRect(&clientRect);

	//���ڸ��ݶԻ����С���ÿؼ���С
	CRect rect;
	GetClientRect(&rect);
	m_pOld.x = rect.right - rect.left;
	m_pOld.y = rect.bottom - rect.top;

	run();//�򿪾Ϳ�ʼ��������ͷ

	for(int i = 0;i < 10;i++)
	{
		IplImage* temp = cvLoadImage("res/face.bmp",CV_LOAD_IMAGE_COLOR);
		m_vFaces.push_back(temp);
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCatchVideoDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCatchVideo ��Ϣ�������


void CCatchVideoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	if(nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		resize();
	}
	
}

void CCatchVideoDlg::resize()
{
	float fsp[2];
	POINT pNew; //��ȡ���ڶԻ���Ĵ�С
	CRect recta;
	GetClientRect(&recta); //��ÿͻ�����С

	pNew.x = recta.right - recta.left;
	pNew.y = recta.bottom - recta.top;

	//������ű���
	fsp[0] = (float)pNew.x/m_pOld.x;
	fsp[1] = (float)pNew.y/m_pOld.y;
	CRect rect;
	int iCtrlId;
	CPoint OldTLPoint,TLPoint;//���½�
	CPoint OldBRPoint,BRPoint;//���½�

	HWND hWndChild = ::GetWindow(m_hWnd,GW_CHILD);//�г����пؼ�
	while(hWndChild)
	{
		iCtrlId = ::GetDlgCtrlID(hWndChild); //��ÿؼ�ID
		GetDlgItem(iCtrlId)->GetWindowRect(rect);//��õ�ǰ�ؼ��Ĵ�С
		ScreenToClient(rect);
		OldTLPoint = rect.TopLeft();
		OldBRPoint = rect.BottomRight();

		//������ź������
		TLPoint.x = (long)(OldTLPoint.x*fsp[0]);
		TLPoint.y = (long)(OldTLPoint.y*fsp[1]);
		BRPoint.x = (long)(OldBRPoint.x*fsp[0]);
		BRPoint.y = (long)(OldBRPoint.y*fsp[1]);

		rect.SetRect(TLPoint,BRPoint);
		GetDlgItem(iCtrlId)->MoveWindow(rect,TRUE);
		hWndChild=::GetWindow(hWndChild, GW_HWNDNEXT); 
	}
	m_pOld = pNew;
}


void CCatchVideoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch(nIDEvent)
	{
	case 1://��ʾ����ͷ��ʱ��
		//��ʾ����ͷ
		m_Frame = cvQueryFrame(m_Video);  //���һ֡��ͼ��
		m_CvvImage.CopyOf(m_Frame,m_Frame->nChannels);     //�����֡ͼ��
		m_CvvImage.DrawToHDC(hDC,&clientRect);//����ͼ��video��ʾ����
		b_flagProcess = 1;

		break;

	case 2://��ȡͼ��ʱ��
		UpdateFaces();

		IplImage* tempSrc = getFrame();

		if(tempSrc)
		{
			Info1.src = cvCreateImage(cvGetSize(tempSrc),tempSrc->depth,tempSrc->nChannels);
			cvCopy(tempSrc,Info1.src,NULL);
			CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFunc1,&Info1,0,NULL);
		}
		break;

	}
	

	CDialogEx::OnTimer(nIDEvent);
}

IplImage* CCatchVideoDlg::getFrame()
{
	if (!m_Video)
	{
		AfxMessageBox("�ȴ�����ͷ");
		return NULL;
	}
	
	m_GrabFrame=cvQueryFrame(m_Video);
	if (!m_GrabFrame)
		return NULL;
	
	static	char countsnap='1';
	CString   m_name="";
	m_name += countsnap;
	m_name+=".jpg";	
	
	IplImage * m_snap=cvCreateImage(cvGetSize(m_GrabFrame),m_GrabFrame->depth,m_GrabFrame->nChannels);
	cvCopy(m_GrabFrame,m_snap,NULL);
	m_snap->origin=0;
	const char* pszStr = m_name.GetBuffer(m_name.GetLength());
	//cvSaveImage(pszStr,m_snap);	//��ͼ��д���ļ�
	
	countsnap++;
	return m_snap;
}
void CCatchVideoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�ж��Ƿ�������Ƶ��ʾ������
	CRect rect;
	GetDlgItem(IDC_SHOW_VIDEO)->GetWindowRect(&rect);
	if(rect.PtInRect(point))
	{
		//������Ϊ��ͣ
		if(b_flagProcess)
		{
			pause();
			
		}
		else
		{
			run();
			
		}
		//��ͣ��Ϊ����
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCatchVideoDlg::run()
{
	//�������е�����ͷ
	if(!m_Video)
		m_Video = cvCreateCameraCapture(-1);

	if(!m_Video)
	{
		AfxMessageBox("�޷�������ͷ");
		return;
	}
	else
	{
		b_flagProcess = true;
	}
	AfxMessageBox("��ʼ����");
	SetTimer(1,100,NULL); //������ʾ��Ƶ�ļ�ʱ��
	SetTimer(2,1200,NULL); //���ڻ�ȡͼƬ�����������ļ�ʱ��1.2��һ��
}

void CCatchVideoDlg::pause()
{
	//�رռ�ʱ��
	KillTimer(1);
	KillTimer(2);
	b_flagProcess = false;
	AfxMessageBox("����ͣ");
}


void CCatchVideoDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	cvReleaseCapture(&m_Video);
	CDialogEx::OnClose();
}


//ˢ��Faces����
void CCatchVideoDlg::UpdateFaces()
{
	std::vector<IplImage*>::iterator iter;
	int id = IDC_FACE_1;
	int size = m_vFaces.size();
	for(iter = m_vFaces.begin(); iter!=m_vFaces.end(); iter++)
	{
		
		pFaceWnd = GetDlgItem(id++);
		pFaceDC = pFaceWnd->GetDC();
		hFaceDC = pFaceDC->GetSafeHdc();
		pFaceWnd->GetClientRect(&faceRect);

		tempFace.CopyOf(*iter,(*iter)->nChannels);    //Ҫ���õ�����
		tempFace.DrawToHDC(hFaceDC,&faceRect);//����ͼ��face��ʾ����

	}


	
}

void ThreadFunc1(LPVOID lpParam)
{
	threadInfo1* pInfo  = (threadInfo1*)lpParam;
	CvHaarClassifierCascade* cascade;
	CvMemStorage* storage;
	
	//�������Ҫ�õ��ı���
	double scale = 1.3;
	IplImage* gray;
	IplImage* small_img;

	g_clsCriticalSection1.Lock();
	IplImage* src = pInfo->src;
	CString temp_cascade_name = g_cascade_name;
	g_clsCriticalSection1.Unlock();

	//������ʧ��
	if(!src)
		return;

	//���ط�����
	cascade = (CvHaarClassifierCascade*)cvLoadHaarClassifierCascade(temp_cascade_name,cvSize(20,20));
	
	/*IplImage *face=cvCreateImage(cvGetSize(Info1.src),Info1.src->depth,Info1.src->nChannels);
	cvCopy(Info1.src,face,NULL);
	m_vFaces.erase(m_vFaces.begin());
	m_vFaces.push_back(face);*/

	if(cascade)
	{
		//����洢�ռ�
		storage = cvCreateMemStorage(0);

		if(src)
		{
			//��������ͼƬ�����洢�����ͼƬ
			gray = cvCreateImage(cvSize(src->width,src->height), 8, 1);
			small_img = cvCreateImage(cvSize( cvRound (src->width/scale),
                        cvRound (src->height/scale)),8,1);

			//��img��GRBת��λ�Ҷ�ͼ����gray  
			cvCvtColor( src, gray, CV_BGR2GRAY );

			//���Ҷ�ͼgray��С��small_img
			cvResize( gray, small_img, CV_INTER_LINEAR );

			//����ֱ��ͼ���⻯�����Ƚϵ���ͼ��任Ϊ�Ƚ����ͼ�����Ӿֲ��Աȶ�
			cvEqualizeHist( small_img, small_img );

			cvClearMemStorage(storage);

			//������� ������������Ҫ�ǵøĲ���ֵ��
			//����߼���ʺ���ȷ��,������û���Լ�����cvRunHaarClassifierCascade
			CvSeq* faces = cvHaarDetectObjects(small_img,cascade,storage,
					g_scale_factor, g_min_neighbors, g_flags/*CV_HAAR_DO_CANNY_PRUNING*/,cvSize(g_min_size_w, g_min_size_h));

			int i;
			g_clsCriticalSection2.Lock();
			for(i = 0;i<(faces ? faces->total:0);i++)
			{
				CvRect * r = (CvRect*)cvGetSeqElem(faces,i);
				CvRect rect = cvRect(cvRound(r->x*scale),cvRound(r->y*scale),cvRound(r->width*scale),cvRound(r->height*scale));
				cvSetImageROI(src,rect);
				IplImage *face=cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
				cvCopy(src,face,NULL);
				m_vFaces.erase(m_vFaces.begin());
				m_vFaces.push_back(face);
			}
			g_clsCriticalSection2.Unlock();

			cvResetImageROI(src);
			cvReleaseImage( &gray );
			cvReleaseImage( &small_img );
		}else
			return;
		//�ͷ���Դ
		cvReleaseImage(&src);
		cvReleaseMemStorage(&storage);
		cvReleaseHaarClassifierCascade(&cascade);
	}else
	{//�޷����ط�����
		return;
	}
}