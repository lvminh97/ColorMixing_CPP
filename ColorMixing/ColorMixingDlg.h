
// ColorMixingDlg.h : header file
//

#pragma once

#include "ChartLib/ChartCtrl.h"
#include "ChartLib/ChartLineSerie.h"
#include "Colors.h"


// CColorMixingDlg dialog
class CColorMixingDlg : public CDialogEx
{
	// Construction
public:
	CColorMixingDlg(CWnd* pParent = nullptr);	// standard constructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLORMIXING_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
private:
	Colors inputColor;

protected:
	HICON m_hIcon;
	CStatic* colorDataFilenameStatic = NULL;
	CEdit* colorDataEdit = NULL;
	CChartCtrl m_ChartCtrl;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

		void componentMapping(void);

public:
	afx_msg void OnBnClickedButtonImportColorData();
	afx_msg void OnBnClickedButtonCompute();
};
