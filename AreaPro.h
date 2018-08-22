// AreaPro.h: interface for the CAreaPro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AREAPRO_H__88D90509_32B8_464E_8420_B91A8F4652F8__INCLUDED_)
#define AFX_AREAPRO_H__88D90509_32B8_464E_8420_B91A8F4652F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DibObject.h"

class CAreaPro : public CObject  
{
	DECLARE_DYNCREATE(CAreaPro)
public:
	CAreaPro();
	CAreaPro(CDibObject *pDibObject);
	virtual ~CAreaPro();

public:
	//ģ��ƥ��
	BOOL TemplateMatch(CDibObject *pTemplate = NULL, CDibObject *pDibObject = NULL);
	//��������㷨1
	BOOL Feeding1(CDibObject *pDibObject = NULL);

	//ˮƽͶӰ����
	BOOL ProjectH(CDibObject *pDibObject = NULL);

	//��ֱͶӰ����
	BOOL ProjectV(CDibObject *pDibObject = NULL);

	//��������㷨2
	BOOL Feeding2(CDibObject *pDibObject = NULL);

	//������ȡ
	BOOL ContourPickUp(CDibObject *pDibObject = NULL);

	//������ȡ
	BOOL Minusing(unsigned char *pTemplate = NULL, CDibObject *pDibObject = NULL);
	
	//��������
	BOOL EdgeTracing(CDibObject *pDibObject = NULL);

	//Laplacian�񻯲���
	BOOL LapSharping(float *fpArray = NULL, float fCoef = 1, int nTempW = 3, 
				   int nTempH = 3, int nTempXc = 1, int nTempYc = 1, 
				   int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1);
	BOOL GradSharping(int Threshold = 128, int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1,
		CDibObject *pDibObject = NULL);
	//ƽ������
	BOOL Smoothing(float *fpArray = NULL, float fCoef = 9, int nTempW = 3, 
				   int nTempH = 3, int nTempXc = 1, int nTempYc = 1, 
				   int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1);
	//�񻯲���
	BOOL Sharping(float *fpArray = NULL, int *nDirection = NULL, int nTempW = 3, 
				   int nTempH = 3, int nTempXc = 1, int nTempYc = 1, 
				   int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1);
	//��ֵ�˲�
	BOOL MedianFilter(int nType = 0, int nTempW = 3, int nTempH = 3, int nTempXc = 1, 
		              int nTempYc = 1, int nX1 = -1, int nY1 = -1, int nX2 = -1, int nY2 = -1);

	unsigned char *GetBitsPointer();
	unsigned char *GetBitsPointer(CDibObject *pDibObject);

	//����DibObject�����
	void SetDibObjectClass( CDibObject *pDibObject );
//��ϵ��Ժ���
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

protected:
	//��ģ�������ת��ÿִ��һ��ģ��˳ʱ����ת45�㣩
	void TemplateRotating(float *fpTemplate = NULL);

	//ģ���������������ģ�崦��ͼ����������
	BOOL TemplateOperation(float *fpArray, float fCoef, int nTempW, int nTempH, int nTempXc, int nTempYc, 
		                   unsigned char *pData, int nWidthBytes, 
						   int nX1, int nY1, int nX2, int nY2);
	//��ȡֱ��ͼ
	int *GetHistogram( int = -1, int = -1, int = -1, int = -1 );
	//����ֱ��ͼ
	int *CreateHistogram( int, int, int, int, unsigned char *, RGBQUAD *, int );

	//ͼ��������������
	void AddBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);
	//ͼ��������������
	void MinusBuffer(unsigned char *pOut, unsigned char *pIn, LONG lSize);

private:
	unsigned char GetMedian(unsigned char *pData = NULL, int nSize = 9, int nType = 1);
	//�������
	void Convolution(float *fpData, float *fpKernel, float fCoef, int nSize, unsigned char *nResult);
	CDibObject *m_pDibObject;

typedef struct{
	int Height;
	int Width;
}	Seed;

};

#endif // !defined(AFX_AREAPRO_H__88D90509_32B8_464E_8420_B91A8F4652F8__INCLUDED_)
