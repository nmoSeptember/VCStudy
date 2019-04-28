
// TrojanClientDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "TrojanClient.h"
#include "TrojanClientDlg.h"
#include "afxdialogex.h"
#include <WS2tcpip.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTrojanClientDlg dialog



CTrojanClientDlg::CTrojanClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TROJANCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTrojanClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTrojanClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_CLIENT, OnSock)
	ON_BN_CLICKED(IDC_BUTTON1, &CTrojanClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTrojanClientDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CTrojanClientDlg message handlers

BOOL CTrojanClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTrojanClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTrojanClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTrojanClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTrojanClientDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	char szBtnName[10] = { 0 };
	GetDlgItemText(IDC_BUTTON1, (LPTSTR)szBtnName, 10);
	if (!strcmp(szBtnName, "Disconn"))
	{
		SetDlgItemText(IDC_EDIT1, L"Conn");
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		closesocket(m_Socket);
		m_StrMsg = "Active disConnect";
		InsertMsg();
		return;
	}

	char szLpAddr[MAXBYTE] = { 0 };
	struct in_addr sAdd;
	GetDlgItemText(IDC_EDIT1, (LPTSTR)szLpAddr, MAXBYTE);
	m_Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	WSAEventSelect(m_Socket, GetSafeHwnd(), UM_CLIENT | FD_READ | FD_CONNECT | FD_CLOSE);
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	inet_pton(PF_INET, szLpAddr, &sAdd);
	serverAddr.sin_addr.S_un.S_addr = sAdd.S_un.S_addr;
	serverAddr.sin_port = htons(5555);
	connect(m_Socket, (SOCKADDR*)& serverAddr, sizeof(SOCKADDR));
}

void CTrojanClientDlg::InsertMsg()
{
}

LRESULT CTrojanClientDlg::OnSock(WPARAM wParm, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		return NULL;
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
	{
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);

		SetDlgItemText(IDC_BUTTON1, TEXT("Disconn"));
		m_StrMsg = "Connect successful!";
		break;
	}
	case FD_READ:
	{
		DATA_MSG dataMsg;
		recv(m_Socket, (char*)& dataMsg, sizeof(DATA_MSG), 0);
		DispatchMsg((char*)dataMsg.szValue);
		break;
	}
	default:
		break;
	}
	InsertMsg();
	return LRESULT();
}

VOID CTrojanClientDlg::DispatchMsg(char* msg)
{
	DATA_MSG dataMsg;
	memcpy(&dataMsg, (const void*)msg, sizeof(DATA_MSG));
	if (dataMsg.bType == TEXTMSG)
	{
		m_StrMsg = dataMsg.szValue;
	}
	return VOID();
}




void CTrojanClientDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	char szBuf[MAXBYTE] = { 0 };
	GetDlgItemText(IDC_EDIT3, (LPTSTR)szBuf, MAXBYTE);
	send(m_Socket, szBuf, MAXBYTE, 0);
}
