// SymbolRegressView.cpp : implementation of the CSymbolRegressView class
//

#include "stdafx.h"
#include "SymbolRegress.h"

#include "SymbolRegressDoc.h"
#include "SymbolRegressView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSymbolRegressView

IMPLEMENT_DYNCREATE(CSymbolRegressView, CView)

BEGIN_MESSAGE_MAP(CSymbolRegressView, CView)
END_MESSAGE_MAP()

// CSymbolRegressView construction/destruction

CSymbolRegressView::CSymbolRegressView()
{
	// TODO: add construction code here

}

CSymbolRegressView::~CSymbolRegressView()
{
}

BOOL CSymbolRegressView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSymbolRegressView drawing

void CSymbolRegressView::OnDraw(CDC* /*pDC*/)
{
	CSymbolRegressDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSymbolRegressView diagnostics

#ifdef _DEBUG
void CSymbolRegressView::AssertValid() const
{
	CView::AssertValid();
}

void CSymbolRegressView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSymbolRegressDoc* CSymbolRegressView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSymbolRegressDoc)));
	return (CSymbolRegressDoc*)m_pDocument;
}
#endif //_DEBUG


// CSymbolRegressView message handlers
