
// FaceDoc.h : CFaceDoc 类的接口
//


#pragma once
#include "afxcoll.h"
#include <vector>

#include "myGdiPlus.h" 
using namespace Gdiplus; 



class CFaceDoc : public CDocument
{
protected: // 仅从序列化创建
	CFaceDoc();
	DECLARE_DYNCREATE(CFaceDoc)

// 特性
public:
	std::vector<CString> m_vFilesName;			// 选择的文件名

// 操作
public:
	void ShowSelectedImages(int iAddType);      //显示所选择的图片缩略图列表

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CFaceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

//消息映射函数
public:
	//“图片抓取模式”文件操作
	afx_msg void OnFileOpen();
	afx_msg void OnFileAdd();
	afx_msg void OnFileDelete();
	afx_msg void OnFileClean();
};
