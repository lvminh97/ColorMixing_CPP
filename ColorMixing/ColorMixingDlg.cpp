
// ColorMixingDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ColorMixing.h"
#include "ColorMixingDlg.h"
#include "afxdialogex.h"

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


// CColorMixingDlg dialog



CColorMixingDlg::CColorMixingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COLORMIXING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CColorMixingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHART_CTRL, m_ChartCtrl);
}

BEGIN_MESSAGE_MAP(CColorMixingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_COLOR_DATA, &CColorMixingDlg::OnBnClickedButtonImportColorData)
	ON_BN_CLICKED(IDC_BUTTON_COMPUTE, &CColorMixingDlg::OnBnClickedButtonCompute)
END_MESSAGE_MAP()


// CColorMixingDlg message handlers

BOOL CColorMixingDlg::OnInitDialog()
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
	componentMapping();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CColorMixingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CColorMixingDlg::OnPaint()
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
HCURSOR CColorMixingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CColorMixingDlg::componentMapping(void)
{
	colorDataFilenameStatic = (CStatic*)GetDlgItem(IDC_STATIC_COLOR_DATA_FILENAME);
	colorDataEdit = (CEdit*)GetDlgItem(IDC_EDIT_COLOR_DATA);

	CChartStandardAxis* pBottomAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetMinMax(380, 760);
	CChartStandardAxis* pLeftAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pLeftAxis->SetMinMax(0, 1.5);
	m_ChartCtrl.GetTitle()->AddString(_T("Color's reflection"));
	m_ChartCtrl.SetZoomEnabled(FALSE);

	setOutputColor(IDC_BUTTON_SHOW_SAMPLE_COLOR, 0xFFFFFF);
	setOutputColor(IDC_BUTTON_SHOW_COMPUTED_COLOR, 0xFFFFFF);
}

void CColorMixingDlg::setOutputColor(int nID, unsigned int rgb)
{
	CButton* outputColorButton = (CButton*)GetDlgItem(nID);
	CBitmap bmp;
	CRect outputColorButtonSize;
	outputColorButton->GetWindowRect(&outputColorButtonSize);
	COLORREF* bmpData = new COLORREF[outputColorButtonSize.Width() * outputColorButtonSize.Height()];
	for (int i = 0; i < outputColorButtonSize.Height(); i++)
	{
		for (int j = 0; j < outputColorButtonSize.Width(); j++)
		{
			bmpData[outputColorButtonSize.Width() * i + j] = RGB(rgb & 0xFF, (rgb >> 8) & 0xFF, (rgb >> 16) & 0xFF);
		}
	}
	bmp.CreateBitmap(outputColorButtonSize.Width(), outputColorButtonSize.Height(), 1, 32, bmpData);
	outputColorButton->SetBitmap(bmp);
}

void CColorMixingDlg::OnBnClickedButtonImportColorData()
{
	const TCHAR szFilter[] = _T("Text files (*.txt)|*.txt");
	CFileDialog dlg(TRUE, _T("txt"), NULL, OFN_READONLY, szFilter, this);
	if (dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();
		CString sFileName = dlg.GetFileName();
		// read file
		std::ifstream colorFile;
		colorFile.open(CT2A(sFilePath));
		if (colorFile.is_open() == FALSE)
		{
			MessageBox(CA2W("Can not open the data file"), CA2W("Error"));
			return;
		}

		std::string tmpLine = "";
		double tmpDouble = 0;
		int index = 0;
		while (std::getline(colorFile, tmpLine))
		{
			try
			{
				tmpDouble = std::stod(tmpLine);
			}
			catch (const std::exception&)
			{
				MessageBox(CA2W("The format of input data file is not valid"), CA2W("Error"));
				return;
			}
			
			inputColor.setDataAt(index, tmpDouble);
			index++;
		}
		if (index != 31)
		{
			MessageBox(CA2W("The format of input data file is not valid"), CA2W("Error"));
			return;
		}
		
		colorDataFilenameStatic->SetWindowTextW(sFileName);
		colorDataEdit->SetWindowTextW(CA2W(inputColor.getReflectionString().c_str()));
	}
}


void CColorMixingDlg::OnBnClickedButtonCompute()
{
	m_ChartCtrl.RemoveAllSeries();
	CChartLineSerie* m_pLineSeries = m_ChartCtrl.CreateLineSerie(false, false);
	m_pLineSeries->SetColor(0x0000FF);
	m_pLineSeries->SetWidth(2);

	for (int i = 0; i < 31; i++)
	{
		m_pLineSeries->AddPoint(400 + i * 10, inputColor.getDataAt(i));
	}

	setOutputColor(IDC_BUTTON_SHOW_SAMPLE_COLOR, 0x00E01864);
	setOutputColor(IDC_BUTTON_SHOW_COMPUTED_COLOR, 0x00A0E000);
}
