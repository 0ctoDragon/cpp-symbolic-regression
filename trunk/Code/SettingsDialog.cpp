// SettingsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SymbolRegress.h"
#include "SettingsDialog.h"
#include ".\settingsdialog.h"


// CSettingsDialog dialog

IMPLEMENT_DYNAMIC(CSettingsDialog, CDialog)
CSettingsDialog::CSettingsDialog(CWnd* pParent /*=NULL*/,
				 COUNTER popCount, COUNTER selectionSize, double mutRate, 
				 COUNTER maxdepth, COUNTER maxdepthX, COUNTER mindepthX, COUNTER TDensity)
	: CDialog(CSettingsDialog::IDD, pParent),
	PopCount(popCount),
	SelectionSize(selectionSize),
	MutRate(mutRate),
	Maxdepth(maxdepth),
	MaxdepthX(maxdepthX),
	MindepthX(mindepthX),TreeDensity(TDensity){


}

CSettingsDialog::~CSettingsDialog()
{
}

void CSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingsDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingsDialog message handlers

BOOL CSettingsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	PopCountEdit = (CEdit*) GetDlgItem(IDC_POPCOUNT);
	SelectionSizeEdit = (CEdit*) GetDlgItem(ID_SELSIZE);
	MutRateEdit = (CEdit*) GetDlgItem(IDC_MUTRA);

	MaxdepthEdit = (CEdit*) GetDlgItem(IDC_MAXDEPTH);
	MaxdepthXEdit = (CEdit*) GetDlgItem(IDC_MAXDEPTHX);
	MindepthXEdit = (CEdit*) GetDlgItem(IDC_MINDEPTHX);
	TreeDensityEdit = (CEdit*) GetDlgItem(IDC_TDENSITY);

	CString temp;
	
	temp.Format(_T("%d"), PopCount);
	PopCountEdit->SetWindowText(temp);

	temp.Format(_T("%d"), SelectionSize);
	SelectionSizeEdit->SetWindowText(temp);
    
	temp.Format(_T("%f"), MutRate);
	MutRateEdit->SetWindowText(temp);

	temp.Format(_T("%d"), Maxdepth);
	MaxdepthEdit->SetWindowText(temp);
    
	temp.Format(_T("%d"), MaxdepthX);
	MaxdepthXEdit->SetWindowText(temp);
    
	temp.Format(_T("%d"), MindepthX);
	MindepthXEdit->SetWindowText(temp);

	temp.Format(_T("%d"), TreeDensity);
	TreeDensityEdit->SetWindowText(temp);
	return TRUE; 
}

void CSettingsDialog::OnBnClickedOk()
{
	  
	CString t;
	stringstream trad1;
	    
	PopCountEdit->GetWindowText(t);
	trad1<<(LPCTSTR) t;
	trad1>>PopCount;

	trad1.clear();
	SelectionSizeEdit->GetWindowText(t);
	trad1<<(LPCTSTR) t;
	trad1>>SelectionSize; 

	trad1.clear();
	MutRateEdit->GetWindowText(t);
	trad1<<(LPCTSTR) t;
	trad1>>MutRate;
    
	trad1.clear();
	MaxdepthEdit->GetWindowText(t);
	trad1<<(LPCTSTR) t;
	trad1>>Maxdepth;

	trad1.clear();
	MaxdepthXEdit->GetWindowText(t);
	trad1<<(LPCTSTR) t;
	trad1>>MaxdepthX;

	trad1.clear();
	MindepthXEdit->GetWindowText(t);
	trad1<<(LPCTSTR) t;
	trad1>>MindepthX;

	trad1.clear();
	TreeDensityEdit->GetWindowText(t);
	trad1<<(LPCTSTR) t;
	trad1>>TreeDensity;


	CDialog::OnOK();
}
