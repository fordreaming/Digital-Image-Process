// AreaPro.cpp: implementation of the CAreaPro class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AreaPro.h"

IMPLEMENT_DYNCREATE(CAreaPro, CObject)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CAreaPro::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
}
#endif

#ifdef _DEBUG
void CAreaPro::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//Check CDibObject members...
	ASSERT(m_pDibObject != NULL);	//Must exist
}
#endif

/***********************************************************************
*                                                                      *
*                               ��������                               *
*                                                                      *
***********************************************************************/

////////////////////////////////////////////////////////////////////////
//���캯��CAreaPro()    
//----------------------------------------------------------------------
//�������ܣ�����һ��CAreaPro��Ķ����粻����CDibObject���󡣵�һ�ε�   
//			��ĳһ��������ʱ�������һ��CDibObject����ָ�롣
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////

CAreaPro::CAreaPro()
{

}

////////////////////////////////////////////////////////////////////////
//���캯��CAreaPro()    
//----------------------------------------------------------------------
//�������ܣ�����һ��CAreaPro��Ķ��󲢴���CDibObject�������еĲ�����
//			��Ըö���ֱ����һ��������Ϊ����������ͼ��������
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CAreaPro::CAreaPro(CDibObject *pDibObject)
{
	m_pDibObject = pDibObject;
}

//��������
CAreaPro::~CAreaPro()
{

}

////////////////////////////////////////////////////////////////////////
//void SetDibObjectClass(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//�������ܣ�������ΪCAreaPro�����ָ��һ��CDibObject����ָ��
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ��ޡ�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CAreaPro::SetDibObjectClass( CDibObject *pDibObject )
{
	m_pDibObject = pDibObject;
}

////////////////////////////////////////////////////////////////////////
//int *GetHistogram()   
//----------------------------------------------------------------------
//�������ܣ���������ô����m_pDibObject������ͼ��ֱ��ͼ��������д˵�
//			��֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���ʱ����ָ
//			�����κ�δ���������ֵ��Ĭ�ϵ�-1����ֵ��������Ϊͼ������ֵ
//			�����ֲ������nX1��nY1������Ϊ0��nX2������Ϊͼ���ȼ�1��nY2
//			������Ϊͼ��߶ȼ�1����Ҫ������ͼ���Ͻ��в���ʱ����õķ���
//			�ǲ�����nX1��nY1��nX2��nY2ֵ���������ǻᱻĬ��Ϊ����ͼ��
//----------------------------------------------------------------------
//����˵����int	nX1, Ĭ��Ϊ-1
//			int	nY1, Ĭ��Ϊ-1
//			int	nX2, Ĭ��Ϊ-1
//			int	nY2, Ĭ��Ϊ-1
//----------------------------------------------------------------------
//��    �أ�ֱ��ͼ����ָ��*pBuffer���У�
//			nHistogramBuffer[]		�洢����ֱ��ͼ����
//			nHistogramBuffer[256]	�洢��ɫֱ��ͼ����
//			nHistogramBuffer[512]	�洢��ɫֱ��ͼ����
//			nHistogramBuffer[768]	�洢��ֱ��ͼ����
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CAreaPro����ʹ��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int *CAreaPro::GetHistogram( int nX1, int nY1, int nX2, int nY2)
{
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//���������
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( NULL );

	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	//����ֱ��ͼ����
	int *nHistogramBuffer = CreateHistogram(nX1, nY1, nX2, nY2, pBits,
		                                    pPalette, nWidthBytes);

	::GlobalUnlock(m_pDibObject->GetDib());

	return(nHistogramBuffer);

}

////////////////////////////////////////////////////////////////////////
//int *CreateHistogram()   
//----------------------------------------------------------------------
//�������ܣ���������������m_pDibObject������ͼ���ֱ��ͼ��������д˵�
//			��֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���ʱ����ָ
//			�����κ�δ���������ֵ��Ĭ�ϵ�-1����ֵ��������Ϊͼ������ֵ
//			�����ֲ������nX1��nY1������Ϊ0��nX2������Ϊͼ���ȼ�1��nY2
//			������Ϊͼ��߶ȼ�1����Ҫ������ͼ���Ͻ��в���ʱ����õķ���
//			�ǲ�����nX1��nY1��nX2��nY2ֵ���������ǻᱻĬ��Ϊ����ͼ��
//----------------------------------------------------------------------
//����˵����int	nX1					Ĭ��Ϊ-1
//			int	nY1					Ĭ��Ϊ-1
//			int	nX2					Ĭ��Ϊ-1
//			int	nY2					Ĭ��Ϊ-1
//			unsigned char *pData	ͼ��λͼ����ָ��
//			RGBQUAD *pPalette		ͼ���ɫ��ָ��
//			int nWidthBytes			ͼ���ֽڿ��
//----------------------------------------------------------------------
//��    �أ�ֱ��ͼ����ָ��*pBuffer���У�
//			pBuffer[]		�洢����ֱ��ͼ����
//			pBuffer[256]	�洢��ɫֱ��ͼ����
//			pBuffer[512]	�洢��ɫֱ��ͼ����
//			pBuffer[768]	�洢��ֱ��ͼ����
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CAreaPro����ʹ��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int *CAreaPro::CreateHistogram(int nX1, int nY1, int nX2, int nY2, 
							   unsigned char *pData, 
							   RGBQUAD *pPalette, 
							   int nWidthBytes)
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);

	//����ֱ��ͼ���ݻ����������飩
	int *pBuffer = new int [256 * 4];
	//����ֱ��ͼ���ݻ�����ʧ��
	if(pBuffer == NULL) return( NULL );

	//ֱ��ͼ���ݻ���������
	memset(pBuffer, 0, (256 * 4) * sizeof(int));

	//��������
	DWORD dwGray;
	int x, y;
	unsigned char *pTemp, ucRed, ucGreen, ucBlue;

	//ͼ��ĸ߶�
	int nHeight = m_pDibObject->GetHeight();

	switch(m_pDibObject->GetNumBits())
	{
	case 1:				//ÿ����λ��Ϊ1��������
		break;
	case 4:				//ÿ����λ��Ϊ4��������
		break;
	case 8:				//ÿ����λ��Ϊ8		
		for( y = nY1; y <= nY2; y++ )
		{
			//����ָ�붨λ��ͼ��������ʼλ��
			pTemp = pData;
			//����ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pTemp += ((nHeight - 1 - y) * nWidthBytes);
			//����ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pTemp += nX1;
			for(x = nX1; x <= nX2; x++)
			{
				//pTemp[x]Ϊ��ǰ����ֵ���Դ�Ϊ��ɫ���������ֵ��
				//ȡ����ɫ�������Ӧ����������ֵ��
				ucRed   = pPalette[pTemp[x]].rgbRed;
				ucGreen = pPalette[pTemp[x]].rgbGreen;
				ucBlue  = pPalette[pTemp[x]].rgbBlue;

				//����ϵL=0.3R+0.59G+0.11B���õ�����ֵ
				dwGray  = ((DWORD) ucRed * 30 +
					        (DWORD) ucGreen * 59 +
					        (DWORD) ucBlue * 11) / 100;
				dwGray &= 0x000000ff;

				//����ֱ��ͼ����
				pBuffer[dwGray]++;
				//��ɫֱ��ͼ����
				pBuffer[256 + ucRed]++;
				//��ɫֱ��ͼ����
				pBuffer[512 + ucGreen]++;
				//��ɫֱ��ͼ����
				pBuffer[768 + ucBlue]++;

			}
		}
		break;
	case 16:				//ÿ����λ��Ϊ16
		for(y = nY1; y <= nY2; y++)
		{
			//����ָ�붨λ��ͼ��������ʼλ��
			pTemp = pData;
			//����ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pTemp += (( nHeight - 1 - y) * nWidthBytes);
			//����ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pTemp += (nX1 * 2);
			for(x = nX1; x <= nX2; x++)
			{
				//����GETRGB555���ȡ��ԭɫ����
				GETRGB555(ucRed, ucGreen, ucBlue, pTemp);

				//����ϵL=0.3R+0.59G+0.11B���õ�����ֵ
				dwGray = ((DWORD) ucRed * 30 +
					       (DWORD) ucGreen * 59 +
					       (DWORD) ucBlue * 11) / 100;
				dwGray &= 0x000000ff;

				//����ֱ��ͼ����
				pBuffer[dwGray]++;
				//��ɫֱ��ͼ����
				pBuffer[256 + ucRed]++;
				//��ɫֱ��ͼ����
				pBuffer[512 + ucGreen]++;
				//��ɫֱ��ͼ����
				pBuffer[768 + ucBlue]++;

				//����ָ���2
				pTemp += 2;
			}
		}
		break;
	case 24:				//ÿ����λ��Ϊ24
		for(y = nY1; y < nY2; y++)
		{
			//����ָ�붨λ��ͼ��������ʼλ��
			pTemp = pData;
			//����ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pTemp += (( nHeight - 1 - y) * nWidthBytes);
			//����ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pTemp += (nX1 * 3);

			for(x=nX1; x<=nX2; x++)
			{
				//��ȡ������ɫ����ԭɫ��
				ucRed   = pTemp[x * 3 + 2];
				ucGreen = pTemp[x * 3 + 1];
				ucBlue  = pTemp[x * 3];

				//����ϵL=0.3R+0.59G+0.11B���õ�����ֵ
				dwGray  = ((DWORD) ucRed * 30 +
					        (DWORD) ucGreen * 59 +
					        (DWORD) ucBlue * 11) / 100;
				dwGray &= 0x000000ff;

				//����ֱ��ͼ����
				pBuffer[dwGray]++;
				//��ɫֱ��ͼ����
				pBuffer[256 + ucRed]++;
				//��ɫֱ��ͼ����
				pBuffer[512 + ucGreen]++;
				//��ɫֱ��ͼ����
				pBuffer[768 + ucBlue]++;

				//����ָ���3
				pTemp += 3;
			}
		}
		break;
	case 32:				//ÿ����λ��Ϊ24
		for(y = nY1; y <= nY2; y++)
		{
			//����ָ�붨λ��ͼ��������ʼλ��
			pTemp = pData;
			//����ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pTemp += (( nHeight - 1 - y) * nWidthBytes);
			//����ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pTemp += (nX1 * 4);

			for(x = nX1; x <= nX2; x++)
			{
				//����GETRGB888���ȡ������ɫ����ԭɫ��
				GETRGB888(ucRed, ucGreen, ucBlue, pTemp);

				//����ϵL=0.3R+0.59G+0.11B���õ�����ֵ
				dwGray = ((DWORD) ucRed * 30 +
					       (DWORD) ucGreen * 59 +
					       (DWORD) ucBlue * 11) / 100;

				dwGray &= 0x000000ff;

				//����ֱ��ͼ����
				pBuffer[dwGray]++;
				//��ɫֱ��ͼ����
				pBuffer[256 + ucRed]++;
				//��ɫֱ��ͼ����
				pBuffer[512 + ucGreen]++;
				//��ɫֱ��ͼ����
				pBuffer[768 + ucBlue]++;

				//����ָ���4
				pTemp += 4;
			}
		}
		break;
	}
	
	return( pBuffer );
}

////////////////////////////////////////////////////////////////////////
//BOOL Convolution()   
//----------------------------------------------------------------------
//�������ܣ���������ָ��������������о�����㡣
//----------------------------------------------------------------------
//����˵����float *fpData		��ִ�о��������
//			float *fpKernel		���������
//			float fCoef			���ϵ��
//			int nSize			����ߴ�
//			unsigned char *nResult		������
//----------------------------------------------------------------------
//��    �أ��ޡ�
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ˽���ͣ�ֻ����CAreaPro����ʹ��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CAreaPro::Convolution(float *fpData, float *fpKernel, float fCoef, 
						   int nSize, unsigned char *nResult)
{

	int i;
	float Sum = 0, fResult;

	//������
	for(i = 0; i < nSize; i++)
	{
		Sum += fpData[i] * fpKernel[i];
	}

	//ִ�о����Ľ��
	fResult = Sum / fCoef;

	//�����ֵ
	fResult = (float)fabs(fResult);
	
	//�ж��Ƿ񳬹�255
	if(fResult > 255.0 )
	{
		// ֱ�Ӹ�ֵΪ255
		fResult = 255.0;
	}
	
	//�Ծ������������룬��ת����unsigned char������Ϊ��󷵻ؽ��
	*nResult = (unsigned char) (fResult + 0.5);			
}

////////////////////////////////////////////////////////////////////////
//void Smoothing()   
//----------------------------------------------------------------------
//�������ܣ��ú�����ָ����ģ�壨�����С������ָ��m_pDibObject�е�ͼ��
//			�������ƽ������������nTempHָ��ģ��ĸ߶ȣ�����nTempWָ��ģ
//			��Ŀ�ȣ�����nTempXc��nTempYcָ��ģ�������Ԫ�����꣬����
//			fpArrayΪָ��ģ��Ԫ�������ָ�룬fCoefָ��ϵ�����κ�δָ
//			���Ļ�Ĭ��Ϊ-1�����꽫����Ϊͼ��ı�Եֵ�����磺nX1��nY1�ᱻ
//			��Ϊ0��nX2��nY2�ᱻ��Ϊͼ��Ŀ�͸ߡ�������ͼ����е�ͨ�˲�
//			����÷����ǲ������κβ���������ʹ��ԭ�ȴ����CDibObject��
//			��ָ�롣
//----------------------------------------------------------------------
//����˵����float *fpArray	ָ��ģ�������ָ��
//			float fCoef		ģ��ϵ��
//			int   nTempW	ģ��Ŀ��
//			int   nTempH	ģ��ĸ߶�
//			int   nTempXc	ģ�������Ԫ��X���� ( <= nTempW - 1)
//			int   nTempYc	ģ�������Ԫ��Y���� ( <= nTempH - 1)
//			int	  nX1		Ĭ��Ϊ-1
//			int   nY1		Ĭ��Ϊ-1
//			int	  nX2		Ĭ��Ϊ-1
//			int	  nY2		Ĭ��Ϊ-1
//----------------------------------------------------------------------
//��    �أ��ޡ�
//----------------------------------------------------------------------
//ע    �⣺�˺����������������TemplateOperation()ģ�����������ͼ�����
//			ƽ��������Ĭ�ϵ�ģ����3��3��Boxƽ��ģ�塣
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::Smoothing(float *fpArray, float fCoef, int nTempW, 
						int nTempH, int nTempXc, int nTempYc,
						int nX1, int nY1, int nX2, int nY2)
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);

	//ֻ����8λͼ��
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("Ŀǰֻ֧��8λ�Ҷ�ͼ��Ĵ���");
		return( FALSE );
	}

	//���������
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//�Ա߽����ز�������
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 > nWidth - nTempW + nTempXc + 1) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 > nHeight - nTempH + nTempYc + 1) nY2 = nHeight - nTempH + nTempYc + 1;

	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	DWORD dwNewSize;

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits(),
											   &nNewWidthBytes, 8);
	if( pBuffer == NULL ) return( NULL );

	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	dwNewSize = nNewWidthBytes * nHeight;

	//�������ڴ洢ɫ��ֵ����ʱ����
	double *pHue = new double [dwNewSize];
	if(pHue == NULL) return(FALSE);
	memset(pHue, 0, dwNewSize * sizeof(double));

	//�������ڴ洢���Ͷ�ֵ����ʱ����
	double *pSaturation = new double [dwNewSize];
	if(pSaturation == NULL) return(FALSE);
	memset(pSaturation, 0, dwNewSize * sizeof(double));

	//�������ڴ洢����ֵ����ʱ����
	unsigned char *pLight = new unsigned char [dwNewSize];
	if(pLight == NULL) return(FALSE);
	memset(pLight, 0, dwNewSize * sizeof(unsigned char));

	float *fTempArray;

	//Ĭ��Ϊ3��3�ĸ�ͨ�˲���1ģ��
	static float fpDefaultArray[] = {1.0, 1.0, 1.0, 
							         1.0, 1.0, 1.0,
							         1.0, 1.0, 1.0};

	//û�д���ģ�壬��Ĭ��ģ��
	if( fpArray == NULL ) 
	{
		fTempArray = fpDefaultArray;
		fCoef = 9;
	}
	//���ô����ģ��
	else fTempArray = fpArray;

	//����Template��������
	if(!TemplateOperation(fTempArray, fCoef, nTempW, nTempH, nTempXc, nTempYc,
						pBits, nWidthBytes, nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	//�ڴ����
	::GlobalUnlock(m_pDibObject->GetDib());

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//void Sharping()   
//----------------------------------------------------------------------
//�������ܣ��ú�����ָ����ģ�壨�����С������ָ��m_pDibObject�е�ͼ��
//			��������񻯲���������nTempHָ��ģ��ĸ߶ȣ�����nTempWָ��ģ
//			��Ŀ�ȣ�����nTempXc��nTempYcָ��ģ�������Ԫ�����꣬����
//			fpArrayΪָ��ģ��Ԫ�������ָ�룬fCoefָ��ϵ�����κ�δָ
//			���Ļ�Ĭ��Ϊ-1�����꽫����Ϊͼ��ı�Եֵ�����磺nX1��nY1�ᱻ
//			��Ϊ0��nX2��nY2�ᱻ��Ϊͼ��Ŀ�͸ߡ�������ͼ����е�ͨ�˲�
//			����÷����ǲ������κβ���������ʹ��ԭ�ȴ����CDibObject��
//			��ָ�롣
//----------------------------------------------------------------------
//����˵����float *fpArray	ָ��ģ�������ָ��
//			int nTempNum	ģ�����
//			int   nTempW	ģ��Ŀ��
//			int   nTempH	ģ��ĸ߶�
//			int   nTempXc	ģ�������Ԫ��X���� ( <= nTempW - 1)
//			int   nTempYc	ģ�������Ԫ��Y���� ( <= nTempH - 1)
//			int	  nX1		Ĭ��Ϊ-1
//			int   nY1		Ĭ��Ϊ-1
//			int	  nX2		Ĭ��Ϊ-1
//			int	  nY2		Ĭ��Ϊ-1
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::Sharping(float *fpArray, int *nDirection, int nTempW, 
						int nTempH, int nTempXc, int nTempYc,
						int nX1, int nY1, int nX2, int nY2)
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);

	//ֻ����8λͼ��
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("Ŀǰֻ֧��8λ�Ҷ�ͼ��Ĵ���");
		return( FALSE );
	}

	//���������
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//�Ա߽����ز�������
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 > nWidth - nTempW + nTempXc + 1) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 > nHeight - nTempH + nTempYc + 1) nY2 = nHeight - nTempH + nTempYc + 1;

	//�������
	unsigned char *pBuffer, *pBits, *pNewBits1, *pNewBits2;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;
	DWORD dwNewSize;

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( NULL );

	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	HGLOBAL hNewDib1, hNewDib2;

	//��ͼ���ļ���С�����ֽ�Ϊ��λ��
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDib1 = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//�ڴ����ʧ��
	if( hNewDib1 == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits1 = (unsigned char *) ::GlobalLock( hNewDib1 );
	if( pNewBits1 == NULL )
	{
		::GlobalFree( hNewDib1 );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//����ͼ������
	memcpy(pNewBits1, pBits, nWidthBytes * nHeight);

	//Ϊ��ͼ������ڴ�
	hNewDib2 = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//�ڴ����ʧ��
	if( hNewDib2 == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits2 = (unsigned char *) ::GlobalLock( hNewDib2 );
	if( pNewBits2 == NULL )
	{
		::GlobalFree( hNewDib2 );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//����ͼ������
	memcpy(pNewBits2, pBits, nWidthBytes * nHeight);

	float *fTempArray;

	//Ĭ��Ϊ3��3��Kirsch�ݶ�ģ��
	static float fpDefaultArray[] = {5.0, 5.0, 5.0, 
							         -3.0, 0.0, -3.0,
							         -3.0, -3.0, -3.0};

	//û�д���ģ�壬��Ĭ��ģ��
	if( fpArray == NULL ) fTempArray = fpDefaultArray;
	//���ô����ģ��
	else fTempArray = fpArray;

	int *nTempDirection;

	//Ĭ��Ϊ8������
	static int nDefaultDirection[] = {1, 1, 1, 1, 1, 1, 1, 1};

	//û�д��뷽����Ĭ�Ϸ���
	if( nDirection == NULL ) nTempDirection = nDefaultDirection;
	//���ô���ķ���
	else nTempDirection = nDirection;
	
	//��ʼ����
	int nStartD = 0;
	
	int i = 0;
	//������ʼ����
	while(nTempDirection[i] == 0 && i < 8)
	{
		//������ת45��		
		i++;

		//����ʼ����ֵ
		nStartD = i;

		//��ģ�������ת
		TemplateRotating(fTempArray);

	}

	//ûָ����ʼ���򣬷���
	if(i == 8) return(FALSE);

	//����Template��������
	if(!TemplateOperation(fTempArray, 1.0, nTempW, nTempH, nTempXc, nTempYc,
		pNewBits1, nWidthBytes, 
		nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	
	//������ͼ�����ݲ����йصı���
	unsigned char *pNewTemp1, *pNewTemp2;
	int x, y;

	for(i = nStartD + 1; i < 8; i++)
	{
		
		//��ģ�������ת
		TemplateRotating(fTempArray);

		if(nTempDirection[i] == 1)
		{
			//����Template��������
			if(!TemplateOperation(fTempArray, 1.0, nTempW, nTempH, nTempXc, nTempYc,
				pNewBits2, nWidthBytes, 
				nX1, nY1, nX2, nY2))
			{
				return(FALSE);
			}
			
			switch(m_pDibObject->GetNumBits())
			{
			case 8:			//8λͼ��
				//��λ��
				for(y = nY1; y <= nY2; y++ )
				{
					//��ͼ������ָ�붨λ����ʼλ��
					pNewTemp1 = pNewBits1;
					//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
					pNewTemp1 += y * nWidthBytes;
					//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
					pNewTemp1 += nX1;
					
					//��ͼ������ָ�붨λ����ʼλ��
					pNewTemp2 = pNewBits2;
					//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
					pNewTemp2 += y * nWidthBytes;
					//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
					pNewTemp2 += nX1;
					
					//��λ��
					for(x = nX1; x <= nX2; x++)
					{
						if(pNewTemp2[x] > pNewTemp1[x])
							pNewTemp1[x] = pNewTemp2[x];
					}
				}
				break;
			}
			//����ͼ������
			memcpy(pNewBits2, pBits, nWidthBytes * nHeight);
		}
		
	}

	//����ͼ������
	memcpy(pBits, pNewBits1, nWidthBytes * nHeight);

	//�ڴ����
	::GlobalUnlock(m_pDibObject->GetDib());

	//�ͷŲ���ʹ�õ��ڴ�
	::GlobalUnlock( hNewDib1 );
	::GlobalFree( hNewDib1 );
	::GlobalUnlock( hNewDib2 );
	::GlobalFree( hNewDib2 );

	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL LapSharping()   
//----------------------------------------------------------------------
//�������ܣ��ú�����ָ����ģ�壨�����С������ָ��m_pDibObject�е�ͼ��
//			�������Laplacian�񻯲���������nTempHָ��ģ��ĸ߶ȣ�����
//			nTempWָ��ģ��Ŀ�ȣ�����nTempXc��nTempYcָ��ģ�������Ԫ��
//			���꣬����fpArrayΪָ��ģ��Ԫ�������ָ�룬fCoefָ��ϵ������
//			��δָ���Ļ�Ĭ��Ϊ-1�����꽫����Ϊͼ��ı�Եֵ�����磺nX1��
//			nY1�ᱻ��Ϊ0��nX2��nY2�ᱻ��Ϊͼ��Ŀ�͸ߡ�������ͼ����е�
//			ͨ�˲�����÷����ǲ������κβ���������ʹ��ԭ�ȴ����
//			CDibObject����ָ�롣
//----------------------------------------------------------------------
//����˵����float *fpArray	ָ��ģ�������ָ��
//			float fCoef		ģ��ϵ��
//			int   nTempW	ģ��Ŀ��
//			int   nTempH	ģ��ĸ߶�
//			int   nTempXc	ģ�������Ԫ��X���� ( <= nTempW - 1)
//			int   nTempYc	ģ�������Ԫ��Y���� ( <= nTempH - 1)
//			int	  nX1		Ĭ��Ϊ-1
//			int   nY1		Ĭ��Ϊ-1
//			int	  nX2		Ĭ��Ϊ-1
//			int	  nY2		Ĭ��Ϊ-1
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::LapSharping(float *fpArray, float fCoef, int nTempW, 
						int nTempH, int nTempXc, int nTempYc,
						int nX1, int nY1, int nX2, int nY2)
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);

	//ֻ����8λͼ��
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("Ŀǰֻ֧��8λ�Ҷ�ͼ��Ĵ���");
		return( FALSE );
	}

	//���������
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//�Ա߽����ز�������
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 > nWidth - nTempW + nTempXc + 1) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 > nHeight - nTempH + nTempYc + 1) nY2 = nHeight - nTempH + nTempYc + 1;

	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	DWORD dwNewSize;

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits(),
											   &nNewWidthBytes, 8);
	if( pBuffer == NULL ) return( NULL );

	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	dwNewSize = nNewWidthBytes * nHeight;

	//�������ڴ洢ɫ��ֵ����ʱ����
	double *pHue = new double [dwNewSize];
	if(pHue == NULL) return(FALSE);
	memset(pHue, 0, dwNewSize * sizeof(double));

	//�������ڴ洢���Ͷ�ֵ����ʱ����
	double *pSaturation = new double [dwNewSize];
	if(pSaturation == NULL) return(FALSE);
	memset(pSaturation, 0, dwNewSize * sizeof(double));

	//�������ڴ洢����ֵ����ʱ����
	unsigned char *pLight = new unsigned char [dwNewSize];
	if(pLight == NULL) return(FALSE);
	memset(pLight, 0, dwNewSize * sizeof(unsigned char));

	float *fTempArray;

	//Ĭ��Ϊ3��3�ĸ�ͨ�˲���1ģ��
	static float fpDefaultArray[] = {-1.0, -1.0, -1.0, 
							         -1.0,  9.0, -1.0,
							         -1.0, -1.0, -1.0};

	//û�д���ģ�壬��Ĭ��ģ��
	if( fpArray == NULL ) fTempArray = fpDefaultArray;
	//���ô����ģ��
	else fTempArray = fpArray;

	//����Template��������
	if(!TemplateOperation(fTempArray, fCoef, nTempW, nTempH, nTempXc, nTempYc,
						pBits, nWidthBytes, nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	//�ڴ����
	::GlobalUnlock(m_pDibObject->GetDib());

	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL MedianFilter()   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObjectͼ����������ֵ�˲����κ�δָ���Ļ�
//			Ĭ��Ϊ-1�����꽫����Ϊͼ��ı�Եֵ�����磺nX1��nY1�ᱻ��Ϊ0��
//			nX2��nY2�ᱻ��Ϊͼ��Ŀ�͸ߡ�������ͼ�������ֵ�˲�����÷�
//			���ǲ������κβ����������ָ��һ��CDibObject����ָ�룬������
//			ʹ��ԭ�ȴ����CDibObject����ָ�롣
//----------------------------------------------------------------------
//����˵����int	  nType		��ȡ��ֵ�ķ�ʽ, Ĭ��Ϊ1
//								0����ð������
//								1������������
//			int   nTempW	���ڵĿ��
//			int   nTempH	���ڵĸ߶�
//			int   nTempXc	���ڵ�����Ԫ��X����
//			int   nTempYc	���ڵ�����Ԫ��Y����
//			int	  nX1		Ĭ��Ϊ-1
//			int   nY1		Ĭ��Ϊ-1
//			int	  nX2		Ĭ��Ϊ-1
//			int	  nY2		Ĭ��Ϊ-1
//					
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::MedianFilter(int nType, int nTempW, int nTempH, int nTempXc, 
							int nTempYc, int nX1, int nY1, int nX2, int nY2)
{
   //ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//��1λ��4λͼ�����κβ���ֱ�ӷ���
	if(m_pDibObject->GetNumBits() != 8)
	{
		// ��ʾ�û��������ô���
		AfxMessageBox("ֻ֧��8λͼ��,���������룡");
		
		// ����
		return( FALSE );
	}
	
	//���������
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//�Ա߽����ز�������
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 >= nWidth - nTempW + nTempXc) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 >= nHeight - nTempH + nTempYc) nY2 = nHeight - nTempH + nTempYc + 1;
	
	//�������
	unsigned char Data;
	//������ͼ�����ݲ����йصı���
	unsigned char *pOldBuffer, *pNewBuffer, 
				  *pOldBits, *pNewBits, 
				  *pOldTemp, *pNewTemp,
				  *pNeighborTemp;

	BITMAPFILEHEADER *pOldBFH, *pNewBFH;

	BITMAPINFOHEADER *pOldBIH, *pNewBIH;

	RGBQUAD *pOldPalette, *pNewPalette;

	int nWidthBytes, nNumColors, x, y, i, j;
	
	//��ȡԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//ԭͼ���ļ�ͷ
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷ
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors * sizeof(RGBQUAD)];
	
	DWORD dwNewSize;
	HGLOBAL hNewDib;
	
	//��ͼ���ļ���С�����ֽ�Ϊ��λ��
	dwNewSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + 
				nNumColors * sizeof( RGBQUAD ) + nWidthBytes * nHeight;

	//Ϊ��ͼ������ڴ�
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );

	//�ڴ����ʧ��
	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}
	
	//��ͼ��ָ��
	pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBuffer == NULL )
	{
		::GlobalFree( hNewDib );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}
	
	//��ͼ���ļ�ͷ
	pNewBFH = (BITMAPFILEHEADER *) pNewBuffer;
	//��ͼ����Ϣͷ
	pNewBIH = (BITMAPINFOHEADER *) &pNewBuffer[sizeof(BITMAPFILEHEADER)];
	//��ͼ���ɫ��ָ��
	pNewPalette = (RGBQUAD *) &pNewBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//��ͼ������ָ��
	pNewBits = (unsigned char *) &pNewBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors*sizeof(RGBQUAD)];
	
	//����ԭͼ�����ݵ���ͼ��

	//�����ļ�ͷ
	memcpy(pNewBFH, pOldBFH, sizeof(BITMAPFILEHEADER));
	
	//������Ϣͷ
	memcpy(pNewBIH, pOldBIH, sizeof(BITMAPINFOHEADER));
	
	//���Ƶ�ɫ��
	for(i = 0; i < nNumColors; i++ ) pNewPalette[i] = pOldPalette[i];
	
	//����ͼ������
	memcpy( pNewBits, pOldBits, nWidthBytes * nHeight );

	//����ģ��ĳߴ�
	int nSize = nTempW * nTempH;
	
	//���������ֵ�õ���ʱ����
	unsigned char *pGray = new unsigned char [nTempW * nTempH];
	if(pGray == NULL) return( NULL );
	memset(pGray, 0, (nSize) * sizeof(unsigned char));

	unsigned char nData;

	switch(m_pDibObject->GetNumBits())
	{
	case 8:			//8λͼ��
		//��λ��
		for(y = nY1; y < nY2; y++ )
		{
			//ԭͼ������ָ�붨λ����ʼλ��
			pOldTemp = pOldBits;
			//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pOldTemp += (nHeight - 1 - y) * nWidthBytes;
			//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pOldTemp += nX1;
			
			//��ͼ������ָ�붨λ����ʼλ��
			pNewTemp = pNewBits;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pNewTemp += (nHeight - 1 - y) * nWidthBytes;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pNewTemp += nX1;
			
			//��λ��
			for(x = nX1; x < nX2; x++)
			{
				//ȡ��ԭͼ��nTempW��nTempH�����ڵ�����ֵ
				for (i = 0; i < nTempH; i++)
				{
					for (j = 0; j < nTempW; j++)
					{
						//����ָ��ָ��ǰ����
						pNeighborTemp = pOldTemp;
						//����ָ��ָ��ԭͼ���y - nTempYc + i�С�
						pNeighborTemp += nWidthBytes * (nTempYc - i);
						//��x - nTempXc + j�����ص�ָ��
						pNeighborTemp += (j - nTempXc);
						
						//��ȡ����ֵ
						Data = *pNeighborTemp;

						pGray[i*nTempW+j] = ( pOldPalette[Data].rgbRed * 30 +
										pOldPalette[Data].rgbGreen * 59 +
										pOldPalette[Data].rgbBlue * 11 ) / 100;
					}
				}
				
				nData = GetMedian(pGray, nSize, nType);
				//��������������ͼ��
				*pNewTemp = (unsigned char) m_pDibObject->GetNearestIndex( 
					pNewPalette[nData].rgbRed, pNewPalette[nData].rgbGreen, 
					pNewPalette[nData].rgbBlue,	pNewPalette, nNumColors );

				//�¾�ͼ������ָ���1
				pOldTemp++;
				pNewTemp++;
				
			}
		}
		break;
	}
	
	//�ͷ��ڴ�
	delete [] pGray;

	::GlobalUnlock( m_pDibObject->GetDib() );
    ::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
    m_pDibObject->SetDib( hNewDib );
    return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL TemplateOperation()   
//----------------------------------------------------------------------
//�������ܣ��ú�����ָ����ģ�壨�����С������ͼ�������������ݽ���ģ���
//			��������nTempHָ��ģ��ĸ߶ȣ�����nTempWָ��ģ��Ŀ�ȣ�����
//			nTempXc��nTempYcָ��ģ�������Ԫ�����꣬����fpArrayΪָ��ģ
//			��Ԫ�������ָ�룬fCoefָ��ģ��ϵ����
//----------------------------------------------------------------------
//����˵����float *fpArray			ָ��ģ�������ָ��
//			float fCoef				ģ��ϵ��
//			int   nTempW			ģ��Ŀ��
//			int   nTempH			ģ��ĸ߶�
//			int   nTempXc			ģ�������Ԫ��X���� ( <= nTempW - 1)
//			int   nTempYc			ģ�������Ԫ��Y���� ( <= nTempH - 1)
//			unsigned char *pData	ͼ������ָ��
//			int nWidthBytes			ͼ���ֽڿ��
//			int	  nX1				����������߽�		
//			int   nY1				���������ϱ߽�
//			int	  nX2				���������ұ߽�
//			int	  nY2				���������±߽�
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CAreaPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::TemplateOperation(float *fpArray, float fCoef, 
						int nTempW, int nTempH, int nTempXc, int nTempYc,
						unsigned char *pData, int nWidthBytes, 
						int nX1, int nY1, int nX2, int nY2)
{
	//��ȡͼ��߶ȣ�������Ϊ��λ��
	int nHeight = m_pDibObject->GetHeight();

	//�������
	unsigned char Data;

	//������ͼ�����ݲ����йصı���
	unsigned char *pOldBits, *pNewBits, 
				  *pOldTemp, *pNewTemp,
				  *pNeighborTemp;
	int nNumColors, x, y, i, j;
	DWORD dwNewSize;

	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();

	//ԭͼ������ָ��
	pOldBits = pData;

	HGLOBAL hNewDib;

	//��ͼ���ļ���С�����ֽ�Ϊ��λ��
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//�ڴ����ʧ��
	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDib );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//����ͼ������
	memcpy( pNewBits, pOldBits, nWidthBytes * nHeight );

	//�������������õ���ʱ����
	float *pGray = new float [nTempW * nTempH];
	if(pGray == NULL) return( NULL );
	memset(pGray, 0, (nTempW * nTempH) * sizeof(float));
	
	//��λ��
	for(y = nY1; y < nY2; y++ )
	{
		//ԭͼ������ָ�붨λ����ʼλ��
		pOldTemp = pOldBits;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
		pOldTemp += (nHeight -1 - y) * nWidthBytes;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1-1λ��
		pOldTemp += nX1;
		
		//��ͼ������ָ�붨λ����ʼλ��
		pNewTemp = pNewBits;
		//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
		pNewTemp += (nHeight -1 - y) * nWidthBytes;
		//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
		pNewTemp += nX1;
		
		//��λ��
		for(x = nX1; x < nX2; x++)
		{
			//ȡ��ԭͼ��nTempW��nTempH�����ڵ�����ֵ
			for (i = 0; i < nTempH; i++)
			{
				for (j = 0; j < nTempW; j++)
				{
					//����ָ��ָ��ǰ����
					pNeighborTemp = pOldTemp;
					//����ָ��ָ��ԭͼ���y - nTempYc + i�С�
					pNeighborTemp += nWidthBytes * (nTempYc - i);
					//��x - nTempXc + j�����ص�ָ��
					pNeighborTemp += (j - nTempXc);
					
					//��ȡ����ֵ
					Data = *pNeighborTemp;

					pGray[i * nTempW + j] = (float)Data;
				}
			}

			//����˳ߴ�
			int nSize = nTempW * nTempH;
			
			//�Ժ�ɫ���������
			Convolution(pGray, fpArray, fCoef, nSize, &Data);
			
			//��������������ͼ��
			*pNewTemp = Data;
			
			//�¾�ͼ������ָ���1
			pOldTemp++;
			pNewTemp++;
			
		}
	}
	
	//�ͷ��ڴ�
	delete [] pGray;

	//����ͼ������
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	
	return( TRUE );
}

BOOL CAreaPro::GradSharping(int Threshold, int nX1, int nY1, int nX2, int nY2,  
							CDibObject *pDibObject)
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);

	//ֻ����8λͼ��
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("Ŀǰֻ֧��8λ�Ҷ�ͼ��Ĵ���");
		return( FALSE );
	}

	//���������
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	int nTempXc = 0;
	int nTempYc = 0;
	int nTempW  = 2;
	int nTempH  = 2;

	//�Ա߽����ز�������
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 > nWidth - nTempW + nTempXc + 1) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 > nHeight - nTempH + nTempYc + 1) nY2 = nHeight - nTempH + nTempYc + 1;

	//�������
	unsigned char *pBuffer, *pBits, *pNewBits1, *pNewBits2;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;
	DWORD dwNewSize;

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( NULL );

	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	HGLOBAL hNewDib1, hNewDib2;

	//��ͼ���ļ���С�����ֽ�Ϊ��λ��
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDib1 = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//�ڴ����ʧ��
	if( hNewDib1 == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits1 = (unsigned char *) ::GlobalLock( hNewDib1 );
	if( pNewBits1 == NULL )
	{
		::GlobalFree( hNewDib1 );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//����ͼ������
	memcpy(pNewBits1, pBits, nWidthBytes * nHeight);

	//Ϊ��ͼ������ڴ�
	hNewDib2 = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//�ڴ����ʧ��
	if( hNewDib2 == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits2 = (unsigned char *) ::GlobalLock( hNewDib2 );
	if( pNewBits2 == NULL )
	{
		::GlobalFree( hNewDib2 );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//����ͼ������
	memcpy(pNewBits2, pBits, nWidthBytes * nHeight);

	float aTemplate[4];

	//Roberts H1ģ��
	aTemplate[0] =  1.0;
	aTemplate[1] =  0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] =  0.0;

	//����Template��������
	if(!TemplateOperation(aTemplate, 1.0, nTempW, nTempH, nTempXc, nTempYc,
						pNewBits1, nWidthBytes, 
						nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	//Roberts H2ģ��
	aTemplate[0] =  0.0;
	aTemplate[1] =  1.0;
	aTemplate[2] =  0.0;
	aTemplate[3] = -1.0;

	//����Template��������
	if(!TemplateOperation(aTemplate, 1.0, nTempW, nTempH, nTempXc, nTempYc,
						pNewBits2, nWidthBytes, 
						nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	//������ͼ�����ݲ����йصı���
	unsigned char *pOldTemp, *pNewTemp1, *pNewTemp2;
	int x, y;

	DWORD dwTemp;

	switch(m_pDibObject->GetNumBits())
	{
	case 8:			//8λͼ��
		//��λ��
		for(y = nY1; y <= nY2; y++ )
		{
			//ԭͼ������ָ�붨λ����ʼλ��
			pOldTemp = pBits;
			//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pOldTemp += y * nWidthBytes;
			//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1-1λ��
			pOldTemp += nX1;

			//��ͼ������ָ�붨λ����ʼλ��
			pNewTemp1 = pNewBits1;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pNewTemp1 += y * nWidthBytes;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pNewTemp1 += nX1;

			//��ͼ������ָ�붨λ����ʼλ��
			pNewTemp2 = pNewBits2;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pNewTemp2 += y * nWidthBytes;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pNewTemp2 += nX1;

			//��λ��
			for(x = nX1; x <= nX2; x++)
			{
				//����Ŀ��ͼ���е�ǰ�����������ֵ
				dwTemp = (DWORD)(pNewTemp1[x] + pNewTemp2[x]);
				//dwTemp = (DWORD)(sqrt(pNewTemp1[x] * pNewTemp1[x] 
				//					+ pNewTemp2[x] * pNewTemp2[x]));
				//�ж��Ƿ�С����ֵ
				if(dwTemp < 255)
				{
					//�ж��Ƿ������ֵ������С�ڵ�������Ҷ�ֵ����
					if((int)dwTemp >= Threshold)
					{
						pOldTemp[x] = (unsigned char)dwTemp;
					}
				}
				else
				{
					//ֱ�Ӹ�ֵΪ255
					pOldTemp[x] = 255;
				}
			}
		}
		break;
	}

	//�ڴ����
	::GlobalUnlock(m_pDibObject->GetDib());

	//�ͷŲ���ʹ�õ��ڴ�
	::GlobalUnlock( hNewDib1 );
	::GlobalFree( hNewDib1 );
	::GlobalUnlock( hNewDib2 );
	::GlobalFree( hNewDib2 );

	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL TemplateRotating()   
//----------------------------------------------------------------------
//�������ܣ��ú������ڶԴ����ģ�������ת�������Ի�ȡ��ͬ�����ϵ�ģ�塣
//----------------------------------------------------------------------
//����˵����float *fpTemplate		ָ��ģ�������ָ��
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
//ע    �⣺�˺���Ϊ�����ͣ�ֻ����CAreaPro����ʹ�ã���ֻ������3��3��ģ�塣
//			�˺���ÿ����ת45��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CAreaPro::TemplateRotating(float *fpTemplate)
{
	float fTemp[8];
	float fDataTemp;

	//��ģ����Χֵ��˳ʱ�뷽����һ��ʱ����
	fTemp[0] = fpTemplate[0];
	fTemp[1] = fpTemplate[1];
	fTemp[2] = fpTemplate[2];
	fTemp[3] = fpTemplate[5];
	fTemp[4] = fpTemplate[8];
	fTemp[5] = fpTemplate[7];
	fTemp[6] = fpTemplate[6];
	fTemp[7] = fpTemplate[3];

	//f����������ת
	fDataTemp = fTemp[7];
	
	for(int i = 7; i > 0; i--)
	{
		fTemp[i] = fTemp[i - 1];
	}
	
	fTemp[0] = fDataTemp;

	//��ý��
	fpTemplate[0] = fTemp[0];
	fpTemplate[1] = fTemp[1];
	fpTemplate[2] = fTemp[2];
	fpTemplate[5] = fTemp[3];
	fpTemplate[8] = fTemp[4];
	fpTemplate[7] = fTemp[5];
	fpTemplate[6] = fTemp[6];
	fpTemplate[3] = fTemp[7];
}

////////////////////////////////////////////////////////////////////////
//BOOL GetMedian()   
//----------------------------------------------------------------------
//�������ܣ���������ָ����һ������������򣬲��õ�����ֵ��
//----------------------------------------------------------------------
//����˵����int nType					����ʽ����ȡֵ���£�
//											0����ð������
//											1������������
//			unsigned char *pData		����
//			int nSize					�����С
//----------------------------------------------------------------------
//��    �أ��ޡ�
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ˽���ͣ�ֻ����CAreaPro����ʹ��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
unsigned char CAreaPro::GetMedian(unsigned char *pData, int nSize, int nType)
{
	int nMedian;

	//ָ���������㷨����ʶ����ð�������㷨��
	if(nType < 0 || nType > 1) nType = 1;

	switch( nType )
	{
	case 0: //ð������
		{
			int nTemp;
			int nLast = nSize - 1;
			bool bSorted = true;
			
			do
			{
				bSorted = true;
				for (int i = 0; i < nLast; i++)
				{
					if (pData[i] > pData[i + 1])
					{	
						nTemp = pData[i];
						pData[i] = pData[i + 1];
						pData[i + 1] = nTemp;
						bSorted = false;
					}
					
				}
				nLast--;
			} while (!bSorted);
			
		}
		nMedian = pData[nSize / 2];
		break;
	case 1:
		{
			int i;
			int nHisto[256];
			for( i = 0; i < 256; i++) nHisto[ i ] = 0;
			//ͳ����ͬ�Ҷ�ֵ���ֵĴ���				
			for( i = 0; i < nSize; i++)
			{    
				nHisto[(int)pData[i]]++;
			}

			//ȡֱ��ͼ����ֵ
			int nCount = 0;
			for( i = 0; i < 256; i++)
			{ 
				//���ֱ��ͼ����ͬ�Ҷ�ֵ���ִ������ۼӺ�
				nCount += nHisto[i];
				if(nCount >= (int)(nSize / 2 + 1))
				{  
					nMedian = i;
					break;
				}
			}			
		}
		break;
		
	}
	return nMedian;
}

////////////////////////////////////////////////////////////////////////
//BOOL EdgeTracing()   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObjectͼ���������������١�
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject	ͼ�����ָ��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::EdgeTracing(CDibObject *pDibObject)
{
   //ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//ֻ֧��8λͼ��
	if(m_pDibObject->GetNumBits() != 8)
	{
		// ��ʾ�û��������ô���
		AfxMessageBox("ֻ֧��8λͼ��,���������룡");
		
		// ����
		return( FALSE );
	}
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, x, y;

	//�Ƿ��ҵ���ʼ�㼰�ص���ʼ��
	bool bFindStartPoint;

	//�Ƿ�ɨ�赽һ���߽��
	bool bFindPoint;

	//��ʼ�߽���뵱ǰ�߽��
	CPoint StartPoint,CurrentPoint;

	//�˸��������ʼɨ�跽��
	int Direction[8][2]={
							{-1, 1},
							{ 0, 1},
							{ 1, 1},
							{ 1, 0},
							{ 1,-1},
							{ 0,-1},
							{-1,-1},
							{-1, 0}
						};
	int BeginDirect;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//Ϊ��ͼ�������������ڴ�
	DWORD dwNewSize;
	HGLOBAL hNewDibBits;
	//��ͼ����������С
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDibBits = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDibBits == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��255����ɫ�������ͼ��������
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );
	//��ԭͼ�����������ͼ��������
	//memcpy( pNewBits, pOldBits, nWidthBytes * nHeight );

	//���������·��ı߽��
	bFindStartPoint = false;

	for (y = 0; y < nHeight && !bFindStartPoint; y++)
	{
		pOldTemp = pOldBits;
		pOldTemp +=  y * nWidthBytes;
		pNewTemp = pNewBits;
		pNewTemp +=  y * nWidthBytes;
		for(x = 0; x < nWidth && !bFindStartPoint; x++)
		{
			//�ҵ������Ϸ��ı߽��
			if(pOldTemp[x] == 0)
			{
				bFindStartPoint = true;
				StartPoint.y = y;
				StartPoint.x = x;
				//��ͼ����������Ӧλ�ø�0����ɫ��			
				pNewTemp[x] = (unsigned char)0;
			}		
		}
	}

	//������ʼ���������·�������ʼɨ�������Ϸ���
	BeginDirect = 0;

	//�Ƿ���ٵ���ʼ��
	bFindStartPoint = false;

	//�ӳ�ʼ�㿪ʼɨ��
	CurrentPoint = StartPoint;

	//�߽����
	while(!bFindStartPoint)
	{
		bFindPoint = false;
		while(!bFindPoint)
		{
			//��ɨ�跽��鿴һ������
			pOldTemp = pOldBits + nWidthBytes * 
				( CurrentPoint.y + Direction[BeginDirect][1])
				+ (CurrentPoint.x + Direction[BeginDirect][0]);
			if(*pOldTemp == 0)
			{
				bFindPoint = true;
				CurrentPoint.y = CurrentPoint.y + 
								Direction[BeginDirect][1];
				CurrentPoint.x = CurrentPoint.x + 
								Direction[BeginDirect][0];
				if(CurrentPoint == StartPoint)
				{
					bFindStartPoint = true;
				}
				pNewTemp = pNewBits + nWidthBytes * 
									CurrentPoint.y + 
									CurrentPoint.x;
				*pNewTemp = (unsigned char)0;

				//ɨ��ķ�����ʱ����ת����
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
			}
			else
			{
				//ɨ�跽��˳ʱ����תһ��
				BeginDirect++;
				if(BeginDirect == 8)
					BeginDirect = 0;

			}

		}
	}

	//����ͼ���������ԭͼ��������
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL ContourPickUp()   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObjectͼ��������������ȡ��
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject	ͼ�����ָ��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::ContourPickUp(CDibObject *pDibObject)
{
   //ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//ֻ֧��8λͼ��
	if(m_pDibObject->GetNumBits() != 8)
	{
		// ��ʾ�û��������ô���
		AfxMessageBox("ֻ֧��8λͼ��,���������룡");
		
		// ����
		return( FALSE );
	}
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, x, y;

	//�˸�����
	unsigned char n,e,s,w,ne,se,nw,sw;

	//��ʱ�������ֵ
	unsigned char pixel;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//Ϊ��ͼ�������������ڴ�
	DWORD dwNewSize;
	HGLOBAL hNewDibBits;
	//��ͼ����������С
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDibBits = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDibBits == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��255����ɫ�������ͼ��������
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );

	for(y = 1; y < nHeight - 1; y++)
	{
		//ָ��Դͼ������y��
		pOldTemp = pOldBits;
		pOldTemp +=  y * nWidthBytes;
		//ָ��Ŀ��ͼ������y��			
		pNewTemp = pNewBits;
		pNewTemp +=  y * nWidthBytes;
		for(x = 1; x < nWidth - 1; x++)
		{
			//ȡ�õ�ǰָ�봦������ֵ
			pixel = *pOldTemp;

			if(pixel == 0)
			{
				*pNewTemp = 0;
				nw = *(pOldTemp + nWidthBytes - 1);
				n  = *(pOldTemp + nWidthBytes);
				ne = *(pOldTemp + nWidthBytes + 1);
				w = *(pOldTemp - 1);
				e = *(pOldTemp + 1);
				sw = *(pOldTemp - nWidthBytes - 1);
				s  = *(pOldTemp - nWidthBytes );
				se = *(pOldTemp - nWidthBytes + 1);
				//������ڵİ˸��㶼�Ǻڵ�
				if(nw + n + ne + w + e + sw + s + se == 0)
				{
					*pNewTemp = 255;
				}
			}
			pOldTemp++;			
			pNewTemp++;
		}
	}

	//����ͼ���������ԭͼ��������
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL Feeding1()   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObjectͼ��������������䡣
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject	ͼ�����ָ��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::Feeding1(CDibObject *pDibObject)
{
   //ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//ֻ֧��8λͼ��
	if(m_pDibObject->GetNumBits() != 8)
	{
		// ��ʾ�û��������ô���
		AfxMessageBox("ֻ֧��8λͼ��,���������룡");
		
		// ����
		return( FALSE );
	}
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits;
	unsigned char *pOldTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors;

	//���Ӷ�ջ��ָ��
	Seed *Seeds;
	int StackPoint;

	//��ǰ����λ��
	int iCurrentPixelx,iCurrentPixely;

	//��ʱ�������ֵ
	unsigned char pixel;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//��ʼ������
	Seeds = new Seed[nWidthBytes * nHeight];
	Seeds[1].Height = nHeight / 2;
	Seeds[1].Width = nWidthBytes / 2;
	StackPoint = 1;

	while( StackPoint != 0)
	{
		//ȡ������
		iCurrentPixelx = Seeds[StackPoint].Width;
		iCurrentPixely = Seeds[StackPoint].Height;
		StackPoint--;

		pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + iCurrentPixelx;
		//ȡ�õ�ǰָ�봦������ֵ
		pixel = *pOldTemp;

		//����ǰ��Ϳ��
		*pOldTemp = 0;

		//�ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
		//ע���ֹԽ��
		if(iCurrentPixelx > 0)
		{
			pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + iCurrentPixelx - 1;
			//ȡ�õ�ǰָ�봦������ֵ
			pixel = *pOldTemp;
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = iCurrentPixelx - 1;
			}
		}

		//�ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
		//ע���ֹԽ��
		if(iCurrentPixely < nHeight - 1)
		{
			pOldTemp = pOldBits + nWidthBytes * (iCurrentPixely + 1) + iCurrentPixelx;
			//ȡ�õ�ǰָ�봦������ֵ
			pixel = *pOldTemp;
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely + 1;
				Seeds[StackPoint].Width = iCurrentPixelx;
			}
		}

		//�ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
		//ע���ֹԽ��
		if(iCurrentPixelx < nWidth - 1)
		{
			pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + iCurrentPixelx + 1;
			//ȡ�õ�ǰָ�봦������ֵ
			pixel = *pOldTemp;
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = iCurrentPixelx + 1;
			}
		}

		//�ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
		//ע���ֹԽ��
		if(iCurrentPixely > 0)
		{
			pOldTemp = pOldBits + nWidthBytes * (iCurrentPixely - 1) + iCurrentPixelx;
			//ȡ�õ�ǰָ�봦������ֵ
			pixel = *pOldTemp;
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely - 1;
				Seeds[StackPoint].Width = iCurrentPixelx;
			}
		}
	}

	//�ͷŶ�ջ
	delete Seeds;

	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL Feeding2()   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObjectͼ��������������䡣
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject	ͼ�����ָ��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::Feeding2(CDibObject *pDibObject)
{
   //ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//ֻ֧��8λͼ��
	if(m_pDibObject->GetNumBits() != 8)
	{
		// ��ʾ�û��������ô���
		AfxMessageBox("ֻ֧��8λͼ��,���������룡");
		
		// ����
		return( FALSE );
	}
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits;
	unsigned char *pOldTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors;

	//���ұ߽�����λ��
	int xl, xr, i;

	//�Ƿ���������߽�
	BOOL bFilll, bFillr;

	//���Ӷ�ջ��ָ��
	Seed Seeds[10];
	int StackPoint;

	//��ǰ����λ��
	int iCurrentPixelx, iCurrentPixely;
	int iBufferPixelx, iBufferPixely;

	//��ʱ�������ֵ
	unsigned char pixel;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//��ʼ������
	Seeds[1].Height = nHeight / 2;
	Seeds[1].Width = nWidthBytes / 2;
	StackPoint = 1;

	while( StackPoint != 0)
	{
		//ȡ������
		iCurrentPixelx = Seeds[StackPoint].Width;
		iCurrentPixely = Seeds[StackPoint].Height;
		StackPoint--;

		bFilll = true;
		bFillr = true;

		//����������ڵ���
		//�����������ص�λ��
		iBufferPixelx = iCurrentPixelx;
		iBufferPixely = iCurrentPixely;
		//���������
		while(bFilll)
		{
			pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + iCurrentPixelx;
			//ȡ�õ�ǰָ�봦������ֵ
			pixel = *pOldTemp;

			//�����߽�
			if(pixel == 0)
			{
				bFilll = false;
				xl=iCurrentPixelx+1;
			}
			else
			{
				*pOldTemp = 0;
				iCurrentPixelx--;
				//��ֹԽ��
				if(iCurrentPixelx < 0)
				{	
					bFilll = false;
					iCurrentPixelx = 0;
					xl = 0; 
				}
			}		
		}
		//���������
		//ȡ���������ص�λ��
		iCurrentPixelx = iBufferPixelx+1;
		if(iCurrentPixelx > nWidth)
		{	
			bFillr = false;
			iCurrentPixelx = nWidth;
			xr = nWidth; 
		}
		iCurrentPixely = iBufferPixely;
		while(bFillr)
		{
			pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + iCurrentPixelx;
			//ȡ�õ�ǰָ�봦������ֵ
			pixel = *pOldTemp;

			//�����߽�
			if(pixel == 0)
			{
				bFillr = false;
				xr=iCurrentPixelx-1;
			}
			else
			{
				*pOldTemp = 0;
				iCurrentPixelx++;
				//��ֹԽ��
				if(iCurrentPixelx > nWidth)
				{	
					bFillr = false;
					iCurrentPixelx = nWidth;
					xr = nWidth; 
				}
			}		
		}
		//�ϡ�������ɨ�����Ƿ�ȫΪ�߽����ػ�������
		//�ȿ������ɨ����
		iCurrentPixely--;
		if(iCurrentPixely < 0)
		{
			iCurrentPixely = 0;
		}
		for (i = xr; i >= xl;i--)
		{
			pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + i;
			//ȡ�õ�ǰָ�봦������ֵ
			pixel = *pOldTemp;

			//��δ�������أ����µ�����ѹ���ջ
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = i;
				break;
			}				
		}
		//�ٿ������ɨ����
		iCurrentPixely += 2;
		if(iCurrentPixely > nHeight)
		{
			iCurrentPixely = nHeight;
		}
		for (i = xr; i >= xl; i--)
		{
			pOldTemp = pOldBits + nWidthBytes * iCurrentPixely + i;
			//ȡ�õ�ǰָ�봦������ֵ
			pixel = *pOldTemp;

			//��δ�������أ����µ�����ѹ���ջ
			if (pixel == 255)
			{
				StackPoint++;
				Seeds[StackPoint].Height = iCurrentPixely;
				Seeds[StackPoint].Width = i;
				break;
			}				
		}


	}

	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//void AddBuffer()   
//----------------------------------------------------------------------
//�������ܣ��������Դ��������ͼ�����ݽ��мӲ���
//----------------------------------------------------------------------
//����˵����unsigned char *pOut		ָ�����ͼ�����ݵ�ָ��
//			unsigned char *pIn		ָ������ͼ�����ݵ�ָ��
//			LONG lSize				ͼ�����ݵĴ�С��nWidthBytes*nHeight��
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CAreaPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CAreaPro::AddBuffer(unsigned char *pOut, unsigned char *pIn,
						   LONG lSize)
{
	LONG i;
	BYTE Buffer;
	for(i = 0; i < lSize; i++)
	{
		Buffer = *(pIn + i);
		//ִ�������
		*(pOut + i) = *(pOut + i) + Buffer > 255 ? 255 : *(pOut + i) + Buffer;
	}
}

////////////////////////////////////////////////////////////////////////
//void AddBuffer()   
//----------------------------------------------------------------------
//�������ܣ��������Դ��������ͼ�����ݽ��мӲ���
//----------------------------------------------------------------------
//����˵����unsigned char *pOut		ָ�����ͼ�����ݵ�ָ��
//			unsigned char *pIn		ָ������ͼ�����ݵ�ָ��
//			LONG lSize				ͼ�����ݵĴ�С��nWidthBytes*nHeight��
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CAreaPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CAreaPro::MinusBuffer(unsigned char *pOut, unsigned char *pIn,
						   LONG lSize)
{
	LONG i;
	BYTE Buffer;
	for(i = 0; i < lSize; i++)
	{
		Buffer = *(pIn + i);
		//ִ�������
		*(pOut + i) = *(pOut + i) - Buffer < 0 ? 0 : *(pOut + i) - Buffer;
	}
}

////////////////////////////////////////////////////////////////////////
//BOOL ProjectH()   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObjectͼ��������ˮƽͶӰ��
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject	ͼ�����ָ��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::ProjectH(CDibObject *pDibObject)
{
   //ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//ֻ֧��8λͼ��
	if(m_pDibObject->GetNumBits() != 8)
	{
		// ��ʾ�û��������ô���
		AfxMessageBox("ֻ֧��8λͼ��,���������룡");
		
		// ����
		return( FALSE );
	}
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, x, y;

	//ͼ����ÿ���ڵĺڵ����
	long lBlackNumbers;

	//��ʱ�������ֵ
	unsigned char pixel;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//Ϊ��ͼ�������������ڴ�
	DWORD dwNewSize;
	HGLOBAL hNewDibBits;
	//��ͼ����������С
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDibBits = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDibBits == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��255����ɫ�������ͼ��������
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );

	for (y = 0;y < nHeight ;y++)
	{
		lBlackNumbers = 0;
		for(x = 0;x < nWidth ;x++)
		{
			//ָ��Դͼ������y�У���x�����ص�ָ��			
			pOldTemp = pOldBits + nWidthBytes * y + x;
	
			pixel = *pOldTemp;

			if (pixel != 255 && pixel != 0)
			{
				return false;
			}
			if(pixel == 0)
			{
				lBlackNumbers++;
			}
		}
		for(x = 0;x < lBlackNumbers ;x++)
		{	
			//ָ��Ŀ��ͼ������y�У���x�����ص�ָ��			
			pNewTemp = pNewBits + nWidthBytes * y + x;

			*pNewTemp = 0;
		}		
	}

	//����ͼ���������ԭͼ��������
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL ProjectV()   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObjectͼ�������д�ֱͶӰ��
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject	ͼ�����ָ��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::ProjectV(CDibObject *pDibObject)
{
   //ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//ֻ֧��8λͼ��
	if(m_pDibObject->GetNumBits() != 8)
	{
		// ��ʾ�û��������ô���
		AfxMessageBox("ֻ֧��8λͼ��,���������룡");
		
		// ����
		return( FALSE );
	}
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, x, y;

	//ͼ����ÿ���ڵĺڵ����
	long lBlackNumbers;

	//��ʱ�������ֵ
	unsigned char pixel;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//Ϊ��ͼ�������������ڴ�
	DWORD dwNewSize;
	HGLOBAL hNewDibBits;
	//��ͼ����������С
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDibBits = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDibBits == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��255����ɫ�������ͼ��������
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );

	for (x = 0; x < nWidth ;x++)
	{
		lBlackNumbers = 0;
		for(y = 0; y < nHeight ;y++)
		{
			//ָ��Դͼ������y�У���x�����ص�ָ��			
			pOldTemp = pOldBits + nWidthBytes * y + x;
	
			pixel = *pOldTemp;

			if(pixel == 0)
			{
				lBlackNumbers++;
			}
		}
		for(y = 0; y < lBlackNumbers; y++)
		{	
			// ָ��Ŀ��ͼ������y�У���x�����ص�ָ��			
			pNewTemp = pNewBits + nWidthBytes * y + x;

			*pNewTemp = 0;
		}		
	}

	//����ͼ���������ԭͼ��������
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}


////////////////////////////////////////////////////////////////////////
//unsigned char *GetBitsPointer()   
//----------------------------------------------------------------------
//�������ܣ���������ȡ�����CDibObjectͼ�������������ָ�롣
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject	ͼ�����ָ��
//----------------------------------------------------------------------
//��    �أ�unsigned char*
//			�ɹ�ʱ����ͼ����������ָ�룬ʧ��ʱ����NULL��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
unsigned char *CAreaPro::GetBitsPointer(CDibObject *pDibObject)
{
   //ͼ��ָ��Ϊ�գ��޷���������
	if(pDibObject == NULL) return(GetBitsPointer());
	
	//ֻ֧��8λͼ��
	if(pDibObject->GetNumBits() != 8)
	{
		// ��ʾ�û��������ô���
		AfxMessageBox("ֻ֧��8λͼ��,���������룡");
		
		// ����
		return( NULL );
	}
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = pDibObject->GetWidth();
	int nHeight = pDibObject->GetHeight();
	
	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) pDibObject->GetDIBPointer(&nWidthBytes, 
																pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( NULL );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	::GlobalUnlock( pDibObject->GetDib() );

	return(pOldBits);
}

////////////////////////////////////////////////////////////////////////
//unsigned char *GetBitsPointer()   
//----------------------------------------------------------------------
//�������ܣ���������ȡ�����CDibObjectͼ�������������ָ�롣
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//��    �أ�unsigned char*
//			�ɹ�ʱ����ͼ����������ָ�룬ʧ��ʱ����NULL��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
unsigned char *CAreaPro::GetBitsPointer()
{
   //ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(NULL);
	
	//ֻ֧��8λͼ��
	if(m_pDibObject->GetNumBits() != 8)
	{
		// ��ʾ�û��������ô���
		AfxMessageBox("ֻ֧��8λͼ��,���������룡");
		
		// ����
		return( NULL );
	}
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( NULL );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	::GlobalUnlock( m_pDibObject->GetDib() );

	return(pOldBits);
}

////////////////////////////////////////////////////////////////////////
//BOOL Minusing()   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObjectͼ�������в�Ӱ���㡣
//----------------------------------------------------------------------
//����˵����unsigned char *pTemplate	ģ��ͼ������ָ��
//			CDibObject *pDibObject		ͼ�����ָ��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::Minusing(unsigned char *pTemplate, CDibObject *pDibObject)
{
   //ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//��ȡͼ��߶ȣ�������Ϊ��λ��
	int nHeight = m_pDibObject->GetHeight();
	
	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits;
	int nWidthBytes;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer(&nWidthBytes, 
																m_pDibObject->GetNumBits() );

	pOldBits = GetBitsPointer();

	MinusBuffer(pOldBits, pTemplate, nWidthBytes * nHeight);

	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL TemplateMatch()   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObjectͼ��������ͼ��ƥ�����㡣
//----------------------------------------------------------------------
//����˵����CDibObject *pTemplate	ģ��ͼ������ָ��
//			CDibObject *pDibObject		ͼ�����ָ��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CAreaPro::TemplateMatch(CDibObject *pTemplate, CDibObject *pDibObject)
{
   //ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	//ÿ����λ��
	int nNumBits = m_pDibObject->GetNumBits();

	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nTempWidth = pTemplate->GetWidth();
	int nTempHeight = pTemplate->GetHeight();
	//ÿ����λ��
	int nTempNumBits = pTemplate->GetNumBits();
	
	//�������
	unsigned char *pOldBits, *pTempBits, *pNewBits;
	int nWidthBytes, nTempWidthBytes;

	//���ͼ����ֽڿ��
	nWidthBytes = m_pDibObject->WidthBytes(nNumBits, nWidth);
	//���ģ��ͼ����ֽڿ��
	nTempWidthBytes = pTemplate->WidthBytes(nTempNumBits, nTempWidth);

	//���ͼ���������ָ��
	pOldBits = GetBitsPointer();
	//���ģ��ͼ���������ָ��
	pTempBits = GetBitsPointer(pTemplate);

	//����һ����ͼ������Դͼ��ĸ��ƽ��
	CDibObject *pDibNew = new CDibObject(m_pDibObject);
	//�����ͼ���������ָ��
	pNewBits = GetBitsPointer(pDibNew);

	//��255����ɫ�������ͼ��������
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );

	//ѭ������
	int i, j, m, n;

	//�м���
	double dSigmaST;
	double dSigmaS;
	double dSigmaT;

	//�����Բ��
	double R;

	//��������Բ��
	double MaxR;

	//��������Գ���λ��
	int nMaxWidth, nMaxHeight;

	//����ֵ
	unsigned char pixel, templatepixel;

	unsigned char *pOldTemp, *pTemplateTemp, *pNewTemp;

	//����dSigmaT
	dSigmaT = 0;

	for (n = 0; n < nTempHeight; n++)
	{
		for(m = 0; m < nTempWidth; m++)
		{
			// ָ��ģ��ͼ������j�У���i�����ص�ָ��			
			pTemplateTemp = pTempBits + nTempWidthBytes * n + m;
			templatepixel = *pTemplateTemp;
			dSigmaT += (double)templatepixel * templatepixel;
		}
	}

	//�ҵ�ͼ������������Եĳ���λ��
	MaxR = 0.0;
	for (j = 0; j < nHeight - nTempHeight + 1; j++)
	{
		for(i = 0; i < nWidth - nTempWidth + 1; i++)
		{
			dSigmaST = 0;
			dSigmaS = 0;
	
			for (n = 0; n < nTempHeight; n++)
			{
				for(m = 0; m < nTempWidth; m++)
				{
					//ָ��Դͼ������j+n�У���i+m�����ص�ָ��			
					pOldTemp  = pOldBits + nWidthBytes * (j + n) + (i + m);
			
					//ָ��ģ��ͼ������n�У���m�����ص�ָ��			
					pTemplateTemp  = pTempBits + nTempWidthBytes * n + m;

					pixel = *pOldTemp;
					templatepixel = *pTemplateTemp;

					dSigmaS += (double)pixel * pixel;
					dSigmaST += (double)pixel * templatepixel;
				}
			}
			//����������
			R = dSigmaST / ( sqrt(dSigmaS) * sqrt(dSigmaT));
			//����������ԱȽ�
			if (R > MaxR)
			{
				MaxR = R;
				nMaxWidth = i;
				nMaxHeight = j;
			}
		}
	}

	//����������Գ������򲿷ָ��Ƶ�Ŀ��ͼ��
	for (n = 0; n < nTempHeight; n++)
	{
		for(m = 0; m < nTempWidth; m++)
		{
			pTemplateTemp = pTempBits + nTempWidthBytes * n + m;
			pNewTemp = pNewBits + nWidthBytes * (n+nMaxHeight) + (m+nMaxWidth);
			*pNewTemp = *pTemplateTemp;
		}
	}

	//����ͼ���������ԭͼ��������
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	delete pDibNew;

	::GlobalUnlock( m_pDibObject->GetDib() );

	return(TRUE);
}
