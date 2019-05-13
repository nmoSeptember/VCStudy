
// SerManaDlg.h : header file
//

#pragma once


// CSerManaDlg dialog
class CSerManaDlg : public CDialogEx
{
	// Construction
public:
	CSerManaDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERMANA_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ServiceType;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	CButton m_Win32Btn;
private:
	VOID ShowServiceList(DWORD dwServiceType);
};
