#pragma once

#include "cv.h"
#include "highgui.h"

//�̴߳�������ʵ�������������
void ThreadFunc(int num);
/*
  ���ã�ʵ��������⣬�������ȡ������
*/

class CFaceCatch
{
public:
	//��ȡ��������
	CFaceCatch(std::vector<CString> vFilesName,CString strDesPath);
	bool FaceDetection();
	//�Ե���ͼ��Ⲣ���face

protected:
	HANDLE hThread[5]; //�߳̾��
	DWORD threadID[5]; //�߳�ID
	
private:
	//Ҫ������ļ�������
	std::vector<CString> m_vFilesName;
	//��������·��
	CString m_strDesPath;

	//IplImage* src;
	//CvHaarClassifierCascade* cascade;
	//CvMemStorage* storage;
	//bool detect_and_catch(IplImage *img,CString filename);
	


};
