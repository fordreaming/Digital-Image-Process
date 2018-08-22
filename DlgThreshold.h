#if !defined(AFX_DLGTHRESHOLD_H__EBF83EB2_2348_4937_A9C6_01CD47B591AB__INCLUDED_)
#define AFX_DLGTHRESHOLD_H__EBF83EB2_2348_4937_A9C6_01CD47B591AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgThreshold.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgThreshold dialog

class CDlgThreshold : public CDialog
{
// Construction
public:
	//ֱ��ͼ����ָ��
	int *m_pnHistogram;
	
	// ��ǰ����϶�״̬��	0��ʾδ�϶���
	//						1��ʾ�����϶�����
	//						2��ʾ�����϶����ޡ�
	int		m_iIsDraging;
	BOOL	m_bIsInRect;
	
	// ��Ӧ����¼��ľ�������
	CRect	m_MouseRect;

	int m_nThresholdAutoAdjust;

	int m_nThresholdDiscriminant;

	CDlgThreshold(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgThreshold)
	enum { IDD = IDD_DLG_THRESHOLD };
	CEdit	m_ctrlCurEdit;
	CStatic	m_ctrlCurStatic;
	CSpinButtonCtrl	m_ctrlCurrentSpin;
	CSpinButtonCtrl	m_ctrlUpGraySpin;
	CSpinButtonCtrl	m_ctrlLowGraySpin;
	UINT	m_nCurrentThreshold;
	int		m_iLowGray;
	int		m_iUpGray;
	int		m_nThreshold;
	int		m_nThresholdType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgThreshold)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgThreshold)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnThreshMethod3();
	afx_msg void OnThreshMethod1();
	afx_msg void OnThreshMethod2();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChangeEDITLowGray();
	afx_msg void OnChangeEDITUpGray();
	afx_msg void OnChangeEditCurTreshold();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bIsMethoed3;
	BOOL m_bRMouseDown;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTHRESHOLD_H__EBF83EB2_2348_4937_A9C6_01CD47B591AB__INCLUDED_)
