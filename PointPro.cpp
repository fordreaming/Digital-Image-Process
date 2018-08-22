// PointPro.cpp: implementation of the CPointPro class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PointPro.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPointPro, CObject)

//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CPointPro::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
}
#endif

#ifdef _DEBUG
void CPointPro::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//Check CDibObject members...
	ASSERT(m_pDibObject != NULL);	//Must exist
}
#endif

/***********************************************************************
*                                                                      *
*                               �㴦��                                 *
*                                                                      *
***********************************************************************/

////////////////////////////////////////////////////////////////////////
//CPointPro()   
//----------------------------------------------------------------------
//�������ܣ�����CPointPro��������ڴ˹��캯��û��
//			CDibObject�࣬���Ա���Ϊ�������õ���CPointPro��
//			���ṩһ��CDibObject��������
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CPointPro::CPointPro()
{
}

////////////////////////////////////////////////////////////////////////
//CPointPro(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//�������ܣ�����CPointPro��������ڴ˹��캯����һ��
//			CDibObject�࣬��������Ϊ�������õ���CPointPro��
//			���ṩһ��CDibObject������
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CPointPro::CPointPro( CDibObject *pDibObject )
{

	m_pDibObject = pDibObject;

}

////////////////////////////////////////////////////////////////////////
//BOOL MakeGray256(BYTE mGrayType, CDibObject *pDibObject)   
//----------------------------------------------------------------------
//�������ܣ��������������CDibObject�����е�ͼ��Ӳ�ɫת��Ϊ�Ҷ�ͼ��
//			������д˵���֮ǰû��ָ��һ��CDibObject����ָ�룬�������
//			����ʱ����ָ����
//----------------------------------------------------------------------
//����˵����BYTE mGrayType  0��Y=0.3R+0.59G+0.11B 
//							1: Y=R 
//							2: Y=G
//							3: Y=B					
//			CDibObject *pDibObject, Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ��ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::MakeGray256(BYTE mGrayType, 
									CDibObject *pDibObject )
{
	//CDibObject����ָ��
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��δָ�� CDibObject ����ָ�뷵��FALSE
	if( m_pDibObject == NULL ) return( FALSE );
	//����8λ��ͼ�񲻽��д���
	if( m_pDibObject->GetNumBits() < 8 ) return( FALSE );


	//��ȡԭͼ���ֽڿ�Ⱥ�ת�����8λ256ɫ�Ҷ�ͼ����ֽڿ��
	int nOldWidthBytes, nNewWidthBytes;
	char *pBuffer = (char *) m_pDibObject->GetDIBPointer( &nOldWidthBytes, 8, &nNewWidthBytes );
	if( pBuffer == NULL ) return( FALSE );

	//�������
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	RGBQUAD *pOldRGBPalette, *pNewRGBPalette;
	unsigned char *pOldBits, *pNewBits, *pTemp, *pNewTemp;
	int nNumColors, nNumNewColors;

	//��ȡ�ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pBuffer;
	//��ȡ��Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//��ȡ��ɫ��ָ��
	pOldRGBPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��ͼ����ɫ��
	nNumNewColors = 256;
	//��ȡԭͼ������ָ��
	pOldBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD)];

	//Ϊ��ͼ������ڴ�
	HGLOBAL hGlobal;
	//��ͼ�����ֽ���
	DWORD dwSize;
	dwSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) +
			256 * sizeof( RGBQUAD ) + 
			m_pDibObject->GetHeight() * nNewWidthBytes;
	hGlobal = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );
	if( hGlobal == NULL )
	{
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	pBuffer = (char *) ::GlobalLock( hGlobal );
	if( pBuffer == NULL )
	{
		::GlobalFree( hGlobal );
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//�����ͼ����ļ�ͷָ��
	pNewBFH = (BITMAPFILEHEADER *) pBuffer;
	//�����ͼ�����Ϣͷָ��
	pNewBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//�����ͼ��ĵ�ɫ��ָ��
	pNewRGBPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)
						+sizeof(BITMAPINFOHEADER)];
	//����ԭͼ���ļ�ͷ���ݵ���ͼ���ļ�ͷ
	*pNewBFH = *pOldBFH;
	//����ԭͼ����Ϣͷ���ݵ���ͼ����Ϣͷ
	*pNewBIH = *pOldBIH;

	//ѭ����������
	int i, j = 256, x, y;

	pNewBIH->biBitCount = 8;
	pNewBIH->biSizeImage = nNewWidthBytes * m_pDibObject->GetHeight();
	pNewBIH->biClrUsed = 256;
	pNewBFH->bfSize =	sizeof( BITMAPFILEHEADER ) + 
						sizeof( BITMAPINFOHEADER ) + 
						256 * sizeof( RGBQUAD ) + 
						pNewBIH->biSizeImage;
	pNewBFH->bfOffBits =	sizeof( BITMAPFILEHEADER ) + 
							sizeof( BITMAPINFOHEADER ) + 
							nNumNewColors * sizeof( RGBQUAD );
	pNewBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
	m_pDibObject->SetPaletteBytes( 256 * sizeof( RGBQUAD ));
	
	//����256ɫ�Ҷȵ�ɫ��
	for( i = 0; i < j; i++ )
	{
		pNewRGBPalette[i].rgbRed = i;
		pNewRGBPalette[i].rgbGreen = i;
		pNewRGBPalette[i].rgbBlue = i;
	}


	unsigned char *pLookup;	//��ɫ����ұ�
	DWORD dwGray;	//�Ҷȼ���

	switch( m_pDibObject->GetNumBits() )
	{
	case 8:		//256ɫͼ��
		pLookup = new unsigned char [256];
		if( pLookup == NULL ) break;
		memset( pLookup, 0, 256 );	//��ɫ����ұ���0��256�
		switch( mGrayType)
		{
		case 0:	//������Y=0.3R+0.59G+0.11B����ɫͼ��ת��Ϊ�Ҷ�ͼ��
			for( i=0; i<256; i++ )
			{
				dwGray = ( (DWORD) pOldRGBPalette[i].rgbRed * 30 +
					(DWORD) pOldRGBPalette[i].rgbGreen * 59 +
					(DWORD) pOldRGBPalette[i].rgbBlue * 11 ) / 100;
				pLookup[i] = (unsigned char) dwGray;
			}
			break;
		case 1:	//������Y=R����ɫͼ��ת��Ϊ�Ҷ�ͼ��
			for( i=0; i<256; i++ )
			{
				dwGray =  (DWORD) pOldRGBPalette[i].rgbRed;
				pLookup[i] = (unsigned char) dwGray;
			}
			break;
		case 2:	//������Y=G����ɫͼ��ת��Ϊ�Ҷ�ͼ��
			for( i=0; i<256; i++ )
			{
				dwGray = (DWORD) pOldRGBPalette[i].rgbGreen;
				pLookup[i] = (unsigned char) dwGray;
			}
			break;
		case 3:	//������Y=B����ɫͼ��ת��Ϊ�Ҷ�ͼ��
			for( i=0; i<256; i++ )
			{
				dwGray =(DWORD) pOldRGBPalette[i].rgbBlue;
				pLookup[i] = (unsigned char) dwGray;
			}
			break;
		}
		for( y = 0; y < pOldBIH->biHeight; y++ )
		{
			pTemp = pOldBits;	//λͼ������ʼָ��
			pTemp += y * nOldWidthBytes;	//λͼ������һ����ʼָ��
			//ת���ɻҶ�����
			for( x = 0; x < pOldBIH->biWidth; x++ ) pTemp[x] = pLookup[pTemp[x]];
		}
		delete [] pLookup;	//�ͷ�pLookup���ұ���ռ�ڴ�
		
		memcpy( pNewBits, pOldBits, nNewWidthBytes * m_pDibObject->GetHeight());
		
		break;
	case 16:	//16λɫ���ɫͼ��
		unsigned char ucRed, ucGreen, ucBlue;
		for( y=0; y<pOldBIH->biHeight; y++ )
		{
			//λͼ������ʼָ��
			pTemp = pOldBits;	
			pNewTemp = pNewBits;
			//λͼ������һ����ʼָ��
			pTemp += y * nOldWidthBytes;	
			pNewTemp += y * nNewWidthBytes;
			switch( mGrayType )
			{
			case 0:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					GETRGB555( ucRed, ucGreen, ucBlue, &pTemp[x*2] );
					//������Y=0.3R+0.59G+0.11B����ɫͼ��ת��Ϊ�Ҷ�ͼ��
					dwGray = (ucRed * 30 + ucGreen * 59 +ucBlue * 11) / 100;
					//����ͼ�����ݸ�ֵ
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 1:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					GETRGB555( ucRed, ucGreen, ucBlue, &pTemp[x*2] );
					//������Y=R����ɫͼ��ת��Ϊ�Ҷ�ͼ��
					dwGray = (ucRed * 30 + ucGreen * 59 +ucBlue * 11) / 100;
					//����ͼ�����ݸ�ֵ
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 2:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					GETRGB555( ucRed, ucGreen, ucBlue, &pTemp[x*2] );
					//������Y=G����ɫͼ��ת��Ϊ�Ҷ�ͼ��
					dwGray = (ucRed * 30 + ucGreen * 59 +ucBlue * 11) / 100;
					//����ͼ�����ݸ�ֵ
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 3:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					GETRGB555( ucRed, ucGreen, ucBlue, &pTemp[x*2] );
					//������Y=B����ɫͼ��ת��Ϊ�Ҷ�ͼ��
					dwGray = (ucRed * 30 + ucGreen * 59 +ucBlue * 11) / 100;
					//����ͼ�����ݸ�ֵ
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			}
		}
		break;
	case 24:	//24λ���ɫͼ��
		for( y=0; y<pOldBIH->biHeight; y++ )
		{
			//λͼ������ʼָ��
			pTemp = pOldBits;	
			pNewTemp = pNewBits;
			//λͼ������һ����ʼָ��
			pTemp += y * nOldWidthBytes;	
			pNewTemp += y * nNewWidthBytes;
			switch( mGrayType )
			{
			case 0:	//������Y=0.3R+0.59G+0.11B����ɫͼ��ת��Ϊ�Ҷ�ͼ��
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					dwGray = ( (DWORD) pTemp[x*3+2] * 30	//��ɫ
						+(DWORD) pTemp[x*3+1] * 59		//��ɫ
						+(DWORD) pTemp[x*3] * 11		//��ɫ
						) / 100;
					//����ͼ�����ݸ�ֵ
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 1:	//������Y=R����ɫͼ��ת��Ϊ�Ҷ�ͼ��
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					dwGray =  (DWORD) pTemp[x*3+2]; //��ɫ
					//����ͼ�����ݸ�ֵ
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 2:	//������Y=G����ɫͼ��ת��Ϊ�Ҷ�ͼ��
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					dwGray = (DWORD) pTemp[x*3+1] ;	//��ɫ
					//����ͼ�����ݸ�ֵ
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 3:	//������Y=B����ɫͼ��ת��Ϊ�Ҷ�ͼ��
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					dwGray =(DWORD) pTemp[x*3];	//��ɫ
					//����ͼ�����ݸ�ֵ
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			}
		}
		break;
	case 32:	//32λ���ɫͼ��
		for( y=0; y<pOldBIH->biHeight; y++ )
		{
			//λͼ������ʼָ��
			pTemp = pOldBits;	
			pNewTemp = pNewBits;
			//λͼ������һ����ʼָ��
			pTemp += y * nOldWidthBytes;	
			pNewTemp += y * nNewWidthBytes;
			switch( mGrayType )
			{
			case 0:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					//������Y=0.3R+0.59G+0.11B����ɫͼ��ת��Ϊ�Ҷ�ͼ��
					dwGray = ( (DWORD) pTemp[x*4] * 30 //��ɫ
						+(DWORD) pTemp[x*4+1] * 59	//��ɫ
						+(DWORD) pTemp[x*4+2] * 11 //��ɫ
						) / 100;
					//����ͼ�����ݸ�ֵ
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 1:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					//������Y=R����ɫͼ��ת��Ϊ�Ҷ�ͼ��
					dwGray =  (DWORD) pTemp[x*4];//��ɫ
					//����ͼ�����ݸ�ֵ
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 2:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					//������Y=G����ɫͼ��ת��Ϊ�Ҷ�ͼ��
					dwGray = (DWORD) pTemp[x*4+1] ;	//��ɫ
					//����ͼ�����ݸ�ֵ
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			case 3:
				for( x=0; x<pOldBIH->biWidth; x++ )
				{
					//������Y=B����ɫͼ��ת��Ϊ�Ҷ�ͼ��
					dwGray =(DWORD) pTemp[x*4+2] ; //��ɫ
					//����ͼ�����ݸ�ֵ
					pNewTemp[x] = (unsigned char)dwGray;
				}
				break;
			}
		}
		break;
	}
			
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hGlobal );
	m_pDibObject->SetDib( hGlobal );

	m_pDibObject->ProcessImageHeader();

	m_pDibObject->m_nLastError = IMAGELIB_SUCCESS;

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//void SetDibObjectClass(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//�������ܣ�������ΪCImagePointProcess�����ָ��һ��CDibObject����ָ��
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ��ޡ�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CPointPro::SetDibObjectClass( CDibObject *pDibObject )
{

	m_pDibObject = pDibObject;

}

////////////////////////////////////////////////////////////////////////
//int *CreateHistogram()   
//----------------------------------------------------------------------
//�������ܣ����������������CDibObject������ͼ���ֱ��ͼ��������д˵�
//			��֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���ʱ����ָ
//			�����κ�δ���������ֵ��Ĭ�ϵ�-1����ֵ��������Ϊͼ������ֵ
//			�����ֲ������nX1��nY1������Ϊ0��nX2������Ϊͼ���ȼ�1��nY2
//			������Ϊͼ��߶ȼ�1����Ҫ������ͼ���Ͻ��в���ʱ����õķ���
//			�ǲ�����nX1��nY1��nX2��nY2ֵ���������ǻᱻĬ��Ϊ����ͼ��
//----------------------------------------------------------------------
//����˵����int	nX1						Ĭ��Ϊ-1
//			int	nY1						Ĭ��Ϊ-1
//			int	nX2						Ĭ��Ϊ-1
//			int	nY2						Ĭ��Ϊ-1
//			unsigned char *pData		ͼ��λͼ����ָ��
//			RGBQUAD *pPalette			ͼ���ɫ��ָ��
//			int nWidthBytes				ͼ���ֽڿ��
//			CDibObject *pDibObject	Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ�ֱ��ͼ����ָ��*pBuffer���У�
//			pBuffer[]		�洢����ֱ��ͼ����
//			pBuffer[256]	�洢��ɫֱ��ͼ����
//			pBuffer[512]	�洢��ɫֱ��ͼ����
//			pBuffer[768]	�洢��ֱ��ͼ����
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int *CPointPro::CreateHistogram( int nX1, int nY1, int nX2, int nY2, 
									   unsigned char *pData, RGBQUAD *pPalette, 
									   int nWidthBytes, CDibObject *pDibObject )
{

	//ͼ��ָ��Ϊ�գ��޷���������
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	if( m_pDibObject == NULL ) return( FALSE );

	//����ֱ��ͼ���ݻ����������飩
	int *pBuffer = new int [256 * 4];
	//����ֱ��ͼ���ݻ�����ʧ��
	if( pBuffer == NULL ) return( NULL );
	//ֱ��ͼ���ݻ���������
	memset( pBuffer, 0, ( 256 * 4) * sizeof( int ) );

	//��������
	DWORD dwGray;
	int x, y;
	unsigned char *pTemp, ucRed, ucGreen, ucBlue;

	//ͼ��ĸ߶�
	int nHeight = m_pDibObject->GetHeight();

	switch( m_pDibObject->GetNumBits() )
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
			pTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
			//����ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pTemp += nX1;
			for( x = nX1; x <= nX2; x++ )
			{
				//pTemp[x]Ϊ��ǰ����ֵ����Ϊ��ɫ���������ֵ��
				//�Դ�Ϊ������ȡ����ɫ�������Ӧ����������ֵ��
				ucRed   = pPalette[pTemp[x]].rgbRed;
				ucGreen = pPalette[pTemp[x]].rgbGreen;
				ucBlue  = pPalette[pTemp[x]].rgbBlue;

				//����ϵL=0.3R+0.59G+0.11B���õ�����ֵ
				dwGray  = ( (DWORD) ucRed * 30 +
					        (DWORD) ucGreen * 59 +
					        (DWORD) ucBlue * 11 ) / 100;
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
		for( y = nY1; y <= nY2; y++ )
		{
			//����ָ�붨λ��ͼ��������ʼλ��
			pTemp = pData;
			//����ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
			//����ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pTemp += ( nX1 * 2 );
			for( x = nX1; x <= nX2; x++ )
			{
				//��ȡ��ԭɫ����
				GETRGB555( ucRed, ucGreen, ucBlue, pTemp );

				//����ϵL=0.3R+0.59G+0.11B���õ�����ֵ
				dwGray = ( (DWORD) ucRed * 30 +
					       (DWORD) ucGreen * 59 +
					       (DWORD) ucBlue * 11 ) / 100;
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
		for( y = nY1; y < nY2; y++ )
		{
			//����ָ�붨λ��ͼ��������ʼλ��
			pTemp = pData;
			//����ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
			//����ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pTemp += ( nX1 * 3 );

			for( x=nX1; x<=nX2; x++ )
			{
				//��ȡ������ɫ����ԭɫ��
				ucRed   = pTemp[x * 3 + 2];
				ucGreen = pTemp[x * 3 + 1];
				ucBlue  = pTemp[x * 3];

				//����ϵL=0.3R+0.59G+0.11B���õ�����ֵ
				dwGray  = ( (DWORD) ucRed * 30 +
					        (DWORD) ucGreen * 59 +
					        (DWORD) ucBlue * 11 ) / 100;
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
		for( y = nY1; y <= nY2; y++ )
		{
			//����ָ�붨λ��ͼ��������ʼλ��
			pTemp = pData;
			//����ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
			//����ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pTemp += ( nX1 * 4 );

			for( x = nX1; x <= nX2; x++ )
			{
				//��ȡ������ɫ����ԭɫ��
				GETRGB888( ucRed, ucGreen, ucBlue, pTemp );

				//����ϵL=0.3R+0.59G+0.11B���õ�����ֵ
				dwGray = ( (DWORD) ucRed * 30 +
					       (DWORD) ucGreen * 59 +
					       (DWORD) ucBlue * 11 ) / 100;

				dwGray &= 0x000000ff;

				//����ֱ��ͼ����
				pBuffer[dwGray]++;
				//��ɫֱ��ͼ����
				pBuffer[256 + ucRed]++;
				//��ɫֱ��ͼ����
				pBuffer[512 + ucGreen]++;
				//��ɫֱ��ͼ����
				pBuffer[768 + ucBlue]++;

				pTemp += 4;
			}
		}
		break;
	}
	
	return( pBuffer );

}

////////////////////////////////////////////////////////////////////////
//int FindThresh_Auto( CDibObject *pDibObject )   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ�������Ե�����ֵ��ȷ����
//			ֵ(ֻ��8λ�Ҷ�ͼ����Ч��
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ�int ��ֵ
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CPointPro::FindThresh_Auto( CDibObject *pDibObject )
{
	if( pDibObject != NULL ) m_pDibObject = pDibObject;	//CDibObject����ָ��
	if( m_pDibObject == NULL ) return( -1 );	//��δָ�� CDibObject ����ָ�뷵��FALSE

	unsigned char *pBuffer, *pBits;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, i;

	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( -1 );

	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];
	if (pBIH->biBitCount != 8) return (-1);
	//��ȡ�Ҷ�ͼ��Ŀ�Ⱥ͸߶ȣ������ر�ʾ��
	int nWidth = pBIH->biWidth;
	int nHeight = pBIH->biHeight;
	
	int *pHistogram = GetHistogram();
	
	long MaxPiex1 = 0;
	long MaxPiex2 = 0;
	unsigned char MaxGray1, MaxGray2;

	for(i = 0; i < 256; i++)
	{
		if ( pHistogram[i] > MaxPiex1)
		{
			MaxPiex1 = pHistogram[i];
			MaxGray1 = i;
		}
	}

	for(i = 0; i < 256; i++)
	{
		if ( pHistogram[i] > MaxPiex2 && pHistogram[i] < MaxPiex1)
		{
			MaxPiex2 = pHistogram[i];
			MaxGray2 = i;
		}
	}

	unsigned char Temp;
	
	if(MaxGray1 > MaxGray2)
	{
		Temp = MaxGray1;
		MaxGray1 = MaxGray2;
		MaxGray2 = MaxGray1;
	}

	int Thresh;

	long MinPiex = nWidth * nHeight;

	for(i = MaxGray1; i <= MaxGray2; i++)
	{
		if(pHistogram[i] < MinPiex) Thresh = i;
	}

	delete [] pHistogram;

	return(Thresh);
}

////////////////////////////////////////////////////////////////////////
//int FindThresh( CDibObject *pDibObject )   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ�����б������ȷ����ֵ��
//			�������ж��������������ڵ���255��ͼ�������ֱ��ͼ������Ϊ��
//			����ֵ������Դ��������������С��255��ͼ��ֱ�Ӳ���ͼ��������
//			Ϊ�����ֵ������Դ�������ɴ����߼����ٶȡ�
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ�int ��ֵ
//----------------------------------------------------------------------
//ע��		ֻ��8λ�Ҷ�ͼ����Ч
////////////////////////////////////////////////////////////////////////
int CPointPro::FindThresh_Discriminant( CDibObject *pDibObject )
{
	//CDibObject����ָ��
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��δָ�� CDibObject ����ָ�뷵��FALSE
	if( m_pDibObject == NULL ) return( -1 );

	unsigned char *pBuffer, *pBits;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, i;

	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( -1 );

	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];
	if (pBIH->biBitCount != 8) return (-1);

	//��ȡ�Ҷ�ͼ��Ŀ�Ⱥ͸߶ȣ������ر�ʾ��
	int nWidth = pBIH->biWidth;
	int nHeight = pBIH->biHeight;
	
	int *nHistogramBuffer = GetHistogram();
	
	float Sigma[256];
	float Mean1, Mean2;
	
	//Sigma[256]��������
	for( i = 0;i < 256; i++) Sigma[i] = 0.0;
	
	int nLevelLow = 0;
	int nLevelHigh = 0;
	
	//������С�Ҷ�ֵ
	i = 0;
	while(nLevelLow == 0)
	{
		if( nHistogramBuffer[i] != 0 )
		{
			nLevelLow = i;
		}
		i++;
	}

	//�������Ҷ�ֵ
	i = 255;
	while(nLevelHigh == 0)
	{
		if( nHistogramBuffer[i] != 0 )
		{
			nLevelHigh = i;
		}
		i--;
	}

	//thѭ����С����ֵth�����ع�Ϊ��һ�࣬����th�����ع�Ϊ�ڶ���
	//th=0��1��2������nLevel��
	for(int th = nLevelLow; th <= nLevelHigh; th++)   
	{
		long lGrayLow = 0;
		long lGrayHigh = 0;
		long lSumLow = 0;
		long lSumHigh = 0;
		for(i = nLevelLow; i < th; i++)
		{
			lSumLow += nHistogramBuffer[i];
			lGrayLow += i * nHistogramBuffer[i];
		}
		for(i = th; i <= nLevelHigh; i++)
		{
			lSumHigh += nHistogramBuffer[i];
			lGrayHigh += i * nHistogramBuffer[i];
		}
		
		if( lSumLow > 0 )
		{
			Mean1 = (float)lGrayLow / lSumLow;
		}
		else
		{
			Mean1 = 0.0;
		}
		if( lSumHigh > 0)
		{
			Mean2 = (float)lGrayHigh / lSumHigh;
		}
		else
		{
			Mean2 = 0.0;
		}
		
		//��������֮��ķ���
		Sigma[th] = (float)lSumLow * lSumHigh 
			* (Mean1 - Mean2) * (Mean1 - Mean2);
	}
	
	//thѭ���������󷽲�������ֵ
	float MaxSigma = Sigma[nLevelLow];
	int Thresh = 0;
	for( i = nLevelLow + 1; i <= nLevelHigh; i++)
	{
		if(MaxSigma < Sigma[i])
		{
			MaxSigma = Sigma[i];
			Thresh = i;
		}
	}
	
	delete [] nHistogramBuffer;
	return(Thresh);
	
}

////////////////////////////////////////////////////////////////////////
//BOOL Threshold( )   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ���ø�������ֵ������ֵ��
//			(ֻ��8λ�Ҷ�ͼ����Ч��
//----------------------------------------------------------------------
//����˵����int Thresh					��ֵ
//			int Mode					��ֵ��������1�����úڣ�0�����ð�
//			BOOL bSetPalette			�Ƿ����õ�ɫ��
//			CDibObject *pDibObject, Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ��ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::Threshold(int Thresh, int Mode, BOOL bSetPalette, CDibObject *pDibObject)
{
	//CDibObject����ָ��
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��δָ�� CDibObject ����ָ�뷵��FALSE
	if( m_pDibObject == NULL ) return( -1 );
	if( !IsGray256() ) return( -1 );

	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, i;

	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( -1 );

	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];

	//��ȡ�Ҷ�ͼ��Ŀ�Ⱥ͸߶ȣ������ر�ʾ��
	int nWidth = pBIH->biWidth;
	int nHeight = pBIH->biHeight;

	// �Ҷ�ӳ���
	unsigned char pMap[256];
	
	switch (Mode)
	{
	case 1:
		for( i = 0; i <= Thresh; i++) pMap[i] = 255;
		for( i = Thresh + 1; i <= 255; i++) pMap[i] = 0;
		break;
	case 0:
		for( i = 0; i <= Thresh; i++) pMap[i] = 0;
		for( i = Thresh + 1; i <= 255; i++) pMap[i] = 255;
		break;
	}

	for ( i = 0; i < nHeight; i ++)
	{
		pTemp = pBits;
		pTemp += i * nWidthBytes;	//λͼ������һ����ʼָ��
		for ( int j = 0; j < nWidth; j ++)
		{
			pTemp[j] = pMap[pTemp[j]];
		}
	}
	m_pDibObject->ProcessPalette();
		if( bSetPalette && m_pDibObject->m_nPaletteBytes > 0 )
		{
			CWindowDC WindowDC( NULL );
			m_pDibObject->SetPalette( &WindowDC );
		}

	::GlobalUnlock( m_pDibObject->GetDib() );

	m_pDibObject->m_nLastError = IMAGELIB_SUCCESS;


	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL IsGray256(CDibObject *pDibObject)
//----------------------------------------------------------------------
//�������ܣ��������жϴ����CDibObject�����е�ͼ���Ƿ�Ϊ256���Ҷ�ͼ��
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::IsGray256( CDibObject *pDibObject )
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//����8λͼ�񣬱ز���256���Ҷ�ͼ�񣬲�������FALSE
	if( m_pDibObject->GetNumBits() != 8 ) return( FALSE );

	//�������
	unsigned char *pBuffer;
	RGBQUAD *pPalette;
	int nWidthBytes, i;

	//pBuffer: ���λͼ����ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );

	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];

	//����Ƿ�Ϊ256ɫ�Ҷȵ�ɫ��
	for( i=0; i<256; i++ )
	{
		if( pPalette[i].rgbRed != pPalette[i].rgbGreen 
			|| pPalette[i].rgbRed != pPalette[i].rgbBlue
			|| pPalette[i].rgbGreen != pPalette[i].rgbBlue )
			return( FALSE);
	}
	::GlobalUnlock( m_pDibObject->GetDib() );

	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL LinerTran(CImagObject *pDibObject, float fa, float fb)   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ����лҶȵ����Ա任����
//			�����д˵���֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ�
//			��ʱ����ָ�����κ�δ���������ֵ��Ĭ�ϵ�-1����ֵ��������Ϊͼ
//			������ֵ�����ֲ������nX1��nY1������Ϊ0��nX2������Ϊͼ���
//			�ȼ�1��nY2������Ϊͼ��߶ȼ�1����Ҫ������ͼ���Ͻ��в���ʱ��
//			��õķ����ǲ�����nX1��nY1��nX2��nY2ֵ���������ǻᱻĬ��Ϊ��
//			��ͼ��
//----------------------------------------------------------------------
//����˵����float fa			���Ա任��б��
//			float fb			���Ա任�Ľؾ�
//			int	nX1,			Ĭ��Ϊ-1
//			int	nY1,			Ĭ��Ϊ-1
//			int	nX2,			Ĭ��Ϊ-1
//			int	nY2,			Ĭ��Ϊ-1
//			CDibObject *pDibObject, Ĭ��ΪNULL
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::LinerTran(float fa, float fb,
									 int nX1,
									 int nY1,
									 int nX2,
									 int nY2,
									 CDibObject *pDibObject)
{
	//CDibObject����ָ��
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��δָ�� CDibObject ����ָ�뷵��FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	BOOL bLessThanHalf, bCompleteImage;	

	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2,
										&bCompleteImage, 
										&bLessThanHalf );
	//�������
	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, x, y, i;
	
	//pBuffer: ���λͼ����ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );
	
	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
		nNumColors*sizeof(RGBQUAD)];
	
	LOGPALETTE *pLogPal = m_pDibObject->GetLogPal();
	CPalette *_pPalette = m_pDibObject->GetPalette();
	
	//�����Ҷ�ӳ���
	unsigned char *pMap = PointSlopeLinerTran(fa, fb);
			
	switch( m_pDibObject->GetNumBits() )
	{
	case 1:
		for( y=nY1; y<=nY2; y++ )
		{
			unsigned char Mask;
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y ) 
				* nWidthBytes ) + ( nX1 / 8 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				Mask = 0x80 >> ( x & 7 );
				if( pTemp[0] & Mask ) pTemp[0] &= ( Mask ^ 0xff );
				else pTemp[0] |= Mask;
				if( ( x & 7 ) == 7 ) pTemp++;
			}
		}
		break;
	case 4:
		if( bCompleteImage )
		{
			_pPalette->DeleteObject();
			for( i=0; i<nNumColors; i++ )
			{
				pPalette[i].rgbRed = pMap[pPalette[i].rgbRed];
				pPalette[i].rgbGreen = pMap[pPalette[i].rgbGreen];
				pPalette[i].rgbBlue = pMap[pPalette[i].rgbBlue];
			}
			LOGPALETTE *pLogPal = m_pDibObject->CreateLogPalette( 
								pPalette, nNumColors );
			if( pLogPal != NULL )
			{
				_pPalette->CreatePalette( pLogPal );
				delete [] pLogPal;
			}
		}
		else
		{
			unsigned char ucRed, ucGreen, ucBlue, Data;
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + ( nX1 / 2 ) );
				for( x=nX1; x<=nX2; x++ )
				{
					Data = *pTemp;
					if( !( x & 1 ) ) Data >>= 4;
					else Data &= 0x0f;
					ucRed = pPalette[Data].rgbRed;
					ucGreen = pPalette[Data].rgbGreen;
					ucBlue = pPalette[Data].rgbBlue;
					ucRed = pMap[ucRed];
					ucGreen = pMap[ucGreen];
					ucBlue = pMap[ucBlue];
					Data = (unsigned char) m_pDibObject->GetNearestIndex( ucRed, ucGreen, 
						ucBlue, pPalette, nNumColors );
					if( !( x & 1 ) )
					{
						Data <<= 4;
						(*pTemp) &= 0x0f;
						(*pTemp) |= Data;
					}
					else
					{
						(*pTemp) &= 0xf0;
						(*pTemp) |= Data;
						pTemp++;
					}
				}
			}
		}
		break;
	case 8:
		if(IsGray256())
		{
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + nX1 );
				for( x=nX1; x<=nX2; x++ )
				{
					pTemp[x] = pMap[pTemp[x]];
				}
			}
		}
		else if( bCompleteImage )
		{
			_pPalette->DeleteObject();
			for( i=0; i<nNumColors; i++ )
			{
				pPalette[i].rgbRed = pMap[pPalette[i].rgbRed];
				pPalette[i].rgbGreen = pMap[pPalette[i].rgbGreen];
				pPalette[i].rgbBlue = pMap[pPalette[i].rgbBlue];
			}
			LOGPALETTE *pLogPal = m_pDibObject->CreateLogPalette( pPalette, nNumColors );
			if( pLogPal != NULL )
			{
				_pPalette->CreatePalette( pLogPal );
				delete [] pLogPal;
			}
		}
		else
		{
			unsigned char ucRed, ucGreen, ucBlue, Data;
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + nX1 );
				for( x=nX1; x<=nX2; x++ )
				{
					Data = *pTemp;
					ucRed = pPalette[Data].rgbRed;
					ucGreen = pPalette[Data].rgbGreen;
					ucBlue = pPalette[Data].rgbBlue;
					ucRed = pMap[ucRed];
					ucGreen = pMap[ucGreen];
					ucBlue = pMap[ucBlue];
					Data = (unsigned char) m_pDibObject->GetNearestIndex( ucRed, 
						ucGreen, ucBlue, pPalette, nNumColors );
					*pTemp++ = Data;
				}
			}
		}
		break;
	case 16:
		unsigned char ucRed, ucGreen, ucBlue;
		for( y=0; y< nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( m_pDibObject->GetHeight() - 1 - y ) 
						* nWidthBytes );
			pTemp += ( nX1 * 2 );
			for( x=nX1; x<=nX2; x++ )
			{
				GETRGB555( ucRed, ucGreen, ucBlue, pTemp );
				ucRed = pMap[ucRed];
				ucGreen = pMap[ucGreen];
				ucBlue = pMap[ucBlue];
				PUTRGB555( ucRed, ucGreen, ucBlue, pTemp );
				pTemp += 2;
			}
		}
		break;
	case 24:
		for( y=nY1; y<=nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
				* nWidthBytes ) + ( nX1 * 3 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				pTemp[0] = pMap[pTemp[0]];
				pTemp[1] = pMap[pTemp[1]];
				pTemp[2] = pMap[pTemp[2]];
				pTemp += 3;
			}
		}
		break;
	case 32:
		for( y=nY1; y<=nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y ) 
				* nWidthBytes ) + ( nX1 * 4 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				pTemp[0] = pMap[pTemp[0]];
				pTemp[1] = pMap[pTemp[1]];
				pTemp[2] = pMap[pTemp[2]];
				pTemp[3] = pMap[pTemp[3]];
				pTemp += 4;
			}
		}
		break;
	}
	
	delete [] pMap;
	
	::GlobalUnlock( m_pDibObject->GetDib() );
	
	m_pDibObject->m_nLastError = IMAGELIB_SUCCESS;
	
	// ����
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL GrayStretch()
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ����лҶ�����任�����
//			���д˵���֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���
//			ʱ����ָ�����κ�δ���������ֵ��Ĭ�ϵ�-1����ֵ��������Ϊͼ��
//			�����ֵ�����ֲ������nX1��nY1������Ϊ0��nX2������Ϊͼ����
//			��1��nY2������Ϊͼ��߶ȼ�1����Ҫ������ͼ���Ͻ��в���ʱ����
//			�õķ����ǲ�����nX1��nY1��nX2��nY2ֵ���������ǻᱻĬ��Ϊ����
//			ͼ��
//----------------------------------------------------------------------
//����˵����BYTE bX1	�Ҷ������һ�����x����
//			BYTE bY1	�Ҷ������һ�����y����
//			BYTE bX2	�Ҷ�����ڶ������x����
//			BYTE bY2	�Ҷ�����ڶ������y����
//			int	nX1, Ĭ��Ϊ-1
//			int	nY1, Ĭ��Ϊ-1
//			int	nX2, Ĭ��Ϊ-1
//			int	nY2, Ĭ��Ϊ-1
//			CDibObject *pDibObject, Ĭ��ΪNULL
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::GrayStretch(BYTE bX1,
									   BYTE bY1,
									   BYTE bX2,
									   BYTE bY2,
									   int nX1,
									   int nY1,
									   int nX2,
									   int nY2, 
									   CDibObject *pDibObject)
{
	//�����Ҷ�ӳ���
	unsigned char *pMap = DoublePointLinerTran(bX1, bY1, bX2, bY2);

	//CDibObject����ָ��
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��δָ�� CDibObject ����ָ�뷵��FALSE
	if( m_pDibObject == NULL ) return( FALSE );
	BOOL bLessThanHalf, bCompleteImage;	

	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2,
										&bCompleteImage, 
										&bLessThanHalf );
	//�������
	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, x, y, i;
	
	//pBuffer: ���λͼ����ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );
	
	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
		nNumColors*sizeof(RGBQUAD)];
	
	LOGPALETTE *pLogPal = m_pDibObject->GetLogPal();
	CPalette *_pPalette = m_pDibObject->GetPalette();
	
	switch( m_pDibObject->GetNumBits() )
	{
	case 1:
		for( y=nY1; y<=nY2; y++ )
		{
			unsigned char Mask;
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y ) 
				* nWidthBytes ) + ( nX1 / 8 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				Mask = 0x80 >> ( x & 7 );
				if( pTemp[0] & Mask ) pTemp[0] &= ( Mask ^ 0xff );
				else pTemp[0] |= Mask;
				if( ( x & 7 ) == 7 ) pTemp++;
			}
		}
		break;
	case 4:
		if( bCompleteImage )
		{
			_pPalette->DeleteObject();
			for( i=0; i<nNumColors; i++ )
			{
				pPalette[i].rgbRed = pMap[pPalette[i].rgbRed];
				pPalette[i].rgbGreen = pMap[pPalette[i].rgbGreen];
				pPalette[i].rgbBlue = pMap[pPalette[i].rgbBlue];
			}
			LOGPALETTE *pLogPal = m_pDibObject->CreateLogPalette( 
								pPalette, nNumColors );
			if( pLogPal != NULL )
			{
				_pPalette->CreatePalette( pLogPal );
				delete [] pLogPal;
			}
		}
		else
		{
			unsigned char ucRed, ucGreen, ucBlue, Data;
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + ( nX1 / 2 ) );
				for( x=nX1; x<=nX2; x++ )
				{
					Data = *pTemp;
					if( !( x & 1 ) ) Data >>= 4;
					else Data &= 0x0f;
					ucRed = pPalette[Data].rgbRed;
					ucGreen = pPalette[Data].rgbGreen;
					ucBlue = pPalette[Data].rgbBlue;
					ucRed = pMap[ucRed];
					ucGreen = pMap[ucGreen];
					ucBlue = pMap[ucBlue];
					Data = (unsigned char) m_pDibObject->GetNearestIndex( ucRed, ucGreen, 
						ucBlue, pPalette, nNumColors );
					if( !( x & 1 ) )
					{
						Data <<= 4;
						(*pTemp) &= 0x0f;
						(*pTemp) |= Data;
					}
					else
					{
						(*pTemp) &= 0xf0;
						(*pTemp) |= Data;
						pTemp++;
					}
				}
			}
		}
		break;
	case 8:
		if( bCompleteImage )
		{
			_pPalette->DeleteObject();
			for( i=0; i<nNumColors; i++ )
			{
				pPalette[i].rgbRed = pMap[pPalette[i].rgbRed];
				pPalette[i].rgbGreen = pMap[pPalette[i].rgbGreen];
				pPalette[i].rgbBlue = pMap[pPalette[i].rgbBlue];
			}
			LOGPALETTE *pLogPal = m_pDibObject->CreateLogPalette( pPalette, nNumColors );
			if( pLogPal != NULL )
			{
				_pPalette->CreatePalette( pLogPal );
				delete [] pLogPal;
			}
		}
		else
		{
			unsigned char ucRed, ucGreen, ucBlue, Data;
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + nX1 );
				for( x=nX1; x<=nX2; x++ )
				{
					Data = *pTemp;
					ucRed = pPalette[Data].rgbRed;
					ucGreen = pPalette[Data].rgbGreen;
					ucBlue = pPalette[Data].rgbBlue;
					ucRed = pMap[ucRed];
					ucGreen = pMap[ucGreen];
					ucBlue = pMap[ucBlue];
					Data = (unsigned char) m_pDibObject->GetNearestIndex( ucRed, 
						ucGreen, ucBlue, pPalette, nNumColors );
					*pTemp++ = Data;
				}
			}
		}
		break;
	case 16:
		unsigned char ucRed, ucGreen, ucBlue;
		for( y=0; y< nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( m_pDibObject->GetHeight() - 1 - y ) 
						* nWidthBytes );
			pTemp += ( nX1 * 2 );
			for( x=nX1; x<=nX2; x++ )
			{
				GETRGB555( ucRed, ucGreen, ucBlue, pTemp );
				ucRed = pMap[ucRed];
				ucGreen = pMap[ucGreen];
				ucBlue = pMap[ucBlue];
				PUTRGB555( ucRed, ucGreen, ucBlue, pTemp );
				pTemp += 2;
			}
		}
		break;
	case 24:
		for( y=nY1; y<=nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
				* nWidthBytes ) + ( nX1 * 3 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				pTemp[0] = pMap[pTemp[0]];
				pTemp[1] = pMap[pTemp[1]];
				pTemp[2] = pMap[pTemp[2]];
				pTemp += 3;
			}
		}
		break;
	case 32:
		for( y=nY1; y<=nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y ) 
				* nWidthBytes ) + ( nX1 * 4 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				pTemp[0] = pMap[pTemp[0]];
				pTemp[1] = pMap[pTemp[1]];
				pTemp[2] = pMap[pTemp[2]];
				pTemp[3] = pMap[pTemp[3]];
				pTemp += 4;
			}
		}
		break;
	}
	
	delete [] pMap;
	
	::GlobalUnlock( m_pDibObject->GetDib() );
	
	m_pDibObject->m_nLastError = IMAGELIB_SUCCESS;
	
	// ����
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL InteEqualize(CDibObject *pDibObject)
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ�����ֱ��ͼ���⡣
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::InteEqualize(CDibObject *pDibObject)
{
	//CDibObject����ָ��
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��δָ�� CDibObject ����ָ�뷵��FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//ֻ����8λͼ��
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("Ŀǰֻ֧��8λ�Ҷ�ͼ��Ĵ���");
		return( FALSE );
	}
	
	//�������
	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, x, y;

	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//pBuffer: ���λͼ����ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );

	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];

	// ����ֱ��ͼ����
	int *pHistogram = GetHistogram();

	// �Ҷ�ӳ���
	BYTE bMap[256];

	int i, j;

	for(i = 0; i < 256; i++)
	{
		bMap[i] = 0;
	}
	
	// ����Ҷ�ӳ���
	for (i = 0; i < 256; i++)
	{
		// ��ʼΪ0
		long lTemp = 0;
		
		for (j = 0; j <= i ; j++)
		{
			lTemp += pHistogram[j];
		}
		
		// �����Ӧ���»Ҷ�ֵ
		bMap[i] = (BYTE) (lTemp * 255 / nHeight / nWidth);
	}

	delete [] pHistogram;

	for(y = 0; y < nHeight; y++)
	{
		pTemp = pBits;
		pTemp += y * nWidthBytes;	//λͼ������һ����ʼָ��
		for(x = 0; x < nWidth; x++)
		{
			long lpSrc = pTemp[x];
			// �����µĻҶ�ֵ
			pTemp[x] = bMap[lpSrc];			
		}
	}
	
	::GlobalUnlock( m_pDibObject->GetDib() );

	// ����
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL ReplaceColorPal()
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ����ָ����α��ɫ�������
//			�滻ͼ��ĵ��԰塣
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL
//			BYTE * bpColorsTable	- α��ɫ�����
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::ReplaceColorPal(CDibObject *pDibObject, 
										   BYTE *bpColorsTable)
{
	//CDibObject����ָ��
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��δָ�� CDibObject ����ָ�뷵��FALSE
	if( m_pDibObject == NULL ) return( FALSE );
	
	//�������
	unsigned char *pBuffer;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	//pBuffer: ���λͼ����ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );

	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ

	// ��ȡα��ɫ���룬����DIB��ɫ��
	for(int i=0; i<256; i++ )
	{
		// ����DIB��ɫ���ɫ����
		pPalette[i].rgbRed = bpColorsTable[i * 4];
		// ����DIB��ɫ����ɫ����
		pPalette[i].rgbGreen = bpColorsTable[i * 4 + 1];
		// ����DIB��ɫ����ɫ����
		pPalette[i].rgbBlue	= bpColorsTable[i * 4 + 2];
		// ����DIB��ɫ�屣��λ
		pPalette[i].rgbReserved	= 0;
	}
	::GlobalUnlock( m_pDibObject->GetDib() );
	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//unsigned char *PointSlopeLinerTran(float fa, float fb)
//----------------------------------------------------------------------
//�������ܣ������������б���Ա任�ĻҶ�ӳ�������һ����������������б
//			���Ա任����ʹ�á�
//----------------------------------------------------------------------
//����˵����float fa			���Ա任��б��
//			float fb			���Ա任�Ľؾ�
//----------------------------------------------------------------------
//���أ�unsigned char *�͵ĻҶ�ӳ���
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
unsigned char *CPointPro::PointSlopeLinerTran(float fa,
														 float fb)
{
	// �Ҷ�ӳ���
	unsigned char *pMap = new unsigned char[256];
	
	// ����Ҷ�ӳ���
	for (int i = 0; i <= 255; i++)
	{
		float fTemp = fa * (float)(i) + fb;
		// �ж��Ƿ񳬳���Χ
		if (fTemp > 255)
		{
			// ֱ�Ӹ�ֵΪ255
			pMap[i] = 255;
		}
		else if (fTemp < 0)
		{
			// ֱ�Ӹ�ֵΪ0
			pMap[i] = 0;
		}
		else
		{
			// ��������
			pMap[i] = (unsigned char) (fTemp + 0.5);
		}
	}
	return(pMap);
}

////////////////////////////////////////////////////////////////////////
//unsigned char *DoublePointLinerTran()
//----------------------------------------------------------------------
//�������ܣ������������б���Ա任�ĻҶ�ӳ�������һ����������������б
//			���Ա任����ʹ�á�
//----------------------------------------------------------------------
//����˵����BYTE bX1	�Ҷ������һ�����x����
//			BYTE bY1	�Ҷ������һ�����y����
//			BYTE bX2	�Ҷ�����ڶ������x����
//			BYTE bY2	�Ҷ�����ڶ������y����
//----------------------------------------------------------------------
//���أ�unsigned char *�͵ĻҶ�ӳ���
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
unsigned char * CPointPro::DoublePointLinerTran(BYTE bX1,
														   BYTE bY1,
														   BYTE bX2,
														   BYTE bY2)
{
	// �Ҷ�ӳ���
	unsigned char *pMap = new unsigned char[256];

	// ����Ҷ�ӳ���
	for (int i = 0; i <= bX1; i++)
	{
		// �ж�bX1�Ƿ����0����ֹ��ĸΪ0��
		if (bX1 > 0)
		{
			// ���Ա任
			pMap[i] = (BYTE) bY1 * i / bX1;
		}
		else
		{
			// ֱ�Ӹ�ֵΪ0
			pMap[i] = 0;
		}
	}
	
	for (int i = 0; i <= bX2; i++)
	{
		// �ж�bX1�Ƿ����bX2����ֹ��ĸΪ0��
		if (bX2 != bX1)
		{
			// ���Ա任
			pMap[i] = bY1 + (BYTE) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// ֱ�Ӹ�ֵΪbY1
			pMap[i] = bY1;
		}
	}

	for (int i = 0; i < 256; i++)
	{
		// �ж�bX2�Ƿ����255����ֹ��ĸΪ0��
		if (bX2 != 255)
		{
			// ���Ա任
			pMap[i] = bY2 + (BYTE) ((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// ֱ�Ӹ�ֵΪ255
			pMap[i] = 255;
		}
	}
	return(pMap);
}

////////////////////////////////////////////////////////////////////////
//BOOL IsBlackandWhite256(CDibObject *pDibObject)
//----------------------------------------------------------------------
//�������ܣ��������жϴ����CDibObject�����е�ͼ���Ƿ�Ϊ256����ֵͼ��
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::IsBlackandWhite256(CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//����8λͼ�񣬱ز���256���Ҷ�ͼ�񣬲�������FALSE
	if( !IsGray256() ) return( FALSE );

	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( -1 );

	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];
	//��ȡ�Ҷ�ͼ��Ŀ�Ⱥ͸߶ȣ������ر�ʾ��
	int nWidth = pBIH->biWidth;
	int nHeight = pBIH->biHeight;

	for(int y = 0; y < nHeight; y++)
	{
		pTemp = pBits;
		pTemp += y * nWidthBytes;	//λͼ������һ����ʼָ��
		for(int x = 0; x < nWidth; x++)
		{
			if( pTemp[x] != 0 && pTemp[x] != 255) return(FALSE);			
		}
	}

	return(TRUE);
}


////////////////////////////////////////////////////////////////////////
//int *GetHistogram()   
//----------------------------------------------------------------------
//�������ܣ���������ô����CDibObject������ͼ���ֱ��ͼ��������д˵�
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
//			CDibObject *pDibObject, Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ�ֱ��ͼ����ָ��*pBuffer���У�
//			nHistogramBuffer[]		�洢����ֱ��ͼ����
//			nHistogramBuffer[256]	�洢��ɫֱ��ͼ����
//			nHistogramBuffer[512]	�洢��ɫֱ��ͼ����
//			nHistogramBuffer[768]	�洢��ֱ��ͼ����
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int *CPointPro::GetHistogram( int nX1, 
									int nY1,
									int nX2,
									int nY2,
									CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//���������
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( NULL );

	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD)];

	//����ֱ��ͼ����
	int *nHistogramBuffer = CreateHistogram( nX1, nY1, nX2, nY2, pBits, pPalette, nWidthBytes );

	::GlobalUnlock( m_pDibObject->GetDib() );

	return( nHistogramBuffer );

}

////////////////////////////////////////////////////////////////////////
//BOOL SplitChannels()   
//----------------------------------------------------------------------
//�������ܣ��������������CDibObject�����е�ͼ�������ɺ졢�̡�������
//			���ɵ�256ɫ�Ҷ�ͼ��
//----------------------------------------------------------------------
//����˵����CDibObject *pRed		ָ���ɫͨ����ͼ��ָ��
//			CDibObject *pGreen	ָ����ɫͨ����ͼ��ָ��
//			CDibObject *pBlue		ָ����ɫͨ����ͼ��ָ��
//----------------------------------------------------------------------
//��    �أ��ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::SplitChannels(CDibObject *pRed, 
							  CDibObject *pGreen, 
							  CDibObject *pBlue)
{
	//û�д�����Ӧָ���޷���ɲ���
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("û�д���ԭʼͼ���޷���ɲ�����");
		return FALSE;
	}

	if(pRed == NULL)
	{
		AfxMessageBox("û�д����ɫͨ��ָ�룬�޷���ɲ�����");
		return FALSE;
	}

	if(pGreen == NULL)
	{
		AfxMessageBox("û�д�����ɫͨ��ָ�룬�޷���ɲ�����");
		return FALSE;
	}

	if(pBlue == NULL)
	{
		AfxMessageBox("û�д�����ɫͨ��ָ�룬�޷���ɲ�����");
		return FALSE;
	}

	//����256ɫ�Ҷ�ͼ������ת��
	if(IsGray256())
	{
		AfxMessageBox("����256ɫ�Ҷ�ͼ�񣬲��ܽ���ͨ�����룡");
		return FALSE;
	}

	//��ɫͨ��
	MakeGray256(1, pRed);

	//��ɫͨ��
	MakeGray256(2, pGreen);
	
	//��ɫͨ��
	MakeGray256(3, pBlue);

	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL GetMixedChannel()   
//----------------------------------------------------------------------
//�������ܣ���������ô����CDibObject�����е�ͼ���еĻ�Ϸ���ͨ����
//----------------------------------------------------------------------
//����˵����CDibObject *pMixed	ָ����ͨ����ͼ��ָ��
//----------------------------------------------------------------------
//��    �أ��ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::GetMixedChannel(CDibObject *pMixed)
{
	//û�д�����Ӧָ���޷���ɲ���
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("û�д���ԭʼͼ���޷���ɲ�����");
		return FALSE;
	}

	if(pMixed == NULL)
	{
		AfxMessageBox("û�д���ͨ��ָ�룬�޷���ɲ�����");
		return FALSE;
	}

	//����256ɫ�Ҷ�ͼ������ת��
	if(IsGray256())
	{
		AfxMessageBox("����256ɫ�Ҷ�ͼ�񣬲��ܽ���ͨ�����룡");
		return FALSE;
	}

	MakeGray256(0, pMixed);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL GetRedChannel()   
//----------------------------------------------------------------------
//�������ܣ���������ô����CDibObject�����е�ͼ���еĺ�ɫ����ͨ����
//----------------------------------------------------------------------
//����˵����CDibObject *pRed	ָ���ɫͨ����ͼ��ָ��
//----------------------------------------------------------------------
//��    �أ��ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::GetRedChannel(CDibObject *pRed)
{
	//û�д�����Ӧָ���޷���ɲ���
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("û�д���ԭʼͼ���޷���ɲ�����");
		return FALSE;
	}

	if(pRed == NULL)
	{
		AfxMessageBox("û�д����ɫͨ��ָ�룬�޷���ɲ�����");
		return FALSE;
	}

	//����256ɫ�Ҷ�ͼ������ת��
	if(IsGray256())
	{
		AfxMessageBox("����256ɫ�Ҷ�ͼ�񣬲��ܽ���ͨ�����룡");
		return FALSE;
	}

	//��ɫͨ��
	MakeGray256(1, pRed);
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL GetGreenChannel()   
//----------------------------------------------------------------------
//�������ܣ���������ô����CDibObject�����е�ͼ���е���ɫ����ͨ����
//----------------------------------------------------------------------
//����˵����CDibObject *pGreen	ָ����ͨ����ͼ��ָ��
//----------------------------------------------------------------------
//��    �أ��ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::GetGreenChannel(CDibObject *pGreen)
{
	//û�д�����Ӧָ���޷���ɲ���
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("û�д���ԭʼͼ���޷���ɲ�����");
		return FALSE;
	}

	if(pGreen == NULL)
	{
		AfxMessageBox("û�д���ͨ��ָ�룬�޷���ɲ�����");
		return FALSE;
	}

	//����256ɫ�Ҷ�ͼ������ת��
	if(IsGray256())
	{
		AfxMessageBox("����256ɫ�Ҷ�ͼ�񣬲��ܽ���ͨ�����룡");
		return FALSE;
	}

	MakeGray256(2, pGreen);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL GetBlueChannel()   
//----------------------------------------------------------------------
//�������ܣ���������ô����CDibObject�����е�ͼ���е���ɫ����ͨ����
//----------------------------------------------------------------------
//����˵����CDibObject *pBlue	ָ����ɫͨ����ͼ��ָ��
//----------------------------------------------------------------------
//��    �أ��ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::GetBlueChannel(CDibObject *pBlue)
{
	//û�д�����Ӧָ���޷���ɲ���
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("û�д���ԭʼͼ���޷���ɲ�����");
		return FALSE;
	}

	if(pBlue == NULL)
	{
		AfxMessageBox("û�д���ͨ��ָ�룬�޷���ɲ�����");
		return FALSE;
	}

	//����256ɫ�Ҷ�ͼ������ת��
	if(IsGray256())
	{
		AfxMessageBox("����256ɫ�Ҷ�ͼ�񣬲��ܽ���ͨ�����룡");
		return FALSE;
	}

	MakeGray256(3, pBlue);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL CombineChannels()   
//----------------------------------------------------------------------
//�������ܣ���������������ɺ졢�̡����������ɵ�256ɫ�Ҷ�ͼ��ϳ�Ϊ��ɫͼ
//			��
//----------------------------------------------------------------------
//����˵����CDibObject *pRed		ָ���ɫͨ����ͼ��ָ��
//			CDibObject *pGreen	ָ����ɫͨ����ͼ��ָ��
//			CDibObject *pBlue		ָ����ɫͨ����ͼ��ָ��
//----------------------------------------------------------------------
//��    �أ��ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPointPro::CombineChannels(CDibObject *pRed, 
								CDibObject *pGreen, 
								CDibObject *pBlue)
{
	//û�д�����Ӧָ���޷���ɲ���
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("û�д��봴����ͼ���޷���ɲ�����");
		return FALSE;
	}

	if(pRed == NULL)
	{
		AfxMessageBox("û�д����ɫͨ��ָ�룬�޷���ɲ�����");
		return FALSE;
	}

	if(pGreen == NULL)
	{
		AfxMessageBox("û�д�����ɫͨ��ָ�룬�޷���ɲ�����");
		return FALSE;
	}

	if(pBlue == NULL)
	{
		AfxMessageBox("û�д�����ɫͨ��ָ�룬�޷���ɲ�����");
		return FALSE;
	}

	//�����ͼ�񲻷�����Ҫ�ĸ�ʽ���޷���ɲ���
	if(pRed->GetNumBits() != 8)
	{
		AfxMessageBox("����ĺ�ɫͨ������8λͼ���޷���ɲ�����");
		return FALSE;
	}

	if(pGreen->GetNumBits() != 8)
	{
		AfxMessageBox("�������ɫͨ������8λͼ���޷���ɲ�����");
		return FALSE;
	}

	if(pBlue->GetNumBits() != 8)
	{
		AfxMessageBox("�������ɫͨ������8λͼ���޷���ɲ�����");
		return FALSE;
	}

	//������ͼ����24λͼ����ת��Ϊ24λͼ��
	if(m_pDibObject->GetNumBits() != 24)
	{
		m_pDibObject->ChangeFormat(24);
	}

	//��ø���ͼ��Ĵ�С
	int nCombineWidth, nRedWidth, nGreenWidth, nBlueWidth;
	int nCombineHeight, nRedHeight, nGreenHeight, nBlueHeight;
	
	//�ϲ���ͼ��Ĵ�С
	nCombineWidth = m_pDibObject->GetWidth();
	nCombineHeight = m_pDibObject->GetHeight();

	//��ɫͨ��ͼ���С
	nRedWidth = pRed->GetWidth();
	nRedHeight = pRed->GetHeight();

	//��ɫͨ��ͼ���С
	nGreenWidth = pGreen->GetWidth();
	nGreenHeight = pGreen->GetHeight();

	//��ɫͨ��ͼ���С
	nBlueWidth = pBlue->GetWidth();
	nBlueHeight = pBlue->GetHeight();

	CRect rectCombine(0, 0, nCombineWidth, nCombineHeight);
	CRect rectRed(0, 0, nRedWidth, nRedHeight);
	CRect rectGreen(0, 0, nGreenWidth, nGreenHeight);
	CRect rectBlue(0, 0, nBlueWidth, nBlueHeight);
	
	//����ͼ��Ĵ�СӦһ��
	if (rectCombine != rectRed 
		|| rectRed != rectGreen 
		|| rectGreen != rectBlue 
		|| rectBlue != rectCombine)
	{
		AfxMessageBox("����ͨ����С��һ�£��޷���ɲ�����");
		return FALSE;
	}

	//��ȡ����ͨ��ԭͼ���ֽڿ��
	int nCombineWidthBytes, nRedWidthBytes, nGreenWidthBytes, nBlueWidthBytes;
	
	//�ϲ���ͼ����ֽڿ��
	char *pCombineBuffer = (char *) m_pDibObject->GetDIBPointer(&nCombineWidthBytes);
	if( pCombineBuffer == NULL ) return( FALSE );
	
	//��ɫͨ����ͼ����ֽڿ��
	char *pRedBuffer = (char *) pRed->GetDIBPointer(&nRedWidthBytes);
	if( pRedBuffer == NULL ) return( FALSE );
	
	//��ɫͨ����ͼ����ֽڿ��
	char *pGreenBuffer = (char *) pGreen->GetDIBPointer(&nGreenWidthBytes);
	if( pGreenBuffer == NULL ) return( FALSE );
	
	//��ɫͨ����ͼ����ֽڿ��
	char *pBlueBuffer = (char *) pBlue->GetDIBPointer(&nBlueWidthBytes);
	if( pBlueBuffer == NULL ) return( FALSE );

	//�������
	BITMAPINFOHEADER *pCombineBIH, *pRedBIH, *pGreenBIH, *pBlueBIH;
	BITMAPFILEHEADER *pCombineBFH, *pRedBFH, *pGreenBFH, *pBlueBFH;
	RGBQUAD *pCombineRGBPalette, *pRedRGBPalette, *pGreenRGBPalette, *pBlueRGBPalette;
	unsigned char *pCombineBits, *pRedBits, *pGreenBits, *pBlueBits;
	unsigned char *pCombineTemp, *pRedTemp, *pGreenTemp, *pBlueTemp;
	int nCombineNumColors, nRedNumColors, nGreenNumColors, nBlueNumColors;

	//�ϲ����ͼ����Ϣ
	//��ȡ�ļ�ͷָ��
	pCombineBFH = (BITMAPFILEHEADER *) pCombineBuffer;
	//��ȡ��Ϣͷָ��
	pCombineBIH = (BITMAPINFOHEADER *) &pCombineBuffer[sizeof(BITMAPFILEHEADER)];
	//��ȡ��ɫ��ָ��
	pCombineRGBPalette = (RGBQUAD *) &pCombineBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];
	//ͼ����ɫ��
	nCombineNumColors = m_pDibObject->GetNumColors();
	//��ȡԭͼ������ָ��
	pCombineBits = (unsigned char *) &pCombineBuffer[sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+nCombineNumColors*sizeof(RGBQUAD)];

	//��ɫͨ����ͼ����Ϣ
	//��ȡ�ļ�ͷָ��
	pRedBFH = (BITMAPFILEHEADER *) pRedBuffer;
	//��ȡ��Ϣͷָ��
	pRedBIH = (BITMAPINFOHEADER *) &pRedBuffer[sizeof(BITMAPFILEHEADER)];
	//��ȡ��ɫ��ָ��
	pRedRGBPalette = (RGBQUAD *) &pRedBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];
	//ͼ����ɫ��
	nRedNumColors = pRed->GetNumColors();
	//��ȡԭͼ������ָ��
	pRedBits = (unsigned char *) &pRedBuffer[sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+nRedNumColors*sizeof(RGBQUAD)];

	//��ɫͨ����ͼ����Ϣ
	//��ȡ�ļ�ͷָ��
	pGreenBFH = (BITMAPFILEHEADER *) pGreenBuffer;
	//��ȡ��Ϣͷָ��
	pGreenBIH = (BITMAPINFOHEADER *) &pGreenBuffer[sizeof(BITMAPFILEHEADER)];
	//��ȡ��ɫ��ָ��
	pGreenRGBPalette = (RGBQUAD *) &pGreenBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];
	//ͼ����ɫ��
	nGreenNumColors = pGreen->GetNumColors();
	//��ȡԭͼ������ָ��
	pGreenBits = (unsigned char *) &pGreenBuffer[sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+nGreenNumColors*sizeof(RGBQUAD)];

	//��ɫͨ����ͼ����Ϣ
	//��ȡ�ļ�ͷָ��
	pBlueBFH = (BITMAPFILEHEADER *) pBlueBuffer;
	//��ȡ��Ϣͷָ��
	pBlueBIH = (BITMAPINFOHEADER *) &pBlueBuffer[sizeof(BITMAPFILEHEADER)];
	//��ȡ��ɫ��ָ��
	pBlueRGBPalette = (RGBQUAD *) &pBlueBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];
	//ͼ����ɫ��
	nBlueNumColors = pBlue->GetNumColors();
	//��ȡԭͼ������ָ��
	pBlueBits = (unsigned char *) &pBlueBuffer[sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+nBlueNumColors*sizeof(RGBQUAD)];

	//ѭ����������
	int x, y;

	//��λ��
	for(y = 0; y < nCombineHeight; y++ )
	{
		//�ϲ����ͼ������ָ�붨λ����ʼλ��
		pCombineTemp = pCombineBits;
		//�ϲ����ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
		pCombineTemp += y * nCombineWidthBytes;
		
		//��ɫͨ��ͼ������ָ�붨λ����ʼλ��
		pRedTemp = pRedBits;
		//��ɫͨ��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
		pRedTemp += y * nRedWidthBytes;
		
		//��ɫͨ��ͼ������ָ�붨λ����ʼλ��
		pGreenTemp = pGreenBits;
		//��ɫͨ��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
		pGreenTemp += y * nGreenWidthBytes;
		
		//��ɫͨ��ͼ������ָ�붨λ����ʼλ��
		pBlueTemp = pBlueBits;
		//��ɫͨ��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
		pBlueTemp += y * nBlueWidthBytes;
		
		//CString s, message = "";

		//��λ��
		for(x = 0; x < nCombineWidth; x++)
		{
			//���кϲ�����
			pCombineTemp[2] = *pRedTemp;
			pCombineTemp[1] = *pGreenTemp;
			pCombineTemp[0] = *pBlueTemp;

			//����ͼ������ָ��
			pCombineTemp += 3;
			pRedTemp++;
			pGreenTemp++;
			pBlueTemp++;
		}
	}

	


	return TRUE;
}
