// DlgIntensity.cpp : implementation file
//

#include "stdafx.h"
#include "Dip.h"
#include "DlgIntensity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgIntensity dialog


CDlgIntensity::CDlgIntensity(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIntensity::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgIntensity)
	m_iLowGray = 0;
	m_iUpGray = 255;
	m_nCurrentGray = 0;
	m_nCurrentPiexsNum = 0;
	m_nHistogramType = 0;
	//}}AFX_DATA_INIT
	m_pnHistogram = NULL;
	m_nPenType = 0;	//��ɫ����
	m_bIsInRect = FALSE;
	m_nGrayValue = 0;
	m_bRMouseDown = FALSE;
	m_bIsGray256 = FALSE;
}


void CDlgIntensity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgIntensity)
	DDX_Control(pDX, IDC_IDC_EDIT_UPSPIN, m_ctrlUpGraySpin);
	DDX_Control(pDX, IDC_IDC_EDIT_LOWSPIN, m_ctrlLowGraySpin);
	DDX_Control(pDX, IDC_IDC_EDIT_CURSPIN, m_ctrlCurrentGraySpin);
	DDX_Text(pDX, IDC_EDIT_LowGray, m_iLowGray);
	DDV_MinMaxInt(pDX, m_iLowGray, 0, 255);
	DDX_Text(pDX, IDC_EDIT_UpGray, m_iUpGray);
	DDV_MinMaxInt(pDX, m_iUpGray, 0, 255);
	DDX_Text(pDX, IDC_EDIT_CUR_GRAY, m_nCurrentGray);
	DDV_MinMaxUInt(pDX, m_nCurrentGray, 0, 255);
	DDX_Text(pDX, IDC_EDIT_CUR_NUMBER, m_nCurrentPiexsNum);
	DDX_Radio(pDX, IDC_HISTOGRAMTYPE, m_nHistogramType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIntensity, CDialog)
	//{{AFX_MSG_MAP(CDlgIntensity)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_EN_CHANGE(IDC_EDIT_LowGray, OnChangeEDITLowGray)
	ON_EN_CHANGE(IDC_EDIT_UpGray, OnChangeEDITUpGray)
	ON_EN_CHANGE(IDC_EDIT_CUR_GRAY, OnChangeEditCurGray)
	ON_BN_CLICKED(IDC_HISTOGRAMTYPE, OnHistogramtype)
	ON_BN_CLICKED(IDC_GREEN_RADIO, OnGreenRadio)
	ON_BN_CLICKED(IDC_RED_RADIO, OnRedRadio)
	ON_BN_CLICKED(IDC_BLUE_RADIO, OnBlueRadio)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIntensity message handlers

BOOL CDlgIntensity::OnInitDialog() 
{
	
	// ����Ĭ��OnInitDialog����
	CDialog::OnInitDialog();
	
	// ��ȡ����ֱ��ͼ�ı�ǩ
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// �����������¼�����Ч����
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;
	
	// ���ý�������¼�����Ч����
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;
	
	// ��ʼ���϶�״̬
	m_iIsDraging = 0;

	//����΢���ؼ��ķ�Χ�ͻ��༭��
	m_ctrlCurrentGraySpin.SetRange(0,255);
	m_ctrlCurrentGraySpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_CUR_GRAY));

	m_ctrlLowGraySpin.SetRange(0,255);
	m_ctrlLowGraySpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_LowGray));
	
	m_ctrlUpGraySpin.SetRange(0,255);
	m_ctrlUpGraySpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_UpGray));

	if ( m_bIsGray256 )
	{
		GrayTypeRedRadio().EnableWindow(FALSE);
		GrayTypeGreenRadio().EnableWindow(FALSE);
		GrayTypeBlueRadio().EnableWindow(FALSE);
	}

	m_pHistTemp = m_pnHistogram;
	// ����TRUE
	return TRUE;
}

void CDlgIntensity::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// ���û�������������ʼ�϶�
	
	// �ж��Ƿ��ڽ�������¼�����Ч������
	if(m_MouseRect.PtInRect(point))
	{
		m_bRMouseDown = FALSE;
		if(	!m_bRMouseDown )
		{			
			::SetCursor(::LoadCursor(NULL,IDC_CROSS));
			m_nGrayValue = point.x-10;
			m_nCurrentGray = m_nGrayValue - 10;
			m_nCurrentPiexsNum = m_pHistTemp[m_nCurrentGray];
			UpdateData(FALSE);
			InvalidateRect(m_MouseRect, TRUE);
		}
		if (point.x == (m_MouseRect.left + m_iLowGray))
		{
			
			// �����϶�״̬1���϶�����
			m_iIsDraging = 1;
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		else if (point.x == (m_MouseRect.left + m_iUpGray))
		{
			
			// �����϶�״̬Ϊ2���϶�����
			m_iIsDraging = 2;
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	
	// Ĭ�ϵ��������������¼�
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgIntensity::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	// �ж��Ƿ��ڽ�������¼�����Ч������
	if(m_MouseRect.PtInRect(point))
	{
		if(	!m_bRMouseDown )
		{			
			::SetCursor(::LoadCursor(NULL,IDC_CROSS));
			m_nGrayValue = point.x-10;
			m_nCurrentGray = m_nGrayValue - 10;
			m_nCurrentPiexsNum = m_pHistTemp[m_nCurrentGray];
			UpdateData(FALSE);
			InvalidateRect(m_MouseRect, TRUE);
		}
		// �ж��Ƿ������϶�
		if (m_iIsDraging != 0)
		{
			// �ж������϶����޻�������
			if (m_iIsDraging == 1)
			{
				// �ж��Ƿ�����<����
				if (point.x - m_MouseRect.left < m_iUpGray)
				{
					// ��������
					m_iLowGray = point.x - m_MouseRect.left;
				}
				else
				{
					// �����Ϲ����ޣ�����Ϊ����-1
					m_iLowGray = m_iUpGray - 1;
					
					// �������λ��
					point.x = m_MouseRect.left + m_iUpGray - 1;
				}
			}
			else
			{
				// �����϶�����
				
				// �ж��Ƿ�����>����
				if (point.x - m_MouseRect.left > m_iLowGray)
				{
					// ��������
					m_iUpGray = point.x - m_MouseRect.left;
				}
				else
				{
					// �����Ϲ����ޣ�����Ϊ���ޣ�1
					m_iUpGray = m_iLowGray + 1;
					
					// �������λ��
					point.x = m_MouseRect.left + m_iLowGray + 1;
				}
			}
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			
			// ����
			UpdateData(FALSE);
			
			// �ػ�ֱ��ͼ
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_iLowGray) || point.x == (m_MouseRect.left + m_iUpGray))
		{
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	
	// Ĭ������ƶ������¼�
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgIntensity::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// ���û��ͷ�������ֹͣ�϶�
	if (m_iIsDraging != 0)
	{
		// �����϶�״̬
		m_iIsDraging = 0;
	}
	
	// Ĭ���ͷ������������¼�
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgIntensity::OnPaint() 
{
	// �ַ���
	CString str;
	
	// ѭ������
	LONG i;
	
	// ������
	LONG lMaxCount = 0;
	
	// �豸������
	CPaintDC dc(this);
	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// ָ��
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,330,300);
	
	// �������ʶ���
	CPen* pPenRed = new CPen;
	
	// ��ɫ����
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	
	// �������ʶ���
	CPen* pPenBlue = new CPen;
	
	// ��ɫ����
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));
	
	// �������ʶ���
	CPen* pPenGreen = new CPen;
	
	// ��ɫ����
	pPenGreen->CreatePen(PS_SOLID,1,RGB(0,255,0));
	
	// �������ʶ���
	CPen* pPenBlack = new CPen;
	
	// ��ɫ����
	pPenBlack->CreatePen(PS_SOLID,1,RGB(0,0,0));

	// �������ʶ���
	CPen* pPenGray = new CPen;
	
	// ��ɫ����
	pPenGray->CreatePen(PS_SOLID,1,RGB(80,80,80));

	// �������ʶ���
	CPen* pPenBrow = new CPen;
	
	// ��ɫ����
	pPenBrow->CreatePen(PS_DOT,1,RGB(128,64,64));

	// �������ʶ���
	CPen* pPenYellow = new CPen;
	
	// ��ɫ����
	pPenYellow->CreatePen(PS_DOT,1,RGB(255,128,64));


	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenBlack);
	
	// ����������
	pDC->MoveTo(10,6);
	
	// ��ֱ��
	pDC->LineTo(10,276);
	
	// ˮƽ��
	pDC->LineTo(320,276);
	
	// дX��̶�ֵ
	str.Format("0");
	pDC->TextOut(10, 279, str);
	str.Format("50");
	pDC->TextOut(60, 279, str);
	str.Format("100");
	pDC->TextOut(110, 279, str);
	str.Format("150");
	pDC->TextOut(160, 279, str);
	str.Format("200");
	pDC->TextOut(210, 279, str);
	str.Format("255");
	pDC->TextOut(265, 279, str);
	
	// ����X��̶�
	for (i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10�ı���
			pDC->MoveTo(i + 10, 276);
			pDC->LineTo(i + 10, 280);
		}
		else
		{
			// 10�ı���
			pDC->MoveTo(i + 10, 276);
			pDC->LineTo(i + 10, 278);
		}
	}
	
	// ����X���ͷ
	pDC->MoveTo(315,271);
	pDC->LineTo(320,276);
	pDC->LineTo(315,281);
	
	// ����X���ͷ
	pDC->MoveTo(10,6);
	pDC->LineTo(5,11);
	pDC->MoveTo(10,6);
	pDC->LineTo(15,11);
	
	// ����������ֵ
	for (i = m_iLowGray; i <= m_iUpGray; i ++)
	{
		if ( m_pHistTemp[ i ] > lMaxCount )
		{
			// �������ֵ
			lMaxCount = m_pHistTemp[ i ];
		}
	}
	
	// ���������ֵ
	pDC->MoveTo(10, 21);
	pDC->LineTo(14, 21);
	str.Format("%d", lMaxCount);
	pDC->TextOut(11, 22, str);
	
	// ���ĳɺ�ɫ����
	pDC->SelectObject(pPenBrow);	
	
	// ���ƴ���������
	pDC->MoveTo(m_iLowGray + 10, 21);
	pDC->LineTo(m_iLowGray + 10, 276);
	
	pDC->MoveTo(m_iUpGray + 10, 21);
	pDC->LineTo(m_iUpGray + 10, 276);
	
	// ���ĳ���ɫ����
	switch(m_nPenType)
	{
	case 0:
		// ���ĳɺ�ɫ����		
		pDC->SelectObject(pPenGray);
		break;
	case 1:
		// ���ĳɺ�ɫ����		
		pDC->SelectObject(pPenRed);
		break;
	case 2:
		// ���ĳ���ɫ����		
		pDC->SelectObject(pPenGreen);
		break;
	case 3:
		// ���ĳ���ɫ����		
		pDC->SelectObject(pPenBlue);
		break;
	}


	
	// �ж��Ƿ��м���
	if (lMaxCount > 0)
	{
		// ����ֱ��ͼ
		for (i = m_iLowGray; i <= m_iUpGray; i ++)
		{
			pDC->MoveTo(i + 10, 276);
			pDC->LineTo(i + 10, 277 - (int) (m_pHistTemp[ i ] * 256 / lMaxCount));
		}
	}

	// ���ĳɻ�ɫ����
	pDC->SelectObject(pPenYellow);	
	
	// ���Ƶ�ǰ���λ����
	pDC->MoveTo(m_nGrayValue, 21);
	pDC->LineTo(m_nGrayValue, 276);
	
	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	
	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
	delete pPenBlack;
	delete pPenBrow;
	delete pPenYellow;
	delete pPenGray;
	
}

void CDlgIntensity::OnOK() 
{
	
	// �ж��Ƿ����޳�������
	if (m_iLowGray > m_iUpGray)
	{
		// ����
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
	}
	
	// ����
	CDialog::OnOK();
}

void CDlgIntensity::OnChangeEDITLowGray() 
{
	// ����䶯���ؼ�����ֵ���浽��Ա�����У�
	UpdateData(TRUE);
	
	// �ж��Ƿ����޳�������
	if (m_iLowGray > m_iUpGray)
	{
		// ����
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
		
		// ���£���Ա��������ֵ���浽�ؼ��У�
		UpdateData(FALSE);
	}
	
	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);
	
}

void CDlgIntensity::OnChangeEDITUpGray() 
{
	// ����䶯���ؼ�����ֵ���浽��Ա�����У�
	UpdateData(TRUE);
	
	// �ж��Ƿ����޳�������
	if (m_iLowGray > m_iUpGray)
	{
		// ����
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
		
		// ����
		UpdateData(FALSE);
	}
	
	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgIntensity::OnChangeEditCurGray() 
{
	// ����䶯���ؼ�����ֵ���浽��Ա�����У�
	UpdateData(TRUE);
	
	m_nCurrentPiexsNum = m_pHistTemp[m_nCurrentGray];
	// ���£���Ա��������ֵ���浽�ؼ��У�
	UpdateData(FALSE);

	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);	
}

void CDlgIntensity::OnHistogramtype() 
{
	m_pHistTemp = m_pnHistogram;
	// ����䶯���ؼ�����ֵ���浽��Ա�����У�
	UpdateData(TRUE);

	m_nCurrentPiexsNum = m_pHistTemp[m_nCurrentGray];

	UpdateData(FALSE);
	//���ĺ�ɫ����	
	m_nPenType = 0;
	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgIntensity::OnGreenRadio() 
{
	m_pHistTemp = &m_pnHistogram[512];
	UpdateData(TRUE);

	m_nCurrentPiexsNum = m_pHistTemp[m_nCurrentGray];

	UpdateData(FALSE);
	//������ɫ����	
	m_nPenType = 2;
	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgIntensity::OnRedRadio() 
{
	m_pHistTemp = &m_pnHistogram[256];
	UpdateData(TRUE);

	m_nCurrentPiexsNum = m_pHistTemp[m_nCurrentGray];

	UpdateData(FALSE);
	//���ĺ�ɫ����
	m_nPenType = 1;
	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgIntensity::OnBlueRadio() 
{
	m_pHistTemp = &m_pnHistogram[768];
	UpdateData(TRUE);

	m_nCurrentPiexsNum = m_pHistTemp[m_nCurrentGray];

	UpdateData(FALSE);
	//������ɫ����	
	m_nPenType = 3;
	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgIntensity::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// �ж��Ƿ��ڽ�������¼�����Ч������
	if(m_MouseRect.PtInRect(point))
	{
		m_bRMouseDown = TRUE;
	}
	CDialog::OnRButtonDown(nFlags, point);
}
