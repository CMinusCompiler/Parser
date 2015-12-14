#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<set>
#include<list>
#include<queue>
using namespace std;
#define FILENAME "test2.txt"
#define MAXROW 100
#define EPSILON "ε"
string var;//变元
map<string,int> var_list;//变元表
map<string,int> ter_list;//终结符表
map<int,string> re_var_list;//变元表
map<int,string> re_ter_list;//终结符表
map<string,int>::iterator it;



string edi_str;

class element
{
public:
    bool isVar;
    int index;
    bool isNull;
	string& toString() const
    {
        //Judge whether the index is of meaning.
        if((isVar&&re_var_list.find(index)!=re_var_list.end())||(!isVar&&re_ter_list.find(index)!=re_ter_list.end()))
            return isVar?re_var_list[index]:re_ter_list[index];
        else
		{
			edi_str.clear();
			edi_str=string("null");
			return edi_str;
		}
		//else return *(new string("null");)
    }

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
	//$$
    bool operator<(const element& elem)const
    {
		string str1=this->toString();
		string str2=elem.toString();

		return (str1<str2);
    }
	//$$
	void clear()
	{
		index=-1;
		isNull=true;
	}
	
	
}edi_elem;



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
    void set_l_part(bool isVar,int index)
    {
        l_part.isVar=isVar;
        l_part.index=index;
    }
    void insert_elem(const element& elem)
    {
        r_part.push_back(elem);
        r_part_size++;
		if(elem.toString().compare(EPSILON)==0)
			isWithEPSILON=true;
    }
	//$$
    string& toString() const
    {
        /*string* produc=new string();
        (*produc)+=l_part.toString();
        (*produc)+=string("->");
        vector<element>::iterator it;
        for(it=r_part.begin();it<r_part.end();it++)
        {
            (*produc)+=it->toString();
            if(it!=r_part.end()-1)
                (*produc)+=string(" ");
        }*/
		
		edi_str.clear();
		edi_str+=l_part.toString();
        edi_str+=string("->");
        vector<element>::const_iterator it;
        for(it=r_part.begin();it<r_part.end();it++)
        {
            edi_str+=it->toString();
            if(it!=r_part.end()-1)
                edi_str+=string(" ");
        }

        return edi_str;

    }
	
	bool operator<(const production& produc)const
    {
		string str1=this->toString();
		string str2=produc.toString();

		return (str1<str2);
    }
	//$$

};

set<element> edi_elem_set;

vector<production> produc_set;//产生式集合
map<production,int> produc_index_map;


class FIRST
{
private:
    map<string,int> FIRST_map;
    vector<set<element> > FIRST_sets;
    int i_pointer;
public:
    FIRST()
    {
        i_pointer=0;
    }
   
    //Return: successful or not
    bool insert(const string& x,const element& elem)
    {
        bool result=0;

        //Judge whether x exists in var_list or ter_list.
        if(var_list.find(x)==var_list.end()&&ter_list.find(x)==ter_list.end())
            return 0;

        map<string,int>::iterator it;

        //FISRT(x) has not been established.
        if(FIRST_map.find(x)==FIRST_map.end())
        {
            FIRST_map.insert(pair<string,int>(x,i_pointer));
            //Establish a FIRST set with nothing.
            FIRST_sets.push_back(set<element>());
            i_pointer++;
        }


        it=FIRST_map.find(x);
        //Judge whether elem exists in FISRT(x).
        if(FIRST_sets[it->second].find(elem)!=FIRST_sets[it->second].end())
            result=0;
        else
        {
            FIRST_sets[it->second].insert(elem);
            result=1;
        }
        return result;

    }



    set<element>& find(string x)
    {
		/*
        set<element>* a=new set<element>;

         //Judge whether x exists in var_list or ter_list.
        if(var_list.find(x)==var_list.end()&&ter_list.find(x)==ter_list.end())
            return *a;

        //If x is a terminative, return {x}.
        if(ter_list.find(x)!=ter_list.end())
        {
            a->insert(element(0,ter_list.find(x)->second));
            return *a;
        }
        else
        {
            //FISRT(x) has not been established.
            if(FIRST_map.find(x)==FIRST_map.end())
            {
                FIRST_map.insert(pair<string,int>(x,i_pointer));
                //Establish a FIRST set with nothing.
                FIRST_sets.push_back(set<element>());
                i_pointer++;
            }

            map<string,int>::iterator it;
            it=FIRST_map.find(x);
            return FIRST_sets[it->second];
        }
		*/
		edi_elem_set.clear();

         //Judge whether x exists in var_list or ter_list.
        if(var_list.find(x)==var_list.end()&&ter_list.find(x)==ter_list.end())
            return edi_elem_set;

        //If x is a terminative, return {x}.
        if(ter_list.find(x)!=ter_list.end())
        {
            edi_elem_set.insert(element(0,ter_list.find(x)->second));
            return edi_elem_set;
        }
        else
        {
            //FISRT(x) has not been established.
            if(FIRST_map.find(x)==FIRST_map.end())
            {
                FIRST_map.insert(pair<string,int>(x,i_pointer));
                //Establish a FIRST set with nothing.
                FIRST_sets.push_back(set<element>());
                i_pointer++;
            }

            map<string,int>::iterator it;
            it=FIRST_map.find(x);
            return FIRST_sets[it->second];
        }

    }
	//$$
	vector<string> find(vector<string>& beta_a)
   { //求first(beita a),其中beita=（V|T）*，a属于T
    //返回first集，vector中string均属于T
	vector<string> first;
	set<element>::iterator it;
	vector<production>::iterator pro_it;
	bool epsilon=false;
	bool exist=false;
	for(int i=0;i<beta_a.size();i++)
	{
		exist=false;
		if(ter_list.find(beta_a[i])==ter_list.end())//beta_a[i]是变元
		{		
			for(it=first_sets.find(beta_a[i]).begin();it!=first_sets.find(beta_a[i]).end();it++)
				if(it->toString().compare(EPSILON)!=0)
				{
					for(int j=0;j<first.size();j++)
						if(first[j].compare(it->toString())==0)
						{//已存在，不需加入
							exist=true;
							break;
						}
					if(!exist)
			           first.push_back(it->toString());//将beta_a[i]的first集加入,不加入空,不重复加入
				}
			for(pro_it=produc_set.begin();pro_it<produc_set.end();pro_it++)
			{//遍历产生式集合，看beta_a[i]是否能产生空
				if(pro_it->l_part.toString().compare(beta_a[i])==0)//找到beta_a[i]的产生式
				   if(pro_it->isWithEPSILON)
				   {
					   epsilon=true;
					   break;
				   }
			}
			if(!epsilon)//beta_a[i]不含空产生式
				break;
		}
		else//beta_a[i]是终结符，加入first集，注意#也是终结符
		{
			for(int j=0;j<first.size();j++)
				if(first[j].compare(beta_a[i])==0)
				{//已存在，不需加入
					exist=true;
					break;
				}
			if(!exist)
			    first.push_back(beta_a[i]);
			break;
		}
	}
	return first;
    }
    set<element> find(vector<element>& beta_a)
   { //求first(beita a),其中beita=（V|T）*，a属于T
    //返回first集
	set<element> first;
	set<element>::iterator it;
	vector<production>::iterator pro_it;
	bool epsilon=false;
	bool exist=false;
	for(int i=0;i<beta_a.size();i++)
	{
		exist=false;
		if(ter_list.find(beta_a[i].toString())==ter_list.end())//beta_a[i]是变元
		{		
			for(it=first_sets.find(beta_a[i].toString()).begin();it!=first_sets.find(beta_a[i].toString()).end();it++)
				if(it->toString().compare(EPSILON)!=0)
				{
					
					if(first.find(element(false,ter_list.find(it->toString())->second))!=first.end())
					{//已存在，不需加入
						exist=true;
						break;
					}
					if(!exist)
			           first.insert(element(false,ter_list.find(it->toString())->second));//将beta_a[i]的first集加入,不加入空,不重复加入
				}
			for(pro_it=produc_set.begin();pro_it<produc_set.end();pro_it++)
			{//遍历产生式集合，看beta_a[i]是否能产生空
				if(pro_it->l_part.toString().compare(beta_a[i].toString())==0)//找到beta_a[i]的产生式
				   if(pro_it->isWithEPSILON)
				   {
					   epsilon=true;
					   break;
				   }
			}
			if(!epsilon)//beta_a[i]不含空产生式
				break;
		}
		else//beta_a[i]是终结符，加入first集，注意#也是终结符
		{
			if(first.find(element(false,beta_a[i].index))!=first.end())
			{//已存在，不需加入
				exist=true;
				break;
			}
			if(!exist)
				first.insert(element(false,beta_a[i].index));
			break;
		}
	}
	return first;
    }
	//$$
	
	void print()
	{
		set<element>::iterator set_it;
		for(it=FIRST_map.begin();it!=FIRST_map.end();it++)
		{
			cout<<it->first<<":";
			for(set_it=FIRST_sets[it->second].begin();set_it!=FIRST_sets[it->second].end();set_it++)
				cout<<set_it->toString()<<" ";
			cout<<endl;
		}
	}
}first_sets;
void split(std::string& s, std::string& delim,std::vector< std::string >* ret);
void init_first_sets(map<string,int>& var_list);


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
	element& get_l_element()
	{
		//If the dot is at the leftest side, return a null element.
		if(ptr_pos==-1)
		{
			edi_elem.clear();
			return edi_elem;
		}
		else
			return production::r_part[ptr_pos];
	}
	element& get_r_element()
	{
		//If the dot is at the rightest side, return a null element.
		if(ptr_pos==production::r_part_size-1)
		{
			edi_elem.clear();
			return edi_elem;
		}
		else 
			return production::r_part[ptr_pos+1];

	}

	//$$
	bool operator<(const flex_production& produc)const
	{
		string str1=this->toString();
		string str2=produc.toString();

		return (str1<str2);
	}
	string& toString()const
	{

		string str;
		str+=l_part.toString();
		str+=string("->");

		
		
        
		//·
		
		

        for(int i=-1;i<(int)(r_part.size());i++)
        {
			if(i!=-1)
				str+=r_part[i].toString();
            if(i!=r_part.size()-1&&i!=ptr_pos)
                str+=string(" ");
			if(i==ptr_pos)
				str+="·";
			
			
        }
		edi_str.clear();
		edi_str=string(str);

        return edi_str;
	}
	//$$
	
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
    LR_item& ptr_r_shift()
    {

		edi_LR_item.clear();
		
		edi_LR_item=LR_item(*this);

		if(ptr_pos==production::r_part_size-1)
			return edi_LR_item;
		else
		{
			edi_LR_item.ptr_pos++;
			return edi_LR_item;
		}
    }

	//$$
	bool operator<(const LR_item& produc)const
	{
	 	string str1=this->toString();
		string str2=produc.toString();

		return (str1<str2);
	}
	bool operator==(const LR_item& produc)const
	{
		string str1=this->toString();
		string str2=produc.toString();

		return (str1==str2);

		return this->toString()==produc.toString();
	}
	bool operator!=(const LR_item& produc)const
	{
		return this->toString()!=produc.toString();
	}
	
	string& toString()const
	{
		string str;
		str+="[";
		str+=flex_production::toString();
		str+=" , ";
		str+=element::toString();
		str+="]";
		edi_str.clear();
		edi_str=string(str);
		return edi_str;
	}
	//$$
	

}edi_LR_item;



//LR_item closure for LR(1)
class LR_item_closure
{
private:
	static int index_generator;
public:
	set<LR_item> closure_instance;
	int size;
	int index;


	//Return: successfully done or not.
	bool insert(const LR_item& item)
	{
		if(closure_instance.find(item)!=closure_instance.end())
			return false;
		
		closure_instance.insert(item);
		size++;
		return true;
	}

	static void incre()
	{
		index_generator++;
	}
	static int get_index_generator()
	{
		return index_generator;
	}
	
	
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
	

	void clear()
	{
		closure_instance.clear();
		size=0;
	}
	//$$
	bool operator<(const LR_item_closure& closure)const
	{
		
		
		if(this->size!=closure.size)
			return this->size<closure.size;
		
		set<LR_item>::iterator it=this->closure_instance.begin();
		for(;it!=this->closure_instance.end();it++)
		{
			//if it does not exist.
			if(closure.closure_instance.find(*it)==closure.closure_instance.end())
				return (*it)<(*(closure.closure_instance.begin()));
			
		}
		return false;
		
	}
	void closure_completion()
	{//  closure_instance=CLOSURE（closure_instance）
		set<LR_item>::iterator it;
		bool isChanged=false;
		while(1)
		{
			isChanged=false;
			for(it=closure_instance.begin();it!=closure_instance.end();it++)
			{
				LR_item not_const_it=LR_item(*it);
				element B=not_const_it.get_r_element();
				//element B=it->get_r_element();
				if(!B.isNull)//the dot isn't at the rightest
				{		
					if(B.isVar)//the right of the dot is a V    ;A->a.B……
					{
						for(int i=0;i<produc_set.size();i++)
						{
							if(produc_set[i].l_part.index==B.index&&produc_set[i].l_part.isVar==B.isVar)//it->get_r_element() production ;B->……
							{
								vector<element> beta_a;
								for(int j=it->ptr_pos+2;j<it->r_part_size;j++)//A->a.Bbeita,a, put beita to vector<element> beta_a
									beta_a.push_back(element(it->r_part[j].isVar,it->r_part[j].index));
								beta_a.push_back(element(false,it->index));//A->a.Bbeita,a, put a to vector<element> beta_a
								set<element>  first_beita_a=first_sets.find(beta_a);
								set<element>::iterator first_it;
								for(first_it=first_beita_a.begin();first_it!=first_beita_a.end();first_it++)//first(beta_a)
									isChanged|=insert(LR_item(-1,produc_set[i],false,first_it->index));
							}
						}
					}
				}
			}
			if(isChanged==false)//until not change
				break;
		}		
	}
	//$$

	void print()
	{
		cout<<"I"<<index<<":"<<endl;
		set<LR_item >::iterator it;
		for(it=closure_instance.begin();it!=closure_instance.end();it++)
		{
			LR_item item=LR_item(*it);
			cout<<item.toString()<<endl;
		}
	}
	

}edi_closure;
int LR_item_closure::index_generator=0;

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
	string& toString()const
	{
		edi_str.clear();
		string str;
		switch (type)
		{
		case shift:
			str+="s";
			str+=index;
			break;
		case reduction:
			str+="r";
			str+=index;
			break;
		case accept:
			str+="acc";
			break;
		case error:
			str+="err";
			break;
		default:
			break;
		}
		edi_str=string(str);
		return edi_str;
	}
}edi_action;
class LR_analysis_table
{
public:
	action& at(int index,element elem) 
	{
		return  (table[index])[elem];
		
	}
	void set_row(int index,const map<element,action>& row)
	{
		table[index]=row;
	}
	void set(int index,element elem,const action& act)
	{
		//If it exists already
		if(table[index].find(elem)!=table[index].end())
		{

			string str1=(table[index])[elem].toString();
			string str2=act.toString();
			if(str1!=str2)
			{
			//There is a conflict.
				cout<<"Conflicts at (I"<<index<<","<<elem.toString()<<"):"
					<<str1<<" vs. "<<str2<<endl;
			}

		}
		(table[index])[elem]=act;
	}

private:
	map<int,map<element,action>> table;
	//static vector<map<element,action>> table;
}LR_table;

set<LR_item_closure> set_C;
void split(std::string& s, std::string& delim,std::vector< std::string >* ret);
void init_first_sets(map<string,int>& var_list);
LR_item_closure& GO(const LR_item_closure& I,element X)
 {
	 edi_closure.clear();
	 set<LR_item> ::iterator it;
	 for(it=I.closure_instance.begin();it!=I.closure_instance.end();it++)
	 {
		 LR_item item=LR_item(*it);
		 if(!item.get_r_element().isNull)
			 if(item.get_r_element().index==X.index&&item.get_r_element().isVar==X.isVar)
			 {
				 LR_item temp=item.ptr_r_shift();
				  edi_closure.insert(temp);
			 }
		        
	 }
	 edi_closure.closure_completion();
	 return edi_closure;
 }
void set_C_construction();


void main()
{
	int i=1;
	int produc_index_generator=0;
	string s=" ";
	string str;
	string ter_var;//产生式右边的串
	vector<string> var_ter_vec;//产生式右部的划分
	
	char cstr[MAXROW];
	int pos;
	FILE *f;
	if((f=fopen(FILENAME,"r"))==NULL)
	{
		cout<<"open error"<<endl;
		exit(0);
	}
	while(!feof(f))
	{
	fgets(cstr,MAXROW,f);
	str=cstr;
	pos=str.find('→');
	if(i==1)
	{
		var_list.insert(pair<string,int>("S'",i));//first insert S' into var_list
		re_var_list[i]="S'";
		i++;
	}
	if(pos>0)
	{
		var=str.substr(0,pos-1);
		if((var_list.find(var))==var_list.end())//没找到
		{
			var_list.insert(pair<string,int>(var,i));//插入变元表
			re_var_list.insert(pair<int,string>(i,var));
			i++;
		}
	}
	}
	fclose(f);

	if((f=fopen(FILENAME,"r"))==NULL)
	{
		cout<<"open error"<<endl;
		exit(0);
	}
	i=1;
	while(!feof(f))
	{
		fgets(cstr,MAXROW,f);
		str=cstr;
		if(str[str.length()-1]=='\n')
			str.resize(str.size()-1);
		pos=str.find('→');
		if(pos>0)
		{
			production product;
			product.set_l_part(true,var_list.find(str.substr(0,pos-1))->second);
			ter_var=str.substr(pos+1,str.length()-pos-1);//产生式右部
			split(ter_var,s,&var_ter_vec);
			for(int j=0;j<var_ter_vec.size();j++)
			{
				if((var_list.find(var_ter_vec[j]))==var_list.end())//不是变元，即为终结符
				{

					if(ter_list.find(var_ter_vec[j])==ter_list.end())
					{
					   product.insert_elem(element(false,i));//产生式插入右部终结符
					   ter_list.insert(pair<string,int>(var_ter_vec[j],i));//插入终结符表				   
					   re_ter_list[i]=var_ter_vec[j];//re_ter_list.insert(pair<int,string>(i,var_ter_vec[j]));
					   i++;
					}
					else
						product.insert_elem(element(false,ter_list.find(var_ter_vec[j])->second));//产生式插入右部终结符
				}
				else//变元
				{	
					product.insert_elem(element(true,(var_list.find(var_ter_vec[j]))->second));//产生式插入右部变元
				}
			}	
			produc_set.push_back(product);//构造produc_set
			
			//Assign index
			produc_index_map[product]=produc_index_generator;
			produc_index_generator++;
		}    
	}
	ter_list.insert(pair<string,int>("#",i));//最后#也插入终结符表				   
    re_ter_list[i]="#";

	fclose(f);
	init_first_sets(var_list);
	//first_sets.print();
	//set_C_construction();



    //test LR_item_closure::closure_completion
	//LR_item_closure test;
	//test.insert(LR_item(-1,produc_set[1],0,3));
	//test.closure_completion();
	//test.print();

	set_C_construction();

	set<LR_item_closure>::iterator it;
	for(it=set_C.begin();it!=set_C.end();it++)
	{
		LR_item_closure mm=LR_item_closure(*it);
		mm.print();
		cout<<endl<<endl;
	}
	/*test first(beita_a)
	vector<string> a;
	a.push_back("E'");
	a.push_back("R'");
	a.push_back("a");
	first_sets.find(a);

	vector<element> b;
	b.push_back(element(1,var_list.find("E'")->second ));
	b.push_back(element(1,var_list.find("R'")->second ));
	b.push_back(element(0,ter_list.find("a")->second ));
	first_sets.find(b);*/
	/*cout<<"变元表"<<endl;
	for(it=var_list.begin();it!=var_list.end();it++)
	{
		cout<<it->first<<"===="<<it->second<<endl;
	}
	cout<<"终结符表"<<endl;
	for(it=ter_list.begin();it!=ter_list.end();it++)
	{
		cout<<it->first<<"===="<<it->second<<endl;
	}
	*/
	system("pause");

}
void split(std::string& s, std::string& delim,std::vector< std::string >* ret)  
{  //将s按照delim划分存到ret中
    size_t last = 0;  
    size_t index=s.find_first_of(delim,last);  
	ret->clear();
    while (index!=std::string::npos)  
    {  
        ret->push_back(s.substr(last,index-last));  
        last=index+1;  
        index=s.find_first_of(delim,last);  
    }  
    if (index-last>0)  
    {  
        ret->push_back(s.substr(last,index-last));  
    }  
}  

void init_first_sets(map<string,int>& var_list)
{//求所有变元的first集
	bool isChanged=false;
	map<string,int>::iterator it; 
	set<element>::iterator set_it;

	while(1)
	{
		isChanged =false;
		for(it=var_list.begin();it!=var_list.end();it++)
		{//遍历变元表
			 for(int i=0;i<produc_set.size();i++)
			 {//遍历产生式集合
				 if((it->first.compare(produc_set[i].l_part.toString()))==0)//找到该变元X的产生式
				 {
					if(!produc_set[i].r_part[0].isVar)//X->a,注意空也是终结符
						isChanged|=first_sets.insert(it->first,produc_set[i].r_part[0]);//将a插入first(X)						
					else//X->Y
					{
						for(int k=0;k<produc_set[i].r_part.size();k++)
						{//对Y1-YN遍历
							if(!(var_list.find(produc_set[i].r_part[k].toString())==var_list.end()))
							{//Yk是变元							
								set<element> first_yk=first_sets.find(produc_set[i].r_part[k].toString());//first(Yk)
								for(set_it=first_yk.begin();set_it!=first_yk.end();set_it++)//将first(Yk)中所有非空元素加入first(X)
									if((*set_it).toString().compare(EPSILON)!=0)	
										isChanged|=first_sets.insert(it->first,*set_it);
								if(ter_list.find(EPSILON)!=ter_list.end())
								{
									if(first_sets.find(produc_set[i].r_part[k].toString()).find(element(false,ter_list.find(EPSILON)->second))!=(first_sets.find(produc_set[i].r_part[k].toString())).end())
									{//如果first(Yk)有空
										if(k==produc_set[i].r_part.size()-1)
										{									
											isChanged|=first_sets.insert(it->first,element(false,ter_list.find(EPSILON)->second));//插入空
											break;
										}
									}
									else
										break;
								}
							}
							
						}
					}
				 }
			 }
		}
		if(isChanged==false)
			break;
	}

}
 
void set_C_construction()
{

	edi_closure.clear();
	production externed_produc;
	externed_produc.set_l_part(true,1);//S'
	externed_produc.insert_elem(element(true,2));//S
	edi_closure.insert(LR_item(-1,externed_produc,false,ter_list.find("#")->second));//[S'->.S,#]
	edi_closure.closure_completion();//CLOSURE([S'->.S,#])

	//set the index of the closure
	edi_closure.index=LR_item_closure::get_index_generator();
	LR_item_closure::incre();

	set<LR_item>::iterator iter;
	for(iter=edi_closure.closure_instance.begin();iter!=edi_closure.closure_instance.end();iter++)
	{
		//Reduction action:
		LR_item item=LR_item(*iter);
		//If it is a reduction item
		if(item.get_r_element().isNull)
			LR_table.set(edi_closure.index,item,action(action_type::reduction,produc_index_map[item]));
	}



	

	set_C.insert(edi_closure);
	map<string,int>::iterator map_it;
	set<LR_item_closure> ::iterator set_it; 
	bool isChanged=false;
	while(1)
	{
		isChanged=false;
		for(set_it=set_C.begin();set_it!=set_C.end();set_it++)
		{
			for(map_it=var_list.begin();map_it!=var_list.end();map_it++)
			{
				LR_item_closure closure=GO(*set_it,element(true,map_it->second));
				//If it is going to be inserted

				
				if(closure.size>0&&set_C.find(closure)==set_C.end())
				{
					//set the index of the closure
					closure.index=LR_item_closure::get_index_generator();
					LR_item_closure::incre();
				}
				else if(closure.size>0)
				{
					closure.index=set_C.find(closure)->index;		
				}
				else
					continue;
				//If it is a empty closure, continue.

	
				//Shift action:
				LR_table.set(set_it->index,element(true,map_it->second),action(action_type::shift,closure.index));
				
				
				set<LR_item>::iterator iter;
				for(iter=closure.closure_instance.begin();iter!=closure.closure_instance.end();iter++)
				{
					//Reduction action:
					LR_item item=LR_item(*iter);
					
					//Accept action:
					if(item==LR_item(0,externed_produc,false,ter_list.find("#")->second))
					{
						cout<<item.toString()<<endl;
						LR_table.set(closure.index,element(false,ter_list.find("#")->second),action(action_type::accept,-1));
						//Do not need other iterations then.
						break;
					}
					
					//If it is a reduction item
					if(item.get_r_element().isNull)
						LR_table.set(closure.index,item,action(action_type::reduction,produc_index_map[item]));
					
					
				}

				
				

				if(closure.size>0&&set_C.find(closure)==set_C.end())
				{
					

					set_C.insert(closure);
					
					isChanged=true;
				}
			}
			for(map_it=ter_list.begin();map_it!=ter_list.end();map_it++)
			{

				

				LR_item_closure closure=GO(*set_it,element(false,map_it->second));

				//If it is going to be inserted
				if(closure.size>0&&set_C.find(closure)==set_C.end())
				{
					//set the index of the closure
					closure.index=LR_item_closure::get_index_generator();
					LR_item_closure::incre();
				}
				else if(closure.size>0)
				{
					closure.index=set_C.find(closure)->index;		
				}
				else
					continue;

				//Shift action:
				LR_table.set(set_it->index,element(false,map_it->second),action(action_type::shift,closure.index));
				
				
				set<LR_item>::iterator iter;
				for(iter=closure.closure_instance.begin();iter!=closure.closure_instance.end();iter++)
				{
					//Reduction action:
					LR_item item=LR_item(*iter);
					//If it is a reduction item
					if(item.get_r_element().isNull)
						LR_table.set(closure.index,item,action(action_type::reduction,produc_index_map[item]));
				}
				
				if(closure.size>0&&set_C.find(closure)==set_C.end())
				{
					

					set_C.insert(closure);
					isChanged=true;
				}
			}
		}
		if(isChanged==false)
			break;
	}
}