
// TrojanServerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "TrojanServer.h"
#include "TrojanServerDlg.h"
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


// CTrojanServerDlg dialog



CTrojanServerDlg::CTrojanServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TROJANSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTrojanServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTrojanServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_SERVER, OnSock)
END_MESSAGE_MAP()


// CTrojanServerDlg message handlers

BOOL CTrojanServerDlg::OnInitDialog()
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
	WSADATA wsaData;
	int wsaSu = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaSu) {
		MessageBox(TEXT("WSAStart Launch Failed!ERROR:" + wsaSu));
	}
	m_ListenSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//WSAEventSelect(m_ListenSock, GetSafeHwnd(), UM_SERVER | FD_READ | FD_CONNECT | FD_CLOSE);
	WSAAsyncSelect(m_ListenSock, GetSafeHwnd(), UM_SERVER, FD_ACCEPT | FD_READ | FD_CONNECT | FD_CLOSE);

	struct in_addr sAdd;
	inet_pton(PF_INET, "127.0.0.1", &sAdd);
	sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_addr.S_un.S_addr = sAdd.S_un.S_addr;
	addr.sin_port = htons(5555);

	if (bind(m_ListenSock, (SOCKADDR*)& addr, sizeof(addr))) {
		MessageBox(TEXT("bind Failed!ERROR:"));
	}
	if (listen(m_ListenSock, 1)) {
		MessageBox(TEXT("listen Failed!ERROR:"));
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTrojanServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTrojanServerDlg::OnPaint()
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
HCURSOR CTrojanServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CTrojanServerDlg::OnSock(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		return -1;
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
	{
		sockaddr_in clientAddr;
		int nSize = sizeof(SOCKADDR);
		m_ClientSock = accept(m_ListenSock, (SOCKADDR*)& clientAddr, &nSize);


		char strAddr[MAXBYTE];		inet_ntop(PF_INET, (SOCKADDR*)& clientAddr, strAddr, MAXBYTE);		wchar_t* pwszUnicode;		int iSize = MultiByteToWideChar(CP_ACP, 0, strAddr, sizeof(strAddr), NULL, 0); //iSize =wcslen(pwsUnicode)+1=6		pwszUnicode = (wchar_t*)malloc(iSize * sizeof(wchar_t)); //����Ҫ pwszUnicode = (wchar_t *)malloc((iSize+1)*sizeof(wchar_t))		MultiByteToWideChar(CP_ACP, 0, strAddr, sizeof(strAddr), pwszUnicode, iSize);		m_StrMsg.Format(L"Request Address:%s:%d", pwszUnicode, ntohs(clientAddr.sin_port));		delete pwszUnicode;
		/*char strAddr[MAXBYTE];
		inet_ntop(PF_INET, (SOCKADDR*)& clientAddr, strAddr, MAXBYTE);
		wchar_t* dbuf;
		size_t sSize = strlen(strAddr);
		size_t dSize = (size_t)MultiByteToWideChar(CP_ACP, 0, (const char*)strAddr, (int)sSize, NULL, 0);
		dbuf = (wchar_t*)malloc(dSize * sizeof(wchar_t));
		WCHAR tmp = MultiByteToWideChar(CP_ACP, 0, (const char*)strAddr, (int)sSize, dbuf, dSize);
		m_StrMsg.Format(TEXT("Request Address:%s:%d"), tmp, ntohs(clientAddr.sin_port));*/
		DATA_MSG dataMsg;
		dataMsg.bType = TEXTMSG;
		dataMsg.bClass = 0;
		CString value;
		strcpy_s(dataMsg.szValue, HELPMSG);
		send(m_ClientSock, dataMsg.szValue, sizeof(dataMsg) + sizeof(CHAR), 0);
		break;
	}
	case FD_READ:
	{
		char szBuf[MAXBYTE] = { 0 };
		recv(m_ClientSock, szBuf, MAXBYTE, 0);
		DIspatchMsg(szBuf);
		m_StrMsg = "Received:";
		m_StrMsg += szBuf;
		break;
	}
	case FD_CLOSE:
	{
		closesocket(m_ListenSock);
		m_StrMsg = "Close connection.";
		break;
	}
	default:
		break;
	}
	InsertMsg();
}

void CTrojanServerDlg::InsertMsg()
{
	CString strMsg;
	GetDlgItemText(IDC_EDIT1, strMsg);
	m_StrMsg += "\r\n";
	m_StrMsg += "---------------------------------------------------\r\n";
	m_StrMsg += strMsg;
	SetDlgItemText(IDC_EDIT1, m_StrMsg);
	m_StrMsg = "";
}

VOID CTrojanServerDlg::DIspatchMsg(char* szBuf)
{
	DATA_MSG dataMsg;
	ZeroMemory((void*)& dataMsg, sizeof(DATA_MSG));
	if (!strcmp(szBuf, "help"))
	{
		dataMsg.bType = TEXTMSG;
		dataMsg.bClass = 0;
		strcpy_s(dataMsg.szValue, HELPMSG);
	}
	return VOID();
}

