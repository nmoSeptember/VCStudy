
// TrojanServerDlg.h : header file
//

#pragma once

#define HELPMSG "help-Show Help Menu\r\n"\
"getsysinfo-Get System Information\r\n"\
"open-Open The CDRom\r\n"\
"close-Close The CDRom\r\n"\
"swap-Swap Mouse Button\r\n"\
"restore-Resore Mouse Button\r\n"\
"exit-Quit BdShell\r\n"\

#define UM_SERVER (WM_USER+200)
#define TEXTMSG 't'
#define BINARYMSG 'b'

typedef struct _DATA_MSG
{
	BYTE bType;
	BYTE bClass;
	char szValue[0x200];
}DATA_MSG, * PDATA_MSG;

// CTrojanServerDlg dialog
class CTrojanServerDlg : public CDialogEx
{

	// Construction
public:
	CTrojanServerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TROJANSERVER_DIALOG };
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
	afx_msg LRESULT OnSock(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void InsertMsg();
	VOID DIspatchMsg(char* szBuf);
	SOCKET m_ListenSock;
	SOCKET m_ClientSock;
	CString m_StrMsg;
	HANDLE m_hServerEvent;
};
