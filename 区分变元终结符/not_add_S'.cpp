// ExcelTest.cpp : 定义控制台应用程序的入口点。
//




#include "stdafx.h"
//#include "XLRange.h"
#include "MSExcel.h"
#include "afxwin.h"
#include "OLEAuto.h"
#include "ExcelDlg.h"
#include "XLRange.h"
#include "resource.h"
#include "LR1PG.h"
#include <conio.h>
#define UP 3
#define DOWN 4
#define LEFT 1
#define RIGHT 2


CMSExcel m_msExcel;
class MyWindow : public CFrameWnd
{
public:
	MyWindow()
	{
	    Create(NULL,_T("Parser"));

		AllocConsole();
		freopen("CONIN$", "r+t", stdin); // 重定向 STDIN
        freopen("CONOUT$", "w+t", stdout); // 重定向STDOUT 

	
	LR1PG::generate_table();
	set_excel();
	}
	void set_excel()
{
		//MessageBox(_T("Window Created"),_T("CFrameWnd Constructor"));
		CString cstr;
		m_msExcel.NewExcelBook();//new excel
		map<string,int>::iterator it;
		
		set_table_head("变元表");
		for(it=LR1PG::var_list.begin();it!=LR1PG::var_list.end();it++)
		{
		   CString cc(it->first.c_str());//string to CString
		   m_msExcel.SetActiveCellText(cc);//set key
		   m_msExcel.MoveCursor(RIGHT);//right
		   cstr.Format(_T("%d"),it->second);//int to CString
		   m_msExcel.SetActiveCellText(cstr);//set value
		   m_msExcel.MoveCursor(DOWN);//down
		   m_msExcel.MoveCursor(LEFT);//left
		}

		m_msExcel.MoveCursor(DOWN);//down
		set_table_head("终结符表");
		for(it=LR1PG::ter_list.begin();it!=LR1PG::ter_list.end();it++)
		{
		   CString cc(it->first.c_str());//string to CString
		   m_msExcel.SetActiveCellText(cc);//set key
		   m_msExcel.MoveCursor(RIGHT);//right
		   cstr.Format(_T("%d"),it->second);//int to CString
		   m_msExcel.SetActiveCellText(cstr);//set value
		   m_msExcel.MoveCursor(DOWN);//down
		   m_msExcel.MoveCursor(LEFT);//left
		}

		m_msExcel.MoveCursor(DOWN);//down
		set_table_head("产生式表");
		map<LR1PG::production,int> ::iterator itt;
		for(itt=LR1PG::produc_index_map.begin();itt!=LR1PG::produc_index_map.end();itt++)
		{
		   CString cc(itt->first.toString().c_str());//string to CString
		   m_msExcel.SetActiveCellText(cc);//set key
		   m_msExcel.MoveCursor(RIGHT);//right
		   cstr.Format(_T("%d"),itt->second);//int to CString
		   m_msExcel.SetActiveCellText(cstr);//set value
		   m_msExcel.MoveCursor(DOWN);//down
		   m_msExcel.MoveCursor(LEFT);//left
		}



		m_msExcel.MoveCursor(DOWN);//down
		set_table_head("FIRSTVT");

		set<LR1PG::element>::iterator first_set_it;
		map<string,int>::iterator first_it;
		for(first_it=LR1PG::first_sets.FIRST_map.begin();first_it!=LR1PG::first_sets.FIRST_map.end();first_it++)
		{
			CString cc(first_it->first.c_str());//string to CString
			m_msExcel.SetActiveCellText(cc);//set key
			m_msExcel.MoveCursor(RIGHT);//right
			string s_first;
			for(first_set_it=LR1PG::first_sets.FIRST_sets[first_it->second].begin();first_set_it!=LR1PG::first_sets.FIRST_sets[first_it->second].end();first_set_it++)
				s_first+="  "+first_set_it->toString();
			CString ss(s_first.c_str());//string to CString
			m_msExcel.SetActiveCellText(ss);//set key
			m_msExcel.MoveCursor(DOWN);
			m_msExcel.MoveCursor(LEFT);
		}



		m_msExcel.MoveCursor(DOWN);//down
		set_table_head("项目集族");
		set<LR1PG::LR_item_closure>::iterator set_it;
		set<LR1PG::LR_item>::iterator item_it;

		for(set_it=LR1PG::set_C.begin();set_it!=LR1PG::set_C.end();set_it++)
		{
			LR1PG::LR_item_closure closure= LR1PG::LR_item_closure(*set_it);
			for(item_it=closure.closure_instance.begin();item_it!=closure.closure_instance.end();item_it++)
			{
				CString cc(item_it->toString().c_str());//string to CString
				m_msExcel.SetActiveCellText(cc);//set key
				m_msExcel.MoveCursor(RIGHT);//right
				cstr.Format(_T("%d"),closure.index);//int to CString
				m_msExcel.SetActiveCellText(_T("I")+cstr);//set value
				m_msExcel.MoveCursor(DOWN);//down
				m_msExcel.MoveCursor(LEFT);//left

			}	
			m_msExcel.MoveCursor(DOWN);//down
		}
		
		m_msExcel.MoveCursor(DOWN);//down
		m_msExcel.SetActiveCellText(_T("规范LR分析表"));
		m_msExcel.MoveCursor(DOWN);//down
		m_msExcel.SetActiveCellText(_T("状态"));//set cell value


		for(it=LR1PG::ter_list.begin();it!=LR1PG::ter_list.end();it++)
		{
		   m_msExcel.MoveCursor(RIGHT);//right
		   CString cc(it->first.c_str());//string to CString
		   m_msExcel.SetActiveCellText(cc);//set key
		   
		}

		for(it=LR1PG::var_list.begin();it!=LR1PG::var_list.end();it++)
		{
		   if(it->first.compare("S'")!=0)
		   {//not add S' into LR_table 
			   m_msExcel.MoveCursor(RIGHT);//right
			   CString cc(it->first.c_str());//string to CString
			   m_msExcel.SetActiveCellText(cc);//set key   
		   }
		}
		m_msExcel.MoveCursor(DOWN);


		for(int j=0;j<LR1PG::LR_table.num_of_lines;j++)
		{
			for(int i=0;i<LR1PG::ter_list.size()+LR1PG::var_list.size();i++)
				m_msExcel.MoveCursor(LEFT);
			cstr.Format(_T("%d"),j);
			m_msExcel.SetActiveCellText(cstr);//set state
			for(it=LR1PG::ter_list.begin();it!=LR1PG::ter_list.end();it++)
			{
			   m_msExcel.MoveCursor(RIGHT);//right
			   CString cc(LR1PG::LR_table.at(j,LR1PG::element(false,it->second)).toString().c_str());//string to CString
			   m_msExcel.SetActiveCellText(cc);//set key
			}
			
			for(it=LR1PG::var_list.begin();it!=LR1PG::var_list.end();it++)
			{
			  if(it->first.compare("S'")!=0)
			  {//not add S' into LR_table
			   m_msExcel.MoveCursor(RIGHT);//right
			   CString cc(LR1PG::LR_table.at(j,LR1PG::element(true,it->second)).toString().c_str());//string to CString
			   m_msExcel.SetActiveCellText(cc);//set key
			  }
			}
            m_msExcel.MoveCursor(DOWN);
		}
		

}
	void set_table_head(string table)
	{
		CString cc(table.c_str());
		m_msExcel.SetActiveCellText(cc);
		m_msExcel.MoveCursor(DOWN);//down
		m_msExcel.SetActiveCellText(_T("键"));//set cell value
		m_msExcel.MoveCursor(RIGHT);//right
		m_msExcel.SetActiveCellText(_T("值"));//set cell value
		m_msExcel.MoveCursor(DOWN);
		m_msExcel.MoveCursor(LEFT);//left
	}
};



class MyApp : public CWinApp
{
public:
	BOOL InitInstance()
	{
		MyWindow *MywindowObject = new MyWindow();
		m_pMainWnd = MywindowObject;
		MywindowObject->ShowWindow(SW_SHOWNORMAL);
		//MywindowObject->ShowWindow(SW_HIDE);
	
		return TRUE;
	}
	
}App;




int main()
{
	


	App.InitInstance();

	
	return 0;

}








