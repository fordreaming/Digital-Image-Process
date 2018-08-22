// DipView.cpp : implementation of the CDipView class
//

#include "stdafx.h"
#include "Dip.h"

#include "DipDoc.h"
#include "DipView.h"
#include "PointPro.h"
#include "DlgIntensity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDipView

IMPLEMENT_DYNCREATE(CDipView, CScrollView)

BEGIN_MESSAGE_MAP(CDipView, CScrollView)
	//{{AFX_MSG_MAP(CDipView)
	ON_COMMAND(ID_VIEW_HIST, OnViewHist)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HIST, OnUpdateViewHist)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDipView construction/destruction

CDipView::CDipView()
{
	// TODO: add construction code here

}

CDipView::~CDipView()
{
}

BOOL CDipView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDipView drawing

void CDipView::OnDraw(CDC* pDC)
{
	CDipDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if( !pDoc->m_bImageLoaded)
	{
		pDoc->LoadImageToDocument();
	}

	//��������
	CSize sizeTotal;
	sizeTotal.cx = pDoc->m_pDibObject->GetWidth();
	sizeTotal.cy = pDoc->m_pDibObject->GetHeight();
	SetScrollSizes (MM_TEXT, sizeTotal);

	//��ȡ�ͻ����ߴ�
	OnPrepareDC(pDC);
	CRect Rect;
	GetClientRect( &Rect );

	//��ȡͼ���ȼ��߶�
	int nImageWidth, nImageHeight;
	nImageWidth = pDoc->m_pDibObject->GetWidth();
	nImageHeight = pDoc->m_pDibObject->GetHeight();

	//��ͼ��ʵ�ʳߴ�С�ڴ��ڳߴ�ʱ����ͼ����ڿͻ����м�
	int nX, nY;
	if( nImageWidth < Rect.Width() )
		nX = (Rect.Width() - nImageWidth ) / 2;
	else
		nX = 0;
	if( nImageHeight < Rect.Height() )
		nY = (Rect.Height() - nImageHeight ) / 2;
	else
		nY = 0;
	

	if( GetFocus() == this )
		pDoc->m_pDibObject->SetPalette(pDC);

	pDoc->m_pDibObject->Draw(pDC, nX, nY);
	
}

void CDipView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CDipView printing

BOOL CDipView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDipView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDipView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDipView diagnostics

#ifdef _DEBUG
void CDipView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDipView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDipDoc* CDipView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDipDoc)));
	return (CDipDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDipView message handlers

void CDipView::OnViewHist() 
{
	CDipDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	//�жϵ�ǰ�Ƿ���ͼ�����
	if( pDoc->m_pDibObject == NULL ) return;
	
	//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
	CPointPro PointOperation( pDoc->m_pDibObject );

	int *pHistogram = PointOperation.GetHistogram();

	//����һ���Ի���CHistDlg���ʵ��
	CDlgIntensity HistDlg;

	//������ֱ��ͼ�����ݴ��ݸ�CHistDlg�Ի�����Ĺ��г�Ա����m_pnHistogram
	if( pHistogram != NULL )
	{
		//����ֱ��ͼ����ָ��
		HistDlg.m_pnHistogram = pHistogram;
		//���õ�ǰ����ֵΪ0��������
		HistDlg.m_nCurrentPiexsNum = pHistogram[0];
		//�����Ƿ�Ϊ256���Ҷ�ͼ��
		HistDlg.m_bIsGray256 = PointOperation.IsGray256();
	}

	//��ʾ�Ի���
	if ( HistDlg.DoModal() != IDOK)
		return;

	delete [] pHistogram;
}


void CDipView::OnUpdateViewHist(CCmdUI* pCmdUI) 
{
	CDipDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->Enable( pDoc->m_pDibObject->GetNumBits() >= 8);
	
}
