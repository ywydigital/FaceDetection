#include "stdafx.h"
#include "FaceCatch.h"

#include <vector>

CFaceCatch::CFaceCatch(std::vector<CString> vFilesName,CString strDesPath)
{
	m_vFilesName = vFilesName;
	m_strDesPath = strDesPath;

}

//全局变量
std::vector<CString> g_vFilesName;   //要处理的文件集合
std::vector<CString>::iterator iter;
volatile BOOL g_bRun; //标志线程是否继续运行
CCriticalSection g_clsCriticalSection; //定义个临界区
CString g_strDesPath;

//用于测试的TEMP
CFile file("worklog.txt",CFile::modeCreate | CFile::modeWrite);
CArchive ar(&file,CArchive::store);


//人脸检测线程，实现人脸获取和输出
void ThreadFunc(int num)
{
	CString temp;//暂存*iter的内容
	std::string tempName;
	IplImage* src;
	CvHaarClassifierCascade* cascade;
	CvMemStorage* storage;

	//人脸检测要用到的变量
	double scale = 1.3;
	IplImage* gray;
	IplImage* small_img;
	CString tmpPath;
	CString tmpCascadeName;
	

	while(g_bRun)
	{
		g_clsCriticalSection.Lock();

		if(iter != g_vFilesName.end())
		{
			temp = *iter;
			iter++;
			tmpPath.Format("%s\\faces",g_strDesPath);
			tmpCascadeName = g_cascade_name;
		}
		else
		{
			g_bRun = FALSE;
			g_clsCriticalSection.Unlock();
			break;
		}
		g_clsCriticalSection.Unlock();
		
		//对fileName进行人脸检测预处理
		//将CString类型转换为const char*类型
		tempName = (LPCSTR)CStringA(temp);
		const char* tmp = tempName.c_str();

		//获得文件名
		CString filename = (temp).Mid((temp).ReverseFind('\\')+1,
			(temp).GetLength()-(temp).ReverseFind('\\')-5);

		//打开文件
		if((src = cvLoadImage(tmp,CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR))== 0)
			//打开文件失败
			return;

		//加载分类器
		cascade = (CvHaarClassifierCascade*)cvLoadHaarClassifierCascade(tmpCascadeName,cvSize(20,20));

		if(cascade)
		{
			//分配存储空间
			storage = cvCreateMemStorage(0);

			cvNamedWindow("人脸检测",CV_WINDOW_AUTOSIZE);

			//如果图片存在则分析并显示结果，否则退出程序

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

				//创建输出文件夹
				if(!PathIsDirectory(tmpPath))
				{
					if(!CreateDirectory(tmpPath,NULL))
						//创建输出文件夹失败
						return;
				}
				//保存获取的人脸
			    CDatabase db;
	            db.Open(NULL, FALSE, FALSE, _T("ODBC;DSN=face;UID=root;PWD=root"));
				temp.Replace("\\","\\\\");
				for(int i = 0;i<(faces ? faces->total : 0);i++)
				{
					CvRect * r = (CvRect*)cvGetSeqElem(faces,i);
					CvRect rect = cvRect(cvRound(r->x*scale),cvRound(r->y*scale),cvRound(r->width*scale),cvRound(r->height*scale));
					cvSetImageROI(src,rect);
					CString save_path;
					save_path.Format("%s\\%s_face%d.jpg",tmpPath,filename,i);
					cvSaveImage(save_path,src);
					CString save_name;
					save_name.Format("%s_face%d.jpg",filename,i);
					CString sql;
					
					save_path.Replace("\\","\\\\");
					sql.Format("insert into face(SourceImg,SourcePath,FaceName,FacePath,Time)  values('%s.jpg','%s','%s','%s',NOW())", 
						filename,temp, save_name,save_path);
					db.ExecuteSQL(sql);
				}
				db.Close();
				cvResetImageROI(src);
				cvReleaseImage( &gray );
				cvReleaseImage( &small_img );
				//测试输出
				CString t;
				t.Format("thread %d handled img %s \n",num,filename);
				ar<<t;
			}
			else
				//加载图片失败
				return;
			//释放资源
			cvReleaseImage(&src);
			cvReleaseMemStorage(&storage);
			cvReleaseHaarClassifierCascade(&cascade);
		} 
		else
		{
			//无法加载分类器
			return;
		}
	}

}
bool CFaceCatch::FaceDetection()
{
	g_vFilesName = m_vFilesName;
	iter = g_vFilesName.begin();
	g_bRun = TRUE;
	g_strDesPath = m_strDesPath;

	int start = clock();

	for(int i = 0;i<5;i++)
	{
		hThread[i] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFunc,
			(VOID*)i,0,&threadID[i]);
	}

	WaitForMultipleObjects(5,hThread,TRUE,INFINITE); //等待所有线程执行完毕

	int time = clock() - start;
	CString s;
	s.Format("run time is %d\n",time);
	ar<<s;
	return true;
}