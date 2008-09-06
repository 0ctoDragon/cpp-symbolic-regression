#pragma once


// CSettingsDialog dialog

class CSettingsDialog : public CDialog
{
	DECLARE_DYNAMIC(CSettingsDialog)

public:
	CSettingsDialog(CWnd* pParent = NULL,
	COUNTER FuncNum = 3,
	COUNTER=50, COUNTER=50, double=0.0, COUNTER=0, COUNTER=0, COUNTER=0,COUNTER=0);   // standard constructor
	virtual ~CSettingsDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

	COUNTER	PopCount;
	COUNTER	SelectionSize;
	double	MutRate;
	COUNTER	Maxdepth;
	COUNTER	MaxdepthX;
	COUNTER	MindepthX;
	COUNTER	TreeDensity;
	COUNTER	m_FuncNum;
protected:
	CEdit* PopCountEdit;
	CEdit* SelectionSizeEdit;
	CEdit* MutRateEdit;
	CEdit* MaxdepthEdit;
	CEdit* MaxdepthXEdit;
	CEdit* MindepthXEdit;
	CEdit* TreeDensityEdit;


	CListBox* FuncListings;



	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLbnSelchangeFunctionlist();
};
