#include "LR1PG.h"
#include <stack>


class LR_analyser
{
private:
	class stack_block
	{
	public:
		int state_index;
		LR1PG::element elem;
		stack_block(int index,LR1PG::element e)
		{
			state_index=index;
			elem=e;
		}
		stack_block(const stack_block& block)
		{
			this->state_index=block.state_index;
			this->elem=block.elem;
		}
	};
	static map<string,int> var_list;
	static map<string,int> ter_list;
	static set<LR1PG::element> element_set;
	static vector<LR1PG::production> production_set;
	static LR1PG::LR_analysis_table LR_table;
	//static map<int,LR1PG::LR_item_closure> S;
	
	static stack<stack_block> LR_stack;
	static void shift(int state_index,const LR1PG::element& elem)
	{
		LR_stack.push(stack_block(state_index,elem));	
	}
	static void reduction(int produc_index)
	{
		int num=production_set[produc_index].r_part_size;
		for(int i=0;i<num;i++)
			LR_stack.pop();
	}

public:
	void static load_productions(const string& file_name)
	{
		if(LR1PG::produc_set.empty())
			LR1PG::load_productions(file_name);

		{
			var_list=LR1PG::var_list;
			ter_list=LR1PG::ter_list;

			//init element_set
			map<string,int>::iterator it;
			for(it=LR1PG::var_list.begin();it!=LR1PG::var_list.end();it++)
				element_set.insert(LR1PG::element(true,it->second));
			for(it=LR1PG::ter_list.begin();it!=LR1PG::ter_list.end();it++)
				element_set.insert(LR1PG::element(true,it->second));
		}

		production_set=LR1PG::produc_set;
	}
	void static load_table(const string& file_name)
	{
		FILE* fp;
		fp=fopen(file_name.c_str(),"r");
		if(!fp)
		{
			cout<<"Cannot open "<<file_name<<"."<<endl;
			return;
		}
		char buf[10000];
		
		while(true)
		{
			int index;
			int n=fscanf(fp,"%d",&index);
			if(n<=0)
				break;

			fgetc(fp);

			fgets(buf,10000,fp);
			if(!buf)
				return;
			string str(buf);
			size_t line_feed_pos=str.find("\n");
			str.erase(line_feed_pos);

			size_t blank_pos=str.find(" ");
			string elem_str=str.substr(0,blank_pos);
			string act_str=str.substr(blank_pos+1);

			LR1PG::element elem;
			if(ter_list.find(elem_str)!=ter_list.end())
			{
			//!isVar
				elem=LR1PG::element(false,ter_list[elem_str]);

			}
			else if(var_list.find(elem_str)!=var_list.end())
			{
			//isVar
				elem=LR1PG::element(true,var_list[elem_str]);
			}
			else
			{
				cout<<"Error: file form does not match."<<endl;
				return;
			}
			LR1PG::action act;
			switch (act_str[0])
			{
			case 's':
				act=LR1PG::action(LR1PG::action_type::shift,atoi(act_str.substr(1).c_str()));
				break;
			case 'r':
				act=LR1PG::action(LR1PG::action_type::reduction,atoi(act_str.substr(1).c_str()));
				break;
			case 'a':
				act=LR1PG::action(LR1PG::action_type::accept,-1);
				break;
			case 'e':
				act=LR1PG::action(LR1PG::action_type::error,-1);
				break;
			}
			LR_table.set(index,elem,act);
		}

		
		
		fclose(fp);	
	}
	void static analyse(const list<LR1PG::element>& elem_stream)
	{
		list<LR1PG::element>::const_iterator ptr=elem_stream.begin();


		LR_stack.push(stack_block(0,LR1PG::element(false,ter_list.find("#")->second)));
		while(true)
		{
		 	LR1PG::action act=LR_table.at(LR_stack.top().state_index,*ptr);
			
			bool loop_swch=false;

			switch (act.type)
			{
			case LR1PG::action_type::shift:
				shift(act.index,*ptr);
				ptr++;
				break;
			case LR1PG::action_type::reduction:
				reduction(act.index);
				shift(LR_stack.top().state_index,production_set[act.index].l_part);
				break;
			case LR1PG::action_type::accept:
				cout<<"Accept!"<<endl;
				loop_swch=true;
				break;
			case LR1PG::action_type::error:
				cout<<"Error."<<endl;
				loop_swch=true;
				break;
			}

			if(loop_swch)
				break;
		}
	}
	

};
map<string,int> LR_analyser::var_list;
map<string,int> LR_analyser::ter_list;
set<LR1PG::element> LR_analyser::element_set;
vector<LR1PG::production> LR_analyser::production_set;
LR1PG::LR_analysis_table LR_analyser::LR_table;
//map<int,LR1PG::LR_item_closure> LR_analyser::S;
stack<LR_analyser::stack_block> LR_analyser::LR_stack;

void main()
{
	
	LR1PG::load_productions(string("testForEpsilon.txt"));
	LR1PG::generate_table();
	LR1PG::LR_table.save("TABLE");



	//LR_analyser::load_productions("wenfa.txt");
	//LR_analyser::load_table("TABLE");
	//LR_analyser::load(LR1PG::var_list,LR1PG::ter_list,LR1PG::produc_set,LR1PG::set_C,LR1PG::LR_table);
	

	system("pause");
}
