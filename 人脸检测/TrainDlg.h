#pragma once


// CTrainDlg �Ի���

class CTrainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTrainDlg)

public:
	CTrainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTrainDlg();

// �Ի�������
	enum { IDD = IDD_TRAIN_DIALOG };

private:
	//ѵ���������������
	CString m_positive;	//������·��
	CString m_negetive; //������·��
	CString m_info; //info.dat·��
	CString m_bg;   //bg.txt·��
	CString m_vec;  //a.vec·��
	CString m_data; //���ѵ�������·��
	CString m_xml;  //���xml��������·��
	int m_iPositiveNum;  //����������
	int m_iNegetiveNum;  //����������
	int m_iImgWidth;    //������ͼƬ�Ŀ�Ⱥ͸߶�
	int m_iImgHeight;
	int m_iStages;    //ѵ������
	int m_iSplits;    //�׶η���������������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
