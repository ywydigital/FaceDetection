#include "stdafx.h"
#include "FaceCatch.h"

#include <vector>

CFaceCatch::CFaceCatch(std::vector<CString> vFilesName,CString strDesPath)
{
	m_vFilesName = vFilesName;
	m_strDesPath = strDesPath;

}

//ȫ�ֱ���
std::vector<CString> g_vFilesName;   //Ҫ������ļ�����
std::vector<CString>::iterator iter;
volatile BOOL g_bRun; //��־�߳��Ƿ��������
CCriticalSection g_clsCriticalSection; //������ٽ���
CString g_strDesPath;

//���ڲ��Ե�TEMP
CFile file("worklog.txt",CFile::modeCreate | CFile::modeWrite);
CArchive ar(&file,CArchive::store);


//��������̣߳�ʵ��������ȡ�����
void ThreadFunc(int num)
{
	CString temp;//�ݴ�*iter������
	std::string tempName;
	IplImage* src;
	CvHaarClassifierCascade* cascade;
	CvMemStorage* storage;

	//�������Ҫ�õ��ı���
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
		
		//��fileName�����������Ԥ����
		//��CString����ת��Ϊconst char*����
		tempName = (LPCSTR)CStringA(temp);
		const char* tmp = tempName.c_str();

		//����ļ���
		CString filename = (temp).Mid((temp).ReverseFind('\\')+1,
			(temp).GetLength()-(temp).ReverseFind('\\')-5);

		//���ļ�
		if((src = cvLoadImage(tmp,CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR))== 0)
			//���ļ�ʧ��
			return;

		//���ط�����
		cascade = (CvHaarClassifierCascade*)cvLoadHaarClassifierCascade(tmpCascadeName,cvSize(20,20));

		if(cascade)
		{
			//����洢�ռ�
			storage = cvCreateMemStorage(0);

			cvNamedWindow("�������",CV_WINDOW_AUTOSIZE);

			//���ͼƬ�������������ʾ����������˳�����

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

				//��������ļ���
				if(!PathIsDirectory(tmpPath))
				{
					if(!CreateDirectory(tmpPath,NULL))
						//��������ļ���ʧ��
						return;
				}
				//�����ȡ������
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
				//�������
				CString t;
				t.Format("thread %d handled img %s \n",num,filename);
				ar<<t;
			}
			else
				//����ͼƬʧ��
				return;
			//�ͷ���Դ
			cvReleaseImage(&src);
			cvReleaseMemStorage(&storage);
			cvReleaseHaarClassifierCascade(&cascade);
		} 
		else
		{
			//�޷����ط�����
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

	WaitForMultipleObjects(5,hThread,TRUE,INFINITE); //�ȴ������߳�ִ�����

	int time = clock() - start;
	CString s;
	s.Format("run time is %d\n",time);
	ar<<s;
	return true;
}