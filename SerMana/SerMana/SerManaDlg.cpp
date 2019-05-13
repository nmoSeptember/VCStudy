
// SerManaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerMana.h"
#include "SerManaDlg.h"
#include "afxdialogex.h"
#include <winsvc.h>

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


// CSerManaDlg dialog



CSerManaDlg::CSerManaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERMANA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerManaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ServiceType);
	DDX_Control(pDX, IDC_RADIO2, m_Win32Btn);
}

BEGIN_MESSAGE_MAP(CSerManaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO2, &CSerManaDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CSerManaDlg::OnBnClickedRadio1)
END_MESSAGE_MAP()


// CSerManaDlg message handlers

BOOL CSerManaDlg::OnInitDialog()
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

	m_ServiceType.InsertColumn(0, TEXT("Service Name"), LVCFMT_LEFT, 120);
	m_ServiceType.InsertColumn(1, TEXT("Display Name"), LVCFMT_LEFT, 220);
	m_ServiceType.InsertColumn(2, TEXT("Status"), LVCFMT_LEFT, 80);
	m_Win32Btn.SetCheck(1);

	ShowServiceList(SERVICE_WIN32);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerManaDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSerManaDlg::OnPaint()
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
HCURSOR CSerManaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSerManaDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	ShowServiceList(SERVICE_WIN32);
}


void CSerManaDlg::OnBnClickedRadio1()
{
	ShowServiceList(SERVICE_DRIVER);
	// TODO: Add your control notification handler code here
}

VOID CSerManaDlg::ShowServiceList(DWORD dwServiceType)
{
	m_ServiceType.DeleteAllItems();

	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCM)
	{
		AfxMessageBox(TEXT("OpenSCManger Error!"));
		return;
	}
	DWORD serviceCount = 0;
	DWORD dwSize = 0;
	LPENUM_SERVICE_STATUS lpInfo;

	BOOL bRet = EnumServicesStatus(hSCM, dwServiceType, SERVICE_STATE_ALL, NULL, 0, &dwSize, &serviceCount, NULL);


	if (!bRet&& GetLastError() == ERROR_MORE_DATA)
	{
		lpInfo = (LPENUM_SERVICE_STATUS)(new BYTE[dwSize]);
		bRet = EnumServicesStatus(hSCM, dwServiceType, SERVICE_STATE_ALL, (LPENUM_SERVICE_STATUS)lpInfo, dwSize, &dwSize, &serviceCount, NULL);
		if (!bRet)
		{
			CloseServiceHandle(hSCM);
			return;
		}
		for (DWORD i = 0; i < serviceCount; i++)
		{
			CString str;
			m_ServiceType.InsertItem(i, lpInfo[i].lpServiceName);
			m_ServiceType.SetItemText(i, 1, lpInfo[i].lpDisplayName);
			switch (lpInfo[0].ServiceStatus.dwCurrentState)
			{
			case SERVICE_PAUSED:
			{
				m_ServiceType.SetItemText(i, 2, _T("Paused"));
				break;
			}
			case SERVICE_STOPPED:
			{
				m_ServiceType.SetItemText(i, 2, _T("Stop"));
				break;
			}
			case SERVICE_RUNNING:
			{
				m_ServiceType.SetItemText(i, 2, _T("Run"));
				break;
			}
			default:
			{
				m_ServiceType.SetItemText(i, 2, _T("Other"));
				break;
			}
			}
		}
		delete lpInfo;
	}
	CloseServiceHandle(hSCM);
}
