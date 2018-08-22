// DlgMedianFilter.cpp : implementation file
//

#include "stdafx.h"
#include "dip.h"
#include "DlgMedianFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMedianFilter dialog


CDlgMedianFilter::CDlgMedianFilter(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMedianFilter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMedianFilter)
	m_iTempH = 0;
	m_iTempW = 0;
	m_iTempMX = 0;
	m_iTempMY = 0;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMedianFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMedianFilter)
	DDX_Control(pDX, IDC_EDIT_TEMPW_SPIN, m_ctrlTempWSpin);
	DDX_Control(pDX, IDC_EDIT_TEMPH_SPIN, m_ctrlTempHSpin);
	DDX_Control(pDX, IDC_EDIT_MY_SPIN, m_ctrlMYSpin);
	DDX_Control(pDX, IDC_EDIT_MX_SPIN, m_ctrlMXSpin);
	DDX_Radio(pDX, IDC_RAD1, m_nWindowSize);
	DDX_Text(pDX, IDC_EDIT_TEMPH, m_iTempH);
	DDX_Text(pDX, IDC_EDIT_TEMPW, m_iTempW);
	DDX_Text(pDX, IDC_EDIT_MX, m_iTempMX);
	DDX_Text(pDX, IDC_EDIT_MY, m_iTempMY);
	DDX_Radio(pDX, IDC_SORT1, m_nSortType);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMedianFilter, CDialog)
	//{{AFX_MSG_MAP(CDlgMedianFilter)
	ON_BN_CLICKED(IDC_RAD1, OnRad1)
	ON_BN_CLICKED(IDC_RAD2, OnRad2)
	ON_BN_CLICKED(IDC_RAD3, OnRad3)
	ON_BN_CLICKED(IDC_RAD4, OnRad4)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPH, OnKillfocusEditTemph)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPW, OnKillfocusEditTempw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMedianFilter message handlers

BOOL CDlgMedianFilter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//����΢���ؼ��ķ�Χ�ͻ��༭��
	m_ctrlTempWSpin.SetRange(1, 99);
	m_ctrlTempWSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_TEMPW));
	
	m_ctrlTempHSpin.SetRange(1, 99);
	m_ctrlTempHSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_TEMPH));
	
	m_ctrlMXSpin.SetRange(0, 99);
	m_ctrlMXSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_MX));

	m_ctrlMYSpin.SetRange(0, 99);
	m_ctrlMYSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_MY));

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMedianFilter::UpdateEdit()
{
	BOOL	bEnable;
	
	// �ж��ǲ����Զ���ģ��
	if (m_nWindowSize == 3) 
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	// �����ı�����΢���ؼ���״̬
	(CEdit *) GetDlgItem(IDC_EDIT_TEMPH)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_TEMPH_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_TEMPW)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_TEMPW_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_MX)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_MX_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_MY)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_MY_SPIN)->EnableWindow(bEnable);
	
}

void CDlgMedianFilter::OnRad1() 
{
	// 3��3����
	m_nWindowSize = 0;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CDlgMedianFilter::OnRad2() 
{
	// 5��5����
	m_nWindowSize = 1;
	m_iTempH  = 5;
	m_iTempW  = 5;
	m_iTempMX = 2;
	m_iTempMY = 2;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CDlgMedianFilter::OnRad3() 
{
	// 7��7����
	m_nWindowSize = 2;
	m_iTempH  = 7;
	m_iTempW  = 7;
	m_iTempMX = 3;
	m_iTempMY = 3;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CDlgMedianFilter::OnRad4() 
{
	// �Զ��崰��
	m_nWindowSize = 3;
	
	// �����ı���״̬
	UpdateEdit();
}

void CDlgMedianFilter::OnOK() 
{
	// ��ȡ�û����ã����£�
	UpdateData(TRUE);
	
	// �ж������Ƿ���Ч
	if ((m_iTempMX < 0) || (m_iTempMX > m_iTempW - 1) ||
		(m_iTempMY < 0) || (m_iTempMY > m_iTempH - 1))
	{
		// ��ʾ�û��������ô���
		MessageBox("����Ԫ�ز������ô���", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}

	BOOL bError = FALSE;
	
	if (m_iTempW % 2 == 0)
	{
		// ��ʾ�û��������ô���
		MessageBox("���ڿ�ȱ���Ϊ������", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// ����
		bError = TRUE;
	}

	if (m_iTempH % 2 == 0)
	{
		// ��ʾ�û��������ô���
		MessageBox("���ڸ߶ȱ���Ϊ������", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// ����
		bError = TRUE;
	}

	if(bError) return;

	// ����
	UpdateData(FALSE);

	CDialog::OnOK();
}

void CDlgMedianFilter::OnKillfocusEditTemph() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
}

void CDlgMedianFilter::OnKillfocusEditTempw() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
}
