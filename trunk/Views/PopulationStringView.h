#pragma once


// CPopulationStringView view

class CSymbolRegressDoc;
class CPopulationStringView : public CListView
{
	DECLARE_DYNCREATE(CPopulationStringView)

protected:
	CPopulationStringView();           // protected constructor used by dynamic creation
	virtual ~CPopulationStringView();
	CListCtrl* ListCtrl;
	CSymbolRegressDoc*  DocPtr;
public:
	CSymbolRegressDoc* GetDocument() const;
	


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void UpdateList();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // debug version in SymbolRegressView.cpp
inline CSymbolRegressDoc* CPopulationStringView::GetDocument() const
   { return reinterpret_cast<CSymbolRegressDoc*>(m_pDocument); }
#endif
