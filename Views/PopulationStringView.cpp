// PopulationStringView.cpp : implementation file
//

#include "stdafx.h"
#include "SymbolRegress.h"
#include "FitnessClass.h"
#include "DNAStatement.h"
#include "PopulationStringView.h"
#include "SymbolRegressDoc.h"
#include ".\populationstringview.h"

// CPopulationStringView

IMPLEMENT_DYNCREATE(CPopulationStringView, CListView)

CPopulationStringView::CPopulationStringView()
{
}

CPopulationStringView::~CPopulationStringView()
{
}

BEGIN_MESSAGE_MAP(CPopulationStringView, CListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
END_MESSAGE_MAP()


// CPopulationStringView diagnostics

#ifdef _DEBUG
void CPopulationStringView::AssertValid() const
{
	CListView::AssertValid();
}

void CPopulationStringView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CSymbolRegressDoc* CPopulationStringView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSymbolRegressDoc)));
	return (CSymbolRegressDoc*)m_pDocument;
}
#endif 

/**********************
Methods
**********************/
void CPopulationStringView::UpdateList(){

	CString strText;
	ListCtrl->DeleteAllItems();


	for(COUNTER i=0; i<DocPtr->m_Population.size(); i++){ 

		ListCtrl->InsertItem(
		LVIF_TEXT|LVIF_STATE, i, strText, 
		/*(i%2)==0 ? LVIS_SELECTED : */0, LVIS_SELECTED,
		0, 0);
		
		strText.Format(TEXT("%d"), i);
		ListCtrl->SetItemText(i, 0,strText);
		if(DocPtr->m_Population[i]){
			strText.Format(TEXT("%f"), DocPtr->m_Population[i]->getFitness());
			ListCtrl->SetItemText(i, 1, strText);
			ListCtrl->SetItemText(i, 2, DocPtr->m_Population[i]->toString());
		}
		else{
			ListCtrl->SetItemText(i, 1, _T("----"));
			ListCtrl->SetItemText(i, 2, _T("----"));
		}
	}

	if(DocPtr->m_CurrentIndividual > -1)
		ListCtrl->SetSelectionMark(DocPtr->m_CurrentIndividual);
	
}



/****************************
Messages and Overrides
****************************/

int CPopulationStringView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//ListCtrl = &GetListCtrl( );
	ASSERT(GetStyle() & LVS_REPORT);

	// Gain a reference to the list control itself
	ListCtrl = &GetListCtrl();
	ListCtrl->InsertColumn(0, _T("Individual ID"), LVCFMT_LEFT,75,0);
	ListCtrl->InsertColumn(1, _T("Fitness"), LVCFMT_LEFT,75,2);
	ListCtrl->InsertColumn(2, _T("Individual"), LVCFMT_LEFT,3000,1);

	DocPtr = (CSymbolRegressDoc*) this->GetDocument();
	return 0;
}

void CPopulationStringView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult){
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CPopulationStringView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult){

	int nSelected = this->ListCtrl->GetSelectedCount();
	// only proceed if one item selected....
	if ( nSelected == 1 )  { 
		DocPtr->m_CurrentIndividual = this->ListCtrl->GetSelectionMark();
		DocPtr->UpdateGraph();
		//this->ListCtrl->SetSelectionMark(DocPtr->m_CurrentIndividual);
	}
	
	*pResult = 0;
}

BOOL CPopulationStringView::PreCreateWindow(CREATESTRUCT& cs){
	
	cs.style = (cs.style & ~LVS_TYPEMASK) | LVS_REPORT;
	cs.style |= LVS_AUTOARRANGE;
	
	return CListView::PreCreateWindow(cs);
}

void CPopulationStringView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/){
	UpdateList();	
}
