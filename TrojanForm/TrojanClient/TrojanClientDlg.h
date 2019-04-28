
// TrojanClientDlg.h : header file
//

#pragma once
#define UM_CLIENT (WM_USER+200)
#define TEXTMSG 't'
#define BINARYMSG 'b'

typedef struct _DATA_MSG
{
	BYTE bType;
	BYTE bClass;
	char szValue[0x200];
}DATA_MSG, * PDATA_MSG;

// CTrojanClientDlg dialog
class CTrojanClientDlg : public CDialogEx
{
	// Construction
public:
	CTrojanClientDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TROJANCLIENT_DIALOG };
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
	afx_msg LRESULT OnSock(WPARAM wParm, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
private:
	void InsertMsg();
	VOID DispatchMsg(char* msg);
	SOCKET m_Socket;
	CString m_StrMsg;
public:
	afx_msg void OnBnClickedButton2();
};
