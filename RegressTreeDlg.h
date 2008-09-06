#pragma once


// CRegressTreeDlg dialog


class CDNAStatement;

class CRegressTreeDlg : public CDialog
{
    DECLARE_DYNAMIC(CRegressTreeDlg)

public:
    CRegressTreeDlg(CWnd* pParent = NULL, CDNAStatement* = (CDNAStatement*)0);   // standard constructor
    virtual ~CRegressTreeDlg();

// Dialog Data
    enum { IDD = IDD_REGRESSTREEDLG };
    
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CDNAStatement* DNAStat;
	CTreeCtrl* DNAStat_Tree;
	CEdit* VarEdit;
	CEdit* FuncRes;
	CEdit* FuncPrime;
	
	CEdit* DepthEdit;

	void ExitError(CString ErrMsg);
	virtual BOOL OnInitDialog();
	void ExpandTree(HTREEITEM Branch);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
