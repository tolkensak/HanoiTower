
#include "stdafx.h"
#include "App.h"
#include "MainDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

class CAboutDlg : public CDialog
{
// Construction
public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHanoiTowerDlg dialog

CHanoiTowerDlg::CHanoiTowerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHanoiTowerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHanoiTowerDlg)
	m_nRing = 5;
	m_nVelocity = 10;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAIN);

	m_bmLampOn.LoadBitmap(IDB_LAMP_ON);
	m_bmLampOff.LoadBitmap(IDB_LAMP_OFF);
	
	m_eState=State_End;
	m_pHanoiTower=NULL;
	srand((UINT)::GetTickCount());
}

void CHanoiTowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHanoiTowerDlg)
	DDX_Text(pDX, IDC_EDT_COUNT, m_nRing);
	DDV_MinMaxInt(pDX, m_nRing, 1, 64);
	DDX_Slider(pDX, IDC_SLD_SPPED, m_nVelocity);
	DDX_Control(pDX, IDC_STA_BOARD, m_stBoard);
	DDX_Control(pDX, IDC_STA_LAMP, m_stLamp);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHanoiTowerDlg, CDialog)
	//{{AFX_MSG_MAP(CHanoiTowerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CONTROL, OnBnBiycik)
	ON_BN_CLICKED(IDC_BTN_MODE, OnBnRing)
	ON_BN_CLICKED(IDC_STA_LAMP, OnStLamp)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHanoiTowerDlg message handlers

BOOL CHanoiTowerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUT & 0xFFF0) == IDM_ABOUT);
	ASSERT(IDM_ABOUT < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strJayli;
		strJayli.LoadString(IDS_ABOUT);
		if (!strJayli.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUT, strJayli);
		}
		pSysMenu->RemoveMenu(SC_SIZE, MF_BYCOMMAND);
		pSysMenu->RemoveMenu(SC_MAXIMIZE, MF_BYCOMMAND);
		pSysMenu->EnableMenuItem(SC_RESTORE, MF_BYCOMMAND | MF_GRAYED);
	}

	SetIcon(m_hIcon, TRUE);		// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CEdit* pspCount=(CEdit*)GetDlgItem(IDC_EDT_COUNT);
	CSpinButtonCtrl* ptmCount=(CSpinButtonCtrl*)GetDlgItem(IDC_SPN_COUNT);
	ptmCount->SetBuddy(pspCount);
	ptmCount->SetRange(1, 64);

	CSliderCtrl* psqVelocity=(CSliderCtrl*)GetDlgItem(IDC_SLD_SPPED);
	psqVelocity->SetRange(0, 20);
	psqVelocity->SetTicFreq(2);
	psqVelocity->SetPageSize(2);
	psqVelocity->SetLineSize(1);

	SetTimer(1, 700, NULL);
	return TRUE;
}

void CHanoiTowerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUT)
		OnStLamp();
	else
	{
		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
			if(nID == SC_MINIMIZE)
			{
				pSysMenu->EnableMenuItem(SC_MINIMIZE, MF_BYCOMMAND | MF_GRAYED);
				pSysMenu->EnableMenuItem(SC_RESTORE, MF_BYCOMMAND | MF_ENABLED);
			}
			else if(nID == SC_RESTORE)
			{
				pSysMenu->EnableMenuItem(SC_RESTORE, MF_BYCOMMAND | MF_GRAYED);
				pSysMenu->EnableMenuItem(SC_MINIMIZE, MF_BYCOMMAND | MF_ENABLED);
			}
		}

		CDialog::OnSysCommand(nID, lParam);
	}
}

void CHanoiTowerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		if(m_eState==State_Stop || m_eState==State_Running)
			m_pHanoiTower->Draw();
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHanoiTowerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHanoiTowerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UpdateData();
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CHanoiTowerDlg::OnTimer(UINT nIDEvent) 
{
	if(m_stLamp.GetBitmap()!=(HBITMAP)m_bmLampOff)
		m_stLamp.SetBitmap((HBITMAP)m_bmLampOff);
	else
		m_stLamp.SetBitmap((HBITMAP)m_bmLampOn);

	CDialog::OnTimer(nIDEvent);
}

void CHanoiTowerDlg::OnClose() 
{
	EState eAwelState=m_eState;
	m_eState=State_End;
	if(eAwelState==State_Stop)
		m_pHanoiTower->ResumeThread();
	CDialog::OnClose();
}

void CHanoiTowerDlg::OnStLamp() 
{
	KillTimer(1);
	m_stLamp.SetBitmap((HBITMAP)m_bmLampOff);
	CAboutDlg AboutDlg;
	AboutDlg.DoModal();
	m_stLamp.SetBitmap((HBITMAP)m_bmLampOn);
	SetTimer(1, 500, NULL);
}

void CHanoiTowerDlg::OnBnBiycik()
{
	switch(m_eState)
	{
		case State_Stop:
				SetDlgItemText(IDC_BTN_CONTROL, _T("Pause"));
				m_pHanoiTower->ResumeThread();
				m_eState=State_Running;
				break;

		case State_Running:
				SetDlgItemText(IDC_BTN_CONTROL, _T("Go"));
				m_pHanoiTower->SuspendThread();
				m_eState=State_Stop;
				break;

		case State_Horz:
				OnBnRing();
	}
}

void CHanoiTowerDlg::OnBnRing()
{
	int nAwelgiCount=m_nRing;
	if(!UpdateData())
	{
		m_nRing=nAwelgiCount;
		UpdateData(FALSE);
		return;
	}

	if(m_eState==State_End)
	{
		GetDlgItem(IDC_EDT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPN_COUNT)->EnableWindow(FALSE);
		SetDlgItemText(IDC_BTN_MODE, _T("Count"));
		SetDlgItemText(IDC_BTN_CONTROL, _T("Go"));
		SetDefID(IDC_BTN_CONTROL);
		CWnd* ppnBiycik=GetDlgItem(IDC_BTN_CONTROL);
		ppnBiycik->EnableWindow();
		ppnBiycik->SetFocus();

		m_pHanoiTower=(CHanoiTower*)AfxBeginThread(RUNTIME_CLASS(CHanoiTower), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		m_pHanoiTower->Create(&m_stBoard, m_nRing, &m_nVelocity, &m_eState);
		m_eState=State_Stop;
	}
	else
	{
		GetDlgItem(IDC_SPN_COUNT)->EnableWindow();
		GetDlgItem(IDC_BTN_CONTROL)->EnableWindow(FALSE);
		SetDlgItemText(IDC_BTN_MODE, _T("Enjoy"));
		SetDefID(IDC_BTN_MODE);
		CWnd* pspCount=GetDlgItem(IDC_EDT_COUNT);
		pspCount->EnableWindow();
		pspCount->SetFocus();

		EState eAwelState=m_eState;
		m_eState=State_End;
		if(eAwelState==State_Stop)
			m_pHanoiTower->ResumeThread();
		m_stBoard.Invalidate();
	}
}
