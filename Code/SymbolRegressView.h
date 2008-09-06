// SymbolRegressView.h : interface of the CSymbolRegressView class
//


#pragma once


class CSymbolRegressView : public CView
{
protected: // create from serialization only
	CSymbolRegressView();
	DECLARE_DYNCREATE(CSymbolRegressView)

// Attributes
public:
	CSymbolRegressDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CSymbolRegressView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SymbolRegressView.cpp
inline CSymbolRegressDoc* CSymbolRegressView::GetDocument() const
   { return reinterpret_cast<CSymbolRegressDoc*>(m_pDocument); }
#endif

