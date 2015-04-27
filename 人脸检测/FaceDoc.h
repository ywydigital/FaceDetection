
// FaceDoc.h : CFaceDoc ��Ľӿ�
//


#pragma once
#include "afxcoll.h"
#include <vector>

#include "myGdiPlus.h" 
using namespace Gdiplus; 



class CFaceDoc : public CDocument
{
protected: // �������л�����
	CFaceDoc();
	DECLARE_DYNCREATE(CFaceDoc)

// ����
public:
	std::vector<CString> m_vFilesName;			// ѡ����ļ���

// ����
public:
	void ShowSelectedImages(int iAddType);      //��ʾ��ѡ���ͼƬ����ͼ�б�

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CFaceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

//��Ϣӳ�亯��
public:
	//��ͼƬץȡģʽ���ļ�����
	afx_msg void OnFileOpen();
	afx_msg void OnFileAdd();
	afx_msg void OnFileDelete();
	afx_msg void OnFileClean();
};
