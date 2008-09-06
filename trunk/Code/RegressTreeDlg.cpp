// RegressTreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SymbolRegress.h"
#include "RegressTreeDlg.h"
#include "DNAStatement.h"
#include ".\regresstreedlg.h"

// CRegressTreeDlg dialog

IMPLEMENT_DYNAMIC(CRegressTreeDlg, CDialog)
CRegressTreeDlg::CRegressTreeDlg(CWnd* pParent /*=NULL*/, CDNAStatement* D)
    : CDialog(CRegressTreeDlg::IDD, pParent), DNAStat(D)
{
    if(!DNAStat)
        ExitError(_T("No Tree Provided To Dialog Box"));
}



CRegressTreeDlg::~CRegressTreeDlg()
{
}

void CRegressTreeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegressTreeDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CRegressTreeDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	DNAStat_Tree = (CTreeCtrl *)GetDlgItem(IDC_TREE1); 
	
	DepthEdit = (CEdit *)GetDlgItem(IDC_DEPTH);
	CString d;
	d.Format("%d", DNAStat->getDepth());
	DepthEdit->SetWindowText(d);

	try{ 
		DNAStat->toTreeCtrl(DNAStat_Tree, DNAStat_Tree->GetRootItem());
	}
	catch(CString ErrMsg){
		CString Messg2(_T("--> Unable to display CDNAStatement in TreeForm at CRegressTreeDlg::OnInitDialog()"));
		Messg2 = ErrMsg + Messg2;
		ExitError(Messg2);
	}
	    
	ExpandTree(DNAStat_Tree->GetRootItem());

    
	VarEdit	= (CEdit*)GetDlgItem(IDC_X1EDIT); 
	FuncRes	= (CEdit*)GetDlgItem(IDC_FXEDID);
	FuncPrime 	= (CEdit*)GetDlgItem(IDC_PRIMEFX);

	VarEdit->SetWindowText(CString(_T("UNDEF")));
	FuncRes->SetWindowText(CString(_T("UNDEF")));
	FuncPrime->SetWindowText(CString(_T("UNDEF")));

	 return TRUE;  
}

void CRegressTreeDlg::ExpandTree(HTREEITEM Branch){ 
        DNAStat_Tree->Expand(Branch, TVE_EXPAND);
        if(DNAStat_Tree->ItemHasChildren(Branch)){
                HTREEITEM Kid =  DNAStat_Tree->GetNextItem(Branch, TVGN_CHILD);
                while(Kid){
                    ExpandTree(Kid);
                    Kid = DNAStat_Tree->GetNextItem(Kid, TVGN_NEXT);
                }    
        }
}

void CRegressTreeDlg::ExitError(CString ErrMsg){
    AfxMessageBox(ErrMsg);
    CDialog::OnCancel();
} 

void CRegressTreeDlg::OnBnClickedButton1(){
	try{
		CString StringVar;
		VarEdit->GetWindowText(StringVar);
		stringstream trad1;
		trad1<<(LPCTSTR) StringVar;
		double Val;
		trad1>>Val;	
		F<double> x = Val;
		x.diff(0,1);

		F<double> Res = DNAStat->Eval(x);
		

		StringVar.Format("%f", Res.x());
		FuncRes->SetWindowText(StringVar);

		StringVar.Format("%f", Res.d(0));
		FuncPrime->SetWindowText(StringVar);

	}
	catch(CString Err){
		AfxMessageBox(Err);
	}


}
