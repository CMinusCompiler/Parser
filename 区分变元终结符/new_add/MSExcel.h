#pragma once

class CMSExcel
{
protected:
	int m_hr;
	IDispatch*	m_pEApp;
	IDispatch*  m_pBooks;
	IDispatch*	m_pActiveBook;
private:
	int Initialize(bool bVisible=true);
public:
	CMSExcel(void);
	~CMSExcel(void);
	int SetVisible(bool bVisible=true);
	int NewExcelBook(bool bVisible=true);
	int OpenExcelBook(LPCTSTR szFilename, bool bVisible=true);
	int SaveAs(LPCTSTR szFilename, int nSaveAs=40);
	int ProtectExcelWorkbook(LPCTSTR szPassword);
	int UnProtectExcelWorkbook(LPCTSTR szPassword);
	int ProtectExcelSheet(int nSheetNo, LPCTSTR szPassword);
	int UnProtectExcelSheet(int nSheetNo, LPCTSTR szPassword);
	int SetExcelCellFormat(LPCTSTR szRange, LPCTSTR szFormat);
	int SetExcelSheetName(int nSheetNo, LPCTSTR szSheetName);
	int GetExcelValue(LPCTSTR szCell, CString &sValue);
	int SetExcelBackgroundColor(LPCTSTR szRange, COLORREF crColor, int nPattern);
	int SetExcelFont(LPCTSTR szRange, LPCTSTR szName, int nSize, COLORREF crColor, bool bBold, bool bItalic);
	int SetExcelValue(LPCTSTR szRange,LPCTSTR szValue,bool bAutoFit, int nAlignment);
	int SetExcelBorder(LPCTSTR szRange, int nStyle);
	int MergeExcelCells(LPCTSTR szRange);
	int AutoFitExcelColumn(LPCTSTR szColumn);
	int AddExcelChart(LPCTSTR szRange, LPCTSTR szTitle, int nChartType, int nLeft, int nTop, int nWidth, int nHeight, int nRangeSheet, int nChartSheet);
	int MoveCursor(int nDirection);
	int GetActiveCell(int &nRow, int &nCol);
	int SetActiveCellText(LPCTSTR szText);
	int Quit();
};
