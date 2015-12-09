#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<set>
using namespace std;
#define FILENAME "test.txt"
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
    bool operator<(const element& elem)const
    {
        return this->index<elem.index;
    }
    bool operator>(const element& elem)const
    {
        return this->index>elem.index;
    }
	void clear()
	{
		index=-1;
		isNull=true;
	}
	
	
};

element edi_elem;

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
    string& toString()
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
        vector<element>::iterator it;
        for(it=r_part.begin();it<r_part.end();it++)
        {
            edi_str+=it->toString();
            if(it!=r_part.end()-1)
                edi_str+=string(" ");
        }

        return edi_str;

    }


};

set<element> edi_elem_set;

vector<production> produc_set;//产生式集合
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
	 vector<string>& find(vector<string>& beita_a)
   { //求first(beita a),其中beita=（V|T）*，a属于T
    //返回first集，vector中string均属于T
	vector<string> first;
	set<element>::iterator it;
	vector<production>::iterator pro_it;
	bool epsilon=false;
	bool exist=false;
	for(int i=0;i<beita_a.size();i++)
	{
		exist=false;
		if(ter_list.find(beita_a[i])==ter_list.end())//beita_a[i]是变元
		{		
			for(it=first_sets.find(beita_a[i]).begin();it!=first_sets.find(beita_a[i]).end();it++)
				if(it->toString().compare(EPSILON)!=0)
				{
					for(int j=0;j<first.size();j++)
						if(first[j].compare(it->toString())==0)
						{//已存在，不需加入
							exist=true;
							break;
						}
					if(!exist)
			           first.push_back(it->toString());//将beita_a[i]的first集加入,不加入空,不重复加入
				}
			for(pro_it=produc_set.begin();pro_it<produc_set.end();pro_it++)
			{//遍历产生式集合，看beita_a[i]是否能产生空
				if(pro_it->l_part.toString().compare(beita_a[i])==0)//找到beita_a[i]的产生式
				   if(pro_it->isWithEPSILON)
				   {
					   epsilon=true;
					   break;
				   }
			}
			if(!epsilon)//beita_a[i]不含空产生式
				break;
		}
		else//beita_a[i]是终结符，加入first集，注意#也是终结符
		{
			for(int j=0;j<first.size();j++)
				if(first[j].compare(beita_a[i])==0)
				{//已存在，不需加入
					exist=true;
					break;
				}
			if(!exist)
			    first.push_back(beita_a[i]);
			break;
		}
	}
	return first;
    }
   set<element>& find(vector<element>& beita_a)
   { //求first(beita a),其中beita=（V|T）*，a属于T
    //返回first集
	set<element> first;
	set<element>::iterator it;
	vector<production>::iterator pro_it;
	bool epsilon=false;
	bool exist=false;
	for(int i=0;i<beita_a.size();i++)
	{
		exist=false;
		if(ter_list.find(beita_a[i].toString())==ter_list.end())//beita_a[i]是变元
		{		
			for(it=first_sets.find(beita_a[i].toString()).begin();it!=first_sets.find(beita_a[i].toString()).end();it++)
				if(it->toString().compare(EPSILON)!=0)
				{
					
					if(first.find(element(false,ter_list.find(it->toString())->second))!=first.end())
					{//已存在，不需加入
						exist=true;
						break;
					}
					if(!exist)
			           first.insert(element(false,ter_list.find(it->toString())->second));//将beita_a[i]的first集加入,不加入空,不重复加入
				}
			for(pro_it=produc_set.begin();pro_it<produc_set.end();pro_it++)
			{//遍历产生式集合，看beita_a[i]是否能产生空
				if(pro_it->l_part.toString().compare(beita_a[i].toString())==0)//找到beita_a[i]的产生式
				   if(pro_it->isWithEPSILON)
				   {
					   epsilon=true;
					   break;
				   }
			}
			if(!epsilon)//beita_a[i]不含空产生式
				break;
		}
		else//beita_a[i]是终结符，加入first集，注意#也是终结符
		{
			if(first.find(element(false,beita_a[i].index))!=first.end())
			{//已存在，不需加入
				exist=true;
				break;
			}
			if(!exist)
				first.insert(element(false,beita_a[i].index));
			break;
		}
	}
	return first;
    }
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
    bool ptr_r_shift()
    {
		if(ptr_pos==production::r_part_size-1)
			return false;
		else
			ptr_pos++;
    }
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
};

//LR_item closure for LR(1)
class LR_item_closure
{
protected:
	static int index_generator;
public:
	set<LR_item> closure_instance;
	int size;
	int index;
	
	bool insert(const LR_item& item)
	{
		closure_instance.insert(item);
		size++;
	}
	LR_item_closure()
	{
		size=0;
		index=index_generator;
		index_generator++;
	}
	LR_item_closure(const LR_item_closure& closure)
	{
		set<LR_item>::const_iterator it;
		for(it=closure.closure_instance.begin();it!=closure.closure_instance.end();it++)
			insert((*it));
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
				element B=it->get_r_element;
				if(B.isNull)//the dot isn't at the rightest
				{		
					if(B.isVar)//the right of the dot is a V    ;A->a.B……
					{
						for(int i=0;i<produc_set.size();i++)
						{
							if(produc_set[i].l_part.index==B.index&&produc_set[i].l_part.isVar==B.isVar)//it->get_r_element() production ;B->……
							{
								vector<element> beita_a;
								for(int j=it->ptr_pos+2;j<it->r_part_size;j++)//A->a.Bbeita,a, put beita to vector<element> beita_a
									beita_a.push_back(element(it->r_part[j].isVar,it->r_part[j].index));
								beita_a.push_back(element(false,it->index));//A->a.Bbeita,a, put a to vector<element> beita_a
								set<element> first_beita_a=first_sets.find(beita_a);
								set<element>::iterator first_it;
								for(first_it=first_beita_a.begin();first_it!=first_beita_a.end();first_it++)//first(beita_a)
									isChanged=insert(LR_item(-1,produc_set[i],false,first_it->index));
							}
						}
					}
				}
			}
			if(isChanged==false)//until not change
				break;
		}		
	}
};
int LR_item_closure::index_generator=0;





/*
class flex_production:public production
{
public:
    int ptr_pos;
    bool ptr_r_shift()
    {
		if(ptr_pos+1==production::r_part_size)
			return false;
		else
			ptr_pos++;
    }
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
		if()
		
	}
	element& get_r_element()
	{
	
	}

};
class LR_item:public flex_production,public element
{
public:
    LR_item()
    {

    }
    LR_item(const LR_item& item)
    {

    }
};
class LR_item_closure
{
public:
	set<LR_item> closure_instance;
	int size;
	void insert(LR_item& item)
	{
	}
	LR_item_closure()
	{}
	LR_item_closure(const LR_item_closure& closure)
	{}

};

*/

void main()
{
	int i=1;
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
		ter_list.insert(pair<string,int>("#",i));//最后#也插入终结符表				   
		re_ter_list[i]="#";
		produc_set.push_back(product);//构造produc_set
	}    
	}
	fclose(f);
	init_first_sets(var_list);
	first_sets.print();

	/*vector<string> a;
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
		if(isChanged==false)
			break;
	}

}
  