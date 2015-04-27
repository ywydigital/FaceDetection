#pragma once

#include "cv.h"
#include "highgui.h"

//线程处理函数，实现人脸检测和输出
void ThreadFunc(int num);
/*
  作用：实现人脸检测，并输出截取的人脸
*/

class CFaceCatch
{
public:
	//获取人脸函数
	CFaceCatch(std::vector<CString> vFilesName,CString strDesPath);
	bool FaceDetection();
	//对单张图检测并获得face

protected:
	HANDLE hThread[5]; //线程句柄
	DWORD threadID[5]; //线程ID
	
private:
	//要处理的文件名集合
	std::vector<CString> m_vFilesName;
	//输出结果的路径
	CString m_strDesPath;

	//IplImage* src;
	//CvHaarClassifierCascade* cascade;
	//CvMemStorage* storage;
	//bool detect_and_catch(IplImage *img,CString filename);
	


};
