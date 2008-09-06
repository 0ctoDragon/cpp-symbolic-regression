#pragma once


// GraphView view
class CSymbolRegressDoc;
class GraphView : public CView
{
	DECLARE_DYNCREATE(GraphView)


protected:
	HGLRC m_hrc;
	CDC*    m_pDC;
	CSymbolRegressDoc* DocPtr;

	virtual BOOL SetupPixelFormat( void );
	virtual BOOL InitializeOpenGL();
	virtual void RenderScene();

   
	GLfloat WinWidth;
	GLfloat WinHeight;
protected:
	GraphView();           // protected constructor used by dynamic creation
	virtual ~GraphView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	CSymbolRegressDoc* GetDocument() const;//CAssignment2Doc*;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()


public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);




public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in SymbolRegressView.cpp
inline CSymbolRegressDoc* GraphView::GetDocument() const
   { return reinterpret_cast<CSymbolRegressDoc*>(m_pDocument); }
#endif
