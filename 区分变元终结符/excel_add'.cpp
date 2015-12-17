int CMSExcel::SetActiveCellText(LPCTSTR szText)
{
	CString cstr(szText);
	cstr=_T("'")+cstr;

	if(!m_pEApp || !m_pActiveBook) return E_FAIL;

	IDispatch *pXLApp;
	{  
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, m_pActiveBook, L"Application", 0);
		pXLApp= result.pdispVal;
	}
	IDispatch *pActiveCell;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pXLApp, L"ActiveCell", 0);
		pActiveCell=result.pdispVal;
	}

	int nRow,nCol;
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pActiveCell, L"Row", 0);
		nRow=result.iVal;
	}
	{
		VARIANT result;
		VariantInit(&result);
		OLEMethod(DISPATCH_PROPERTYGET, &result, pActiveCell, L"Column", 0);
		nCol=result.iVal;
	}

	
	IDispatch *pCells;
	{
		VARIANT result;
		VariantInit(&result);
		VARIANT row, col;
		row.vt =VT_I4;
		row.lVal =nRow;
		col.vt =VT_I4;
		col.lVal =nCol;
		OLEMethod(DISPATCH_PROPERTYGET, &result, pXLApp, L"Cells", 2,col,row);
		pCells=result.pdispVal;
	}
	{
		
		COleVariant val(cstr);
		OLEMethod(DISPATCH_PROPERTYPUT, NULL, pCells, L"Value", 1,val.Detach());
	}
	pCells->Release();
	pActiveCell->Release();
	pXLApp->Release();
	return m_hr;
}