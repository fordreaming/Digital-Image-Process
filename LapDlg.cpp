// LapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dip.h"
#include "LapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLapDlg dialog


CLapDlg::CLapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLapDlg)
	m_iTempMX = 0;
	m_iTempMY = 0;
	m_iTempH = 0;
	m_fTempC = 0;
	m_iTempW = 0;
	m_intType = -1;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLapDlg)
	DDX_Control(pDX, IDC_EDIT_TEMPW_SPIN, m_ctrlTempWSpin);
	DDX_Control(pDX, IDC_EDIT_TEMPH_SPIN, m_ctrlTempHSpin);
	DDX_Control(pDX, IDC_EDIT_TEMPC_SPIN, m_ctrlTempCSpin);
	DDX_Control(pDX, IDC_EDIT_MY_SPIN, m_ctrlMYSpin);
	DDX_Control(pDX, IDC_EDIT_MX_SPIN, m_ctrlMXSpin);
	DDX_Text(pDX, IDC_EDIT_MX, m_iTempMX);
	DDX_Text(pDX, IDC_EDIT_MY, m_iTempMY);
	DDX_Text(pDX, IDC_EDIT_TEMPH, m_iTempH);
	DDX_Text(pDX, IDC_EDIT_TEMPC, m_fTempC);
	DDX_Text(pDX, IDC_EDIT_TEMPW, m_iTempW);
	DDX_Radio(pDX, IDC_RAD1, m_intType);
	DDX_Text(pDX, IDC_EDIT_V0, m_fpArray[0]);
	DDX_Text(pDX, IDC_EDIT_V1, m_fpArray[1]);
	DDX_Text(pDX, IDC_EDIT_V2, m_fpArray[2]);
	DDX_Text(pDX, IDC_EDIT_V3, m_fpArray[3]);
	DDX_Text(pDX, IDC_EDIT_V4, m_fpArray[4]);
	DDX_Text(pDX, IDC_EDIT_V5, m_fpArray[5]);
	DDX_Text(pDX, IDC_EDIT_V6, m_fpArray[6]);
	DDX_Text(pDX, IDC_EDIT_V7, m_fpArray[7]);
	DDX_Text(pDX, IDC_EDIT_V8, m_fpArray[8]);
	DDX_Text(pDX, IDC_EDIT_V9, m_fpArray[9]);
	DDX_Text(pDX, IDC_EDIT_V10, m_fpArray[10]);
	DDX_Text(pDX, IDC_EDIT_V11, m_fpArray[11]);
	DDX_Text(pDX, IDC_EDIT_V12, m_fpArray[12]);
	DDX_Text(pDX, IDC_EDIT_V13, m_fpArray[13]);
	DDX_Text(pDX, IDC_EDIT_V14, m_fpArray[14]);
	DDX_Text(pDX, IDC_EDIT_V15, m_fpArray[15]);
	DDX_Text(pDX, IDC_EDIT_V16, m_fpArray[16]);
	DDX_Text(pDX, IDC_EDIT_V17, m_fpArray[17]);
	DDX_Text(pDX, IDC_EDIT_V18, m_fpArray[18]);
	DDX_Text(pDX, IDC_EDIT_V19, m_fpArray[19]);
	DDX_Text(pDX, IDC_EDIT_V20, m_fpArray[20]);
	DDX_Text(pDX, IDC_EDIT_V21, m_fpArray[21]);
	DDX_Text(pDX, IDC_EDIT_V22, m_fpArray[22]);
	DDX_Text(pDX, IDC_EDIT_V23, m_fpArray[23]);
	DDX_Text(pDX, IDC_EDIT_V24, m_fpArray[24]);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLapDlg, CDialog)
	//{{AFX_MSG_MAP(CLapDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPH, OnKillfocusEditTemph)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPW, OnKillfocusEditTempw)
	ON_BN_CLICKED(IDC_RAD1, OnRad1)
	ON_BN_CLICKED(IDC_RAD2, OnRad2)
	ON_BN_CLICKED(IDC_RAD3, OnRad3)
	ON_BN_CLICKED(IDC_RAD4, OnRad4)
	ON_BN_CLICKED(IDC_RAD5, OnRad5)
	ON_BN_CLICKED(IDC_RAD6, OnRad6)
	ON_BN_CLICKED(IDC_RAD7, OnRad7)
	ON_BN_CLICKED(IDC_RAD8, OnRad8)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLapDlg message handlers


void CLapDlg::UpdateEdit()
{
	BOOL	bEnable;
	
	// ѭ������
	int		i;
	int		j;
	
	// �ж��ǲ����Զ���ģ��
	if (m_intType == 7) 
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
	(CEdit *) GetDlgItem(IDC_EDIT_TEMPC)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_TEMPC_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_MX)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_MX_SPIN)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_MY)->EnableWindow(bEnable);
	(CSpinButtonCtrl *) GetDlgItem(IDC_EDIT_MY_SPIN)->EnableWindow(bEnable);
	
	// IDC_EDIT_V0��ID��ʵ��һ��������������ֵ������Resource.h�ж��塣
	
	// ����ģ��Ԫ���ı���Enable״̬
	for (i = IDC_EDIT_V0; i <= IDC_EDIT_V24; i++)
	{
		// �����ı��򲻿ɱ༭
		(CEdit *) GetDlgItem(i)->EnableWindow(bEnable);
	}
	
	// ��ʾӦ�ÿɼ���ģ��Ԫ���ı���
	for (i = 0; i < m_iTempH; i++)
	{
		for (j = 0; j < m_iTempW; j++)
		{
			// �����ı���ɼ�
			(CEdit *) GetDlgItem(IDC_EDIT_V0 + i*5 + j)->ShowWindow(SW_SHOW);
		}
	}
	
	// ����Ӧ�ò��ɼ���ģ��Ԫ���ı���ǰm_iTempH�еĺ��У�
	for (i = 0; i < m_iTempH; i++)
	{
		for (j = m_iTempW; j < 5; j++)
		{
			// ���ò��ɼ�
			(CEdit *) GetDlgItem(IDC_EDIT_V0 + i*5 + j)->ShowWindow(SW_HIDE);
		}
	}
	
	// ����Ӧ�ò��ɼ���ģ��Ԫ���ı��򣨺��У�
	for (i = m_iTempH; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			// ���ò��ɼ�
			(CEdit *) GetDlgItem(IDC_EDIT_V0 + i*5 + j)->ShowWindow(SW_HIDE);
		}
	}

}

void CLapDlg::OnOK() 
{
	// ��ȡ�û����ã����£�
	UpdateData(TRUE);
	
	// �ж������Ƿ���Ч
	if ((m_iTempMX < 0) || (m_iTempMX > m_iTempW - 1) ||
		(m_iTempMY < 0) || (m_iTempMY > m_iTempH - 1))
	{
		// ��ʾ�û��������ô���
		MessageBox("����Ԫ�ز������ô���", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ����ģ��Ԫ�����飨����ЧԪ�ط����������ǰ�棩
	for (int i = 0; i < m_iTempH; i++)
	{
		for (int j = 0; j < m_iTempW; j++)
		{
			m_fpArray[i * m_iTempW + j] = m_fpArray[i * 5 + j];
		}
	}
	
	// ����
	UpdateData(FALSE);
	
	// �˳�
	CDialog::OnOK();
}

void CLapDlg::OnKillfocusEditTemph() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
}

void CLapDlg::OnKillfocusEditTempw() 
{
	// ����
	UpdateData(TRUE);
	
	// �����ı���״̬
	UpdateEdit();
}

void CLapDlg::OnRad1() 
{
	// 3��3��ͨ�˲���1ģ��
	m_intType = 0;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC  = 1.0;
	
	// ����ģ��Ԫ��
	m_fpArray[0]  = -1.0;
	m_fpArray[1]  = -1.0;
	m_fpArray[2]  = -1.0;
	m_fpArray[3]  =  0.0;
	m_fpArray[4]  =  0.0;
	m_fpArray[5]  = -1.0;
	m_fpArray[6]  =  9.0;
	m_fpArray[7]  = -1.0;
	m_fpArray[8]  =  0.0;
	m_fpArray[9]  =  0.0;
	m_fpArray[10] = -1.0;
	m_fpArray[11] = -1.0;
	m_fpArray[12] = -1.0;
	m_fpArray[13] =  0.0;
	m_fpArray[14] =  0.0;
	m_fpArray[15] =  0.0;
	m_fpArray[16] =  0.0;
	m_fpArray[17] =  0.0;
	m_fpArray[18] =  0.0;
	m_fpArray[19] =  0.0;
	m_fpArray[20] =  0.0;
	m_fpArray[21] =  0.0;
	m_fpArray[22] =  0.0;
	m_fpArray[23] =  0.0;
	m_fpArray[24] =  0.0;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CLapDlg::OnRad2() 
{
	// 3��3��ͨ�˲���2ģ��
	m_intType = 1;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC  = 1.0;
	
	// ����ģ��Ԫ��
	m_fpArray[0]  =  0.0;
	m_fpArray[1]  = -1.0;
	m_fpArray[2]  =  0.0;
	m_fpArray[3]  =  0.0;
	m_fpArray[4]  =  0.0;
	m_fpArray[5]  = -1.0;
	m_fpArray[6]  =  5.0;
	m_fpArray[7]  = -1.0;
	m_fpArray[8]  =  0.0;
	m_fpArray[9]  =  0.0;
	m_fpArray[10] =  0.0;
	m_fpArray[11] = -1.0;
	m_fpArray[12] =  0.0;
	m_fpArray[13] =  0.0;
	m_fpArray[14] =  0.0;
	m_fpArray[15] =  0.0;
	m_fpArray[16] =  0.0;
	m_fpArray[17] =  0.0;
	m_fpArray[18] =  0.0;
	m_fpArray[19] =  0.0;
	m_fpArray[20] =  0.0;
	m_fpArray[21] =  0.0;
	m_fpArray[22] =  0.0;
	m_fpArray[23] =  0.0;
	m_fpArray[24] =  0.0;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CLapDlg::OnRad3() 
{
	// 3��3��ͨ�˲���3ģ��
	m_intType = 2;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC  = 1.0;
	
	// ����ģ��Ԫ��
	m_fpArray[0]  =  1.0;
	m_fpArray[1]  = -2.0;
	m_fpArray[2]  =  1.0;
	m_fpArray[3]  =  0.0;
	m_fpArray[4]  =  0.0;
	m_fpArray[5]  = -2.0;
	m_fpArray[6]  =  5.0;
	m_fpArray[7]  = -2.0;
	m_fpArray[8]  =  0.0;
	m_fpArray[9]  =  0.0;
	m_fpArray[10] =  1.0;
	m_fpArray[11] = -2.0;
	m_fpArray[12] =  1.0;
	m_fpArray[13] =  0.0;
	m_fpArray[14] =  0.0;
	m_fpArray[15] =  0.0;
	m_fpArray[16] =  0.0;
	m_fpArray[17] =  0.0;
	m_fpArray[18] =  0.0;
	m_fpArray[19] =  0.0;
	m_fpArray[20] =  0.0;
	m_fpArray[21] =  0.0;
	m_fpArray[22] =  0.0;
	m_fpArray[23] =  0.0;
	m_fpArray[24] =  0.0;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CLapDlg::OnRad4() 
{
	// 3��3Laplacian1ģ��
	m_intType = 3;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC  = 1.0;
	
	// ����ģ��Ԫ��
	m_fpArray[0]  =  1.0;
	m_fpArray[1]  =  1.0;
	m_fpArray[2]  =  1.0;
	m_fpArray[3]  =  0.0;
	m_fpArray[4]  =  0.0;
	m_fpArray[5]  =  1.0;
	m_fpArray[6]  = -8.0;
	m_fpArray[7]  =  1.0;
	m_fpArray[8]  =  0.0;
	m_fpArray[9]  =  0.0;
	m_fpArray[10] =  1.0;
	m_fpArray[11] =  1.0;
	m_fpArray[12] =  1.0;
	m_fpArray[13] =  0.0;
	m_fpArray[14] =  0.0;
	m_fpArray[15] =  0.0;
	m_fpArray[16] =  0.0;
	m_fpArray[17] =  0.0;
	m_fpArray[18] =  0.0;
	m_fpArray[19] =  0.0;
	m_fpArray[20] =  0.0;
	m_fpArray[21] =  0.0;
	m_fpArray[22] =  0.0;
	m_fpArray[23] =  0.0;
	m_fpArray[24] =  0.0;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CLapDlg::OnRad5() 
{
	// 3��3Laplacian2ģ��
	m_intType = 4;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC  = 1.0;
	
	// ����ģ��Ԫ��
	m_fpArray[0]  =  0.0;
	m_fpArray[1]  =  1.0;
	m_fpArray[2]  =  0.0;
	m_fpArray[3]  =  0.0;
	m_fpArray[4]  =  0.0;
	m_fpArray[5]  =  1.0;
	m_fpArray[6]  = -4.0;
	m_fpArray[7]  =  1.0;
	m_fpArray[8]  =  0.0;
	m_fpArray[9]  =  0.0;
	m_fpArray[10] =  0.0;
	m_fpArray[11] =  1.0;
	m_fpArray[12] =  0.0;
	m_fpArray[13] =  0.0;
	m_fpArray[14] =  0.0;
	m_fpArray[15] =  0.0;
	m_fpArray[16] =  0.0;
	m_fpArray[17] =  0.0;
	m_fpArray[18] =  0.0;
	m_fpArray[19] =  0.0;
	m_fpArray[20] =  0.0;
	m_fpArray[21] =  0.0;
	m_fpArray[22] =  0.0;
	m_fpArray[23] =  0.0;
	m_fpArray[24] =  0.0;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CLapDlg::OnRad6() 
{
	// 3��3Laplacian3ģ��
	m_intType = 5;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC  = 1.0;
	
	// ����ģ��Ԫ��
	m_fpArray[0]  = -1.0;
	m_fpArray[1]  =  2.0;
	m_fpArray[2]  = -1.0;
	m_fpArray[3]  =  0.0;
	m_fpArray[4]  =  0.0;
	m_fpArray[5]  =  2.0;
	m_fpArray[6]  = -4.0;
	m_fpArray[7]  =  2.0;
	m_fpArray[8]  =  0.0;
	m_fpArray[9]  =  0.0;
	m_fpArray[10] = -1.0;
	m_fpArray[11] =  2.0;
	m_fpArray[12] = -1.0;
	m_fpArray[13] =  0.0;
	m_fpArray[14] =  0.0;
	m_fpArray[15] =  0.0;
	m_fpArray[16] =  0.0;
	m_fpArray[17] =  0.0;
	m_fpArray[18] =  0.0;
	m_fpArray[19] =  0.0;
	m_fpArray[20] =  0.0;
	m_fpArray[21] =  0.0;
	m_fpArray[22] =  0.0;
	m_fpArray[23] =  0.0;
	m_fpArray[24] =  0.0;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CLapDlg::OnRad7() 
{
	// 5��5 Gaussian-Laplacian ģ��
	m_intType = 6;
	m_iTempH  = 5;
	m_iTempW  = 5;
	m_iTempMX = 2;
	m_iTempMY = 2;
	m_fTempC  = 1.0;
	
	// ����ģ��Ԫ��
	m_fpArray[0]  = -2.0;
	m_fpArray[1]  = -4.0;
	m_fpArray[2]  = -4.0;
	m_fpArray[3]  = -4.0;
	m_fpArray[4]  = -2.0;
	m_fpArray[5]  = -4.0;
	m_fpArray[6]  =  0.0;
	m_fpArray[7]  =  8.0;
	m_fpArray[8]  =  0.0;
	m_fpArray[9]  = -4.0;
	m_fpArray[10] = -4.0;
	m_fpArray[11] =  8.0;
	m_fpArray[12] =  24.0;
	m_fpArray[13] =  8.0;
	m_fpArray[14] = -4.0;
	m_fpArray[15] = -4.0;
	m_fpArray[16] =  0.0;
	m_fpArray[17] =  8.0;
	m_fpArray[18] =  0.0;
	m_fpArray[19] = -4.0;
	m_fpArray[20] = -2.0;
	m_fpArray[21] = -4.0;
	m_fpArray[22] = -4.0;
	m_fpArray[23] = -4.0;
	m_fpArray[24] = -2.0;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CLapDlg::OnRad8() 
{
	// �Զ���ģ��
	m_intType = 7;
	
	// �����ı���״̬
	UpdateEdit();
}

BOOL CLapDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//����΢���ؼ��ķ�Χ�ͻ��༭��
	m_ctrlTempCSpin.SetRange(1, 25);
	m_ctrlTempCSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_TEMPC));

	m_ctrlTempWSpin.SetRange(0, 5);
	m_ctrlTempWSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_TEMPW));
	
	m_ctrlTempHSpin.SetRange(0, 5);
	m_ctrlTempHSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_TEMPH));
	
	m_ctrlMXSpin.SetRange(0, 2);
	m_ctrlMXSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_MX));

	m_ctrlMYSpin.SetRange(0, 2);
	m_ctrlMYSpin.SetBuddy( (CWnd* )GetDlgItem(IDC_EDIT_MY));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
