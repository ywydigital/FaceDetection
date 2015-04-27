#pragma once


// CTrainDlg 对话框

class CTrainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTrainDlg)

public:
	CTrainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTrainDlg();

// 对话框数据
	enum { IDD = IDD_TRAIN_DIALOG };

private:
	//训练分类器相关数据
	CString m_positive;	//正样本路径
	CString m_negetive; //负样本路径
	CString m_info; //info.dat路径
	CString m_bg;   //bg.txt路径
	CString m_vec;  //a.vec路径
	CString m_data; //存放训练结果的路径
	CString m_xml;  //存放xml分类器的路径
	int m_iPositiveNum;  //正样本个数
	int m_iNegetiveNum;  //负样本个数
	int m_iImgWidth;    //正样本图片的宽度和高度
	int m_iImgHeight;
	int m_iStages;    //训练级数
	int m_iSplits;    //阶段分类器的弱分类器

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTrainHelp();
	afx_msg void OnBnClickedOpenClass();
	afx_msg void OnBnClickedInputPositive();
	afx_msg void OnBnClickedInputNegetive();
	afx_msg void OnBnClickedNormal();
	afx_msg void OnBnClickedCreateInfo();
	afx_msg void OnBnClickedTraining();
};
