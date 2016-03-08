#pragma   once  

#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<set>
#include<list>
#include<queue>
#include<cstdio>

using namespace std;


#define EPSILON "¦Å"


namespace LR1PG
{
	class element
	{
	public:
		bool isVar;
		int index;
		bool isNull;
		element(bool isVar,int index)
		{
			this->isVar=isVar;
			this->index=index;
			this->isNull=false;
		}
		element(const element& elem)
		{
			isVar=elem.isVar;
			index=elem.index;
			isNull=elem.isNull;
		}
		element()
		{
			index=-1;
			isNull=true;
		}
		
		string toString() const;
		bool operator<(const element& elem)const;
		void clear();
	
	
	};
	class production
	{
	public:
		element l_part;
		bool isWithEPSILON;
		vector<element> r_part;
		int r_part_size;
		production()
		{
			r_part_size=0;
			isWithEPSILON=false;
		}
		production(const production& produc)
		{
			isWithEPSILON=produc.isWithEPSILON;	
			set_l_part(produc.l_part.isVar,produc.l_part.index);
			vector<element>::iterator it;
			//for(it=produc.r_part.begin();it<produc.r_part.end();it++)
				//  insert_elem(*it);
			for(int i=0;i<produc.r_part.size();i++)
				insert_elem(produc.r_part[i]);
			r_part_size=produc.r_part_size;
		
		}


		void set_l_part(bool isVar,int index);
		void insert_elem(const element& elem);
		string toString() const;
		bool operator<(const production& produc)const;
		

	};
	class FIRST
	{
	private:
		map<string,int> FIRST_map;
		vector<set<element> > FIRST_sets;
		int i_pointer;
		//Return: successful or not
		bool insert(const string& x,const element& elem);
	public:
		FIRST()
		{
			i_pointer=0;
		}
   
		static void init_FIRST_sets(map<string,int>& var_list);
		set<element> find(string x);
		set<element> find(const element& x);
		set<element> find(const vector<element>& beta_a);
		
		void print();
	};
	class flex_production:public production
	{
	public:
		//ptr_pos:giving the dot position(e.g.:A->a . b c, ptr_pos is 0)
		int ptr_pos;

	

		flex_production(int ptr_pos,production& produc):production(produc)
		{
			this->ptr_pos=ptr_pos;
		}
		flex_production():production()
		{
			ptr_pos=-1;
		}
		flex_production(const flex_production& flex_produc):production(flex_produc)
		{
			this->ptr_pos=flex_produc.ptr_pos;
		}


		element get_l_element();
		element get_r_element();
		bool operator<(const flex_production& produc)const;
		string toString()const;
		
	
	};
	class LR_item:public flex_production,public element
	{
	public:
		LR_item():flex_production(),element()
		{}
		LR_item(int ptr_pos,production& produc,bool isVar,int index):flex_production(ptr_pos,produc),element(isVar,index)
		{}
		LR_item(const LR_item& item):flex_production(item),element(item)
		{}


		//Return: a flex_production reference object(with dot shifted right)
		LR_item ptr_r_shift();
		bool operator<(const LR_item& produc)const;
		bool operator==(const LR_item& produc)const;
		bool operator!=(const LR_item& produc)const;
		string toString()const;
		
	

	};
	//LR_item closure for LR(LEFT)
	class LR_item_closure
	{
	private:
		static int index_generator;
	public:
		set<LR_item> closure_instance;
		int size;
		int index;


		
		LR_item_closure()
		{
			size=0;
		
		}
		LR_item_closure(const LR_item_closure& closure)
		{
			set<LR_item>::const_iterator it;
			for(it=closure.closure_instance.begin();it!=closure.closure_instance.end();it++)
				insert((*it));
		
			this->size=closure.size;

			this->index=closure.index;
		}
		
		
		//Return: successfully done or not.
		bool insert(const LR_item& item);
		static void incre();
		static int get_index_generator();
		void clear();
		bool operator<(const LR_item_closure& closure)const;
		void closure_completion();
		void print();
	

	};
	enum action_type
	{
		shift,reduction,accept,error
	};
	class action
	{
	public:
		action_type type;
		//state index or production index
		int index;
		bool isNull;
		action()
		{
			type=action_type::error;
			isNull=1;
		}
		action(action_type type,int index)
		{
			this->type=type;
			this->index=index;
			isNull=0;
		}
		action(const action& act)
		{
			this->type=act.type;
			this->index=act.index;
			this->isNull=act.isNull;
		}


		string toString()const;
	};
	class LR_analysis_table
	{
	public:
		action at(int index,element elem);
		void set_row(int index,const map<element,action>& row);
		void set(int index,element elem,const action& act);
		int num_of_lines;
		LR_analysis_table()
		{
			num_of_lines=0;
		}
		LR_analysis_table(const LR_analysis_table& LR_table)
		{
			this->table=LR_table.table;
			this->num_of_lines=LR_table.num_of_lines;
		}
		void save(const string& file_name);

	private:	
		map<int,map<element,action>> table;
		
		//static vector<map<element,action>> table;
	};
	
	class C
	{
	private:
		static set<LR_item_closure> instance;
		static map<element,map<LR_item_closure,LR_item_closure> > GO_buf;
		LR_item_closure static GO(const LR_item_closure& I,element X);
	public:
		void static construction();
		void static print();

	};


	//LR_item_closure GO(const LR_item_closure& I,element X);
	//void set_C_construction();
	void load_productions(const string& file_name);
	void generate_table();
	void split(std::string& s, std::string& delim,std::vector< std::string >* ret);
	
	
	

	extern map<string,int> var_list;//¡À???¡À¨ª
	extern map<string,int> ter_list;//???¨¢¡¤?¡À¨ª
	extern map<int,string> re_var_list;//¡À???¡À¨ª
	extern map<int,string> re_ter_list;//???¨¢¡¤?¡À¨ª
	extern FIRST first_sets;
	extern LR_analysis_table LR_table;
	extern vector<production> produc_set;//?¨²?¨²??????
	extern map<production,int> produc_index_map;
	//extern set<LR_item_closure> set_C;



}