// CatchVideo.cpp : 实现文件
//

#include "stdafx.h"
#include "Face.h"
#include "CatchVideoDlg.h"
#include "afxdialogex.h"

//摄像头操作相关变量
CvvImage m_CvvImage;
CvCapture * m_Video;//视频结构体
IplImage* m_Frame;//用于存储一帧
IplImage* m_GrabFrame;//用于存储一帧
CRect clientRect; //客户区
CDC *pDC; //用于画图的设备描述表
HDC hDC;  //DC句柄
CWnd *pWnd;   //显示视频picture ctrl控件
bool b_flagProcess = false; //标志摄像头是否打开

//人脸Face区域相关变量
IplImage* defFace;
CvvImage tempFace;
CWnd *pFaceWnd;
CDC *pFaceDC;
HDC hFaceDC;
CRect faceRect;
std::vector<IplImage*> m_vFaces;//用于存放最近10张face


threadInfo1 Info1;
CCriticalSection g_clsCriticalSection1; //定义个临界区
CCriticalSection g_clsCriticalSection2; //定义个临界区

// CCatchVideo 对话框

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

	ShowWindow(SW_MAXIMIZE);//最大化显示对话框

	//初始化摄像头显示部分
	pWnd = GetDlgItem(IDC_SHOW_VIDEO);
	pDC = pWnd->GetDC();
	hDC = pDC->GetSafeHdc();
	pWnd ->GetClientRect(&clientRect);

	//用于根据对话框大小设置控件大小
	CRect rect;
	GetClientRect(&rect);
	m_pOld.x = rect.right - rect.left;
	m_pOld.y = rect.bottom - rect.top;

	run();//打开就开始运行摄像头

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


// CCatchVideo 消息处理程序


void CCatchVideoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if(nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		resize();
	}
	
}

void CCatchVideoDlg::resize()
{
	float fsp[2];
	POINT pNew; //获取现在对话框的大小
	CRect recta;
	GetClientRect(&recta); //获得客户区大小

	pNew.x = recta.right - recta.left;
	pNew.y = recta.bottom - recta.top;

	//获得缩放比例
	fsp[0] = (float)pNew.x/m_pOld.x;
	fsp[1] = (float)pNew.y/m_pOld.y;
	CRect rect;
	int iCtrlId;
	CPoint OldTLPoint,TLPoint;//左下角
	CPoint OldBRPoint,BRPoint;//右下角

	HWND hWndChild = ::GetWindow(m_hWnd,GW_CHILD);//列出所有控件
	while(hWndChild)
	{
		iCtrlId = ::GetDlgCtrlID(hWndChild); //获得控件ID
		GetDlgItem(iCtrlId)->GetWindowRect(rect);//获得当前控件的大小
		ScreenToClient(rect);
		OldTLPoint = rect.TopLeft();
		OldBRPoint = rect.BottomRight();

		//获得缩放后的坐标
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch(nIDEvent)
	{
	case 1://显示摄像头计时器
		//显示摄像头
		m_Frame = cvQueryFrame(m_Video);  //获得一帧的图像
		m_CvvImage.CopyOf(m_Frame,m_Frame->nChannels);     //保存该帧图像
		m_CvvImage.DrawToHDC(hDC,&clientRect);//绘制图像到video显示区域
		b_flagProcess = 1;

		break;

	case 2://获取图像定时器
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
		AfxMessageBox("先打开摄像头");
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
	//cvSaveImage(pszStr,m_snap);	//把图像写入文件
	
	countsnap++;
	return m_snap;
}
void CCatchVideoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//判断是否落在视频显示区域内
	CRect rect;
	GetDlgItem(IDC_SHOW_VIDEO)->GetWindowRect(&rect);
	if(rect.PtInRect(point))
	{
		//运行设为暂停
		if(b_flagProcess)
		{
			pause();
			
		}
		else
		{
			run();
			
		}
		//暂停设为运行
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCatchVideoDlg::run()
{
	//打开任意有的摄像头
	if(!m_Video)
		m_Video = cvCreateCameraCapture(-1);

	if(!m_Video)
	{
		AfxMessageBox("无法打开摄像头");
		return;
	}
	else
	{
		b_flagProcess = true;
	}
	AfxMessageBox("开始运行");
	SetTimer(1,100,NULL); //用于显示视频的计时器
	SetTimer(2,1200,NULL); //用于获取图片进行人脸检测的计时器1.2秒一次
}

void CCatchVideoDlg::pause()
{
	//关闭计时器
	KillTimer(1);
	KillTimer(2);
	b_flagProcess = false;
	AfxMessageBox("已暂停");
}


void CCatchVideoDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	cvReleaseCapture(&m_Video);
	CDialogEx::OnClose();
}


//刷新Faces区域
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

		tempFace.CopyOf(*iter,(*iter)->nChannels);    //要设置的人脸
		tempFace.DrawToHDC(hFaceDC,&faceRect);//绘制图像到face显示区域

	}


	
}

void ThreadFunc1(LPVOID lpParam)
{
	threadInfo1* pInfo  = (threadInfo1*)lpParam;
	CvHaarClassifierCascade* cascade;
	CvMemStorage* storage;
	
	//人脸检测要用到的变量
	double scale = 1.3;
	IplImage* gray;
	IplImage* small_img;

	g_clsCriticalSection1.Lock();
	IplImage* src = pInfo->src;
	CString temp_cascade_name = g_cascade_name;
	g_clsCriticalSection1.Unlock();

	//传参数失败
	if(!src)
		return;

	//加载分类器
	cascade = (CvHaarClassifierCascade*)cvLoadHaarClassifierCascade(temp_cascade_name,cvSize(20,20));
	
	/*IplImage *face=cvCreateImage(cvGetSize(Info1.src),Info1.src->depth,Info1.src->nChannels);
	cvCopy(Info1.src,face,NULL);
	m_vFaces.erase(m_vFaces.begin());
	m_vFaces.push_back(face);*/

	if(cascade)
	{
		//分配存储空间
		storage = cvCreateMemStorage(0);

		if(src)
		{
			//创建两个图片用来存储处理的图片
			gray = cvCreateImage(cvSize(src->width,src->height), 8, 1);
			small_img = cvCreateImage(cvSize( cvRound (src->width/scale),
                        cvRound (src->height/scale)),8,1);

			//将img由GRB转换位灰度图赋给gray  
			cvCvtColor( src, gray, CV_BGR2GRAY );

			//将灰度图gray缩小后small_img
			cvResize( gray, small_img, CV_INTER_LINEAR );

			//进行直方图均衡化，将比较淡的图像变换为比较深的图像，增加局部对比度
			cvEqualizeHist( small_img, small_img );

			cvClearMemStorage(storage);

			//人脸检测 ！！！！后面要记得改参数值，
			//来提高检测率和正确性,到底有没有自己调用cvRunHaarClassifierCascade
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
		//释放资源
		cvReleaseImage(&src);
		cvReleaseMemStorage(&storage);
		cvReleaseHaarClassifierCascade(&cascade);
	}else
	{//无法加载分类器
		return;
	}
}