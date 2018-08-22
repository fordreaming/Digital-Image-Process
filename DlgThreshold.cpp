// DlgThreshold.cpp : implementation file
//

#include "stdafx.h"
#include "Dip.h"
#include "DlgThreshold.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgThreshold dialog


CDlgThreshold::CDlgThreshold(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgThreshold::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgThreshold)
	m_nCurrentThreshold = 0;
	m_iLowGray = 0;
	m_iUpGray = 255;
	m_nThreshold = 0;
	m_nThresholdType = 0;
	//}}AFX_DATA_INIT
	m_pnHistogram = NULL;
	m_nThresholdAutoAdjust = 0;
	m_nThresholdDiscriminant = 0;
	m_bIsMethoed3 = FALSE;
	m_bRMouseDown = FALSE;
}


void CDlgThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgThreshold)
	DDX_Control(pDX, IDC_EDIT_CUR_TRESHOLD, m_ctrlCurEdit);
	DDX_Control(pDX, IDC_STATIC_CURTHRESH, m_ctrlCurStatic);
	DDX_Control(pDX, IDC_EDIT_CUR_TRESHSPIN, m_ctrlCurrentSpin);
	DDX_Control(pDX, IDC_EDIT_UPSPIN, m_ctrlUpGraySpin);
	DDX_Control(pDX, IDC_EDIT_LOWSPIN, m_ctrlLowGraySpin);
	DDX_Text(pDX, IDC_EDIT_CUR_TRESHOLD, m_nCurrentThreshold);
	DDV_MinMaxUInt(pDX, m_nCurrentThreshold, 0, 255);
	DDX_Text(pDX, IDC_EDIT_LowGray, m_iLowGray);
	DDV_MinMaxInt(pDX, m_iLowGray, 0, 255);
	DDX_Text(pDX, IDC_EDIT_UpGray, m_iUpGray);
	DDX_Radio(pDX, IDC_THRESH_METHOD1, m_nThreshold);
	DDX_Radio(pDX, IDC_TRESHOLD_WHITE, m_nThresholdType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgThreshold, CDialog)
	//{{AFX_MSG_MAP(CDlgThreshold)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_THRESH_METHOD3, OnThreshMethod3)
	ON_BN_CLICKED(IDC_THRESH_METHOD1, OnThreshMethod1)
	ON_BN_CLICKED(IDC_THRESH_METHOD2, OnThreshMethod2)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_EN_CHANGE(IDC_EDIT_LowGray, OnChangeEDITLowGray)
	ON_EN_CHANGE(IDC_EDIT_UpGray, OnChangeEDITUpGray)
	ON_EN_CHANGE(IDC_EDIT_CUR_TRESHOLD, OnChangeEditCurTreshold)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgThreshold message handlers

BOOL CDlgThreshold::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
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
	m_ctrlCurrentSpin.SetRange(0,255);
	m_ctrlCurrentSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_CUR_TRESHOLD));

	m_ctrlLowGraySpin.SetRange(0,255);
	m_ctrlLowGraySpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_LowGray));
	
	m_ctrlUpGraySpin.SetRange(0,255);
	m_ctrlUpGraySpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_UpGray));

	m_ctrlCurEdit.SetReadOnly(TRUE);
	m_ctrlCurrentSpin.EnableWindow(FALSE);
	m_ctrlCurStatic.EnableWindow(FALSE);

	// ����TRUE
	return TRUE;
}

void CDlgThreshold::OnPaint() 
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
	CPen* pPenBrow = new CPen;
	
	// ��ɫ����
	pPenBrow->CreatePen(PS_DOT,1,RGB(128,64,64));

	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
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
		if ( m_pnHistogram[ i ] > lMaxCount )
		{
			// �������ֵ
			lMaxCount = m_pnHistogram[ i ];
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
	
	// ���Ļ���
	pDC->SelectObject(pPenBlue);	
	
	// �ж��Ƿ��м���
	if (lMaxCount > 0)
	{
		// ����ֱ��ͼ
		for (i = m_iLowGray; i <= m_iUpGray; i ++)
		{
			pDC->MoveTo(i + 10, 276);
			pDC->LineTo(i + 10, 277 - (int) (m_pnHistogram[ i ] * 256 / lMaxCount));
		}
	}
	
	// ���ĳɻ�ɫ����
	pDC->SelectObject(pPenGreen);	
	
	// ���ƴ���������
	pDC->MoveTo(m_nCurrentThreshold + 10, 21);
	pDC->LineTo(m_nCurrentThreshold + 10, 276);
	
	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	
	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
	delete pPenBrow;
}

void CDlgThreshold::OnThreshMethod3() 
{
	m_bIsMethoed3 = TRUE;

	// ����䶯���ؼ�����ֵ���浽��Ա�����У�
	UpdateData(TRUE);

	m_ctrlCurEdit.SetReadOnly(FALSE);
	m_ctrlCurrentSpin.EnableWindow(TRUE);
	m_ctrlCurStatic.EnableWindow(TRUE);

	UpdateData(FALSE);

	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgThreshold::OnThreshMethod1() 
{
	m_bIsMethoed3 = FALSE;

	// ����䶯���ؼ�����ֵ���浽��Ա�����У�
	UpdateData(TRUE);

	m_nCurrentThreshold = m_nThresholdAutoAdjust;
	
	// ����䶯����Ա��������ֵ���浽�ؼ��У�
	UpdateData(FALSE);

	m_ctrlCurEdit.SetReadOnly(TRUE);
	m_ctrlCurrentSpin.EnableWindow(FALSE);
	m_ctrlCurStatic.EnableWindow(FALSE);

	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgThreshold::OnThreshMethod2() 
{
	m_bIsMethoed3 = FALSE;

	// ����䶯���ؼ�����ֵ���浽��Ա�����У�
	UpdateData(TRUE);

	m_nCurrentThreshold = m_nThresholdDiscriminant;
	
	// ����䶯����Ա��������ֵ���浽�ؼ��У�
	UpdateData(FALSE);

	m_ctrlCurEdit.SetReadOnly(TRUE);
	m_ctrlCurrentSpin.EnableWindow(FALSE);
	m_ctrlCurStatic.EnableWindow(FALSE);

	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgThreshold::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	// �ж��Ƿ��ڽ�������¼�����Ч������
	if(m_MouseRect.PtInRect(point))
	{
		if(	m_bIsMethoed3 && !m_bRMouseDown )
		{
			::SetCursor(::LoadCursor(NULL,IDC_CROSS));
			m_nCurrentThreshold = point.x-20;
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

void CDlgThreshold::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// ���û�������������ʼ�϶�
	
	// �ж��Ƿ��ڽ�������¼�����Ч������
	if(m_MouseRect.PtInRect(point))
	{
		m_bRMouseDown = FALSE;
		if(	m_bIsMethoed3 && !m_bRMouseDown )
		{
			::SetCursor(::LoadCursor(NULL,IDC_CROSS));
			m_nCurrentThreshold = point.x-20;
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

void CDlgThreshold::OnLButtonUp(UINT nFlags, CPoint point) 
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

void CDlgThreshold::OnChangeEDITLowGray() 
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

void CDlgThreshold::OnChangeEDITUpGray() 
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

void CDlgThreshold::OnChangeEditCurTreshold() 
{
	// ����䶯���ؼ�����ֵ���浽��Ա�����У�
	UpdateData(TRUE);

	// ���£���Ա��������ֵ���浽�ؼ��У�
	UpdateData(FALSE);
	
	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgThreshold::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// �ж��Ƿ��ڽ�������¼�����Ч������
	if(m_MouseRect.PtInRect(point))
	{
		m_bRMouseDown = TRUE;
	}
	CDialog::OnRButtonDown(nFlags, point);
}
