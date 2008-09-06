// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "SymbolRegress.h"

#include "ChildFrm.h"
#include ".\childfrm.h"

#include "GraphView.h"
#include "PopulationStringView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	BOOL bRet;
	
	if(!m_wndSplitter.CreateStatic(this, 2, 1))
		return false;

	bRet = m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CPopulationStringView), CSize(60,200), pContext);
	if(!bRet) return false;

	bRet = m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(GraphView), CSize(60,200), pContext);
	if(!bRet) return false;

	return true;
		//CMDIChildWnd::OnCreateClient(lpcs, pContext);
}













