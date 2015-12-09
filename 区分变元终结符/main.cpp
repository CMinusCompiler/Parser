#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<set>
#include<list>
#include<queue>
using namespace std;
#define FILENAME "wenfa.txt"
#define MAXROW 100
#define EPSILON "��"
string var;//��Ԫ
map<string,int> var_list;//��Ԫ��
map<string,int> ter_list;//�ս����
map<int,string> re_var_list;//��Ԫ��
map<int,string> re_ter_list;//�ս����
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
	bool operator==(const element& elem)const
    {
        return this->index==elem.index;
    }
	bool operator!=(const element& elem)const
    {
        return this->index!=elem.index;
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
	bool operator<(const production& produc)const
    {
		if(l_part==l_part)
		{
			vector<element>::const_iterator it=r_part.begin();	
			vector<element>::const_iterator _it=produc.r_part.begin();
			for(;it<r_part.end()&&_it<produc.r_part.end();it++,_it++)
				if((*it)!=(*_it))
					return (*it)<(*_it);
			return false;
		}
		else
			return l_part<produc.l_part;
        
    }
    bool operator>(const production& produc)const
    {
		if(l_part==l_part)
		{
			vector<element>::const_iterator it=r_part.begin();	
			vector<element>::const_iterator _it=produc.r_part.begin();
			for(;it<r_part.end()&&_it<produc.r_part.end();it++,_it++)
				if((*it)!=(*_it))
					return (*it)>(*_it);
			return false;
		}
		else
			return l_part>produc.l_part;

    }
	bool operator==(const production& produc)const
	{
		if(l_part==l_part)
		{
			vector<element>::const_iterator it=r_part.begin();	
			vector<element>::const_iterator _it=produc.r_part.begin();
			for(;it<r_part.end()&&_it<produc.r_part.end();it++,_it++)
				if((*it)!=(*_it))
					return false;
			return true;
		}
		else
			return false;
	}
	bool operator!=(const production& produc)const
	{
		if(l_part==l_part)
		{
			vector<element>::const_iterator it=r_part.begin();	
			vector<element>::const_iterator _it=produc.r_part.begin();
			for(;it<r_part.end()&&_it<produc.r_part.end();it++,_it++)
				if((*it)!=(*_it))
					return true;
			return false;
		}
		else
			return true;
	}
};

set<element> edi_elem_set;

vector<production> produc_set;//����ʽ����
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

	bool operator>(const flex_production& produc)const
	{
		if(production::operator==(produc))
		{
			return this->ptr_pos>produc.ptr_pos;
		}
		else
			return production::operator>(produc);
	}
	bool operator<(const flex_production& produc)const
	{
		if(production::operator==(produc))
		{
			return this->ptr_pos<produc.ptr_pos;
		}
		else
			return production::operator<(produc);
	}
	bool operator==(const flex_production& produc)const
	{
		if(production::operator==(produc))
		{
			return this->ptr_pos==produc.ptr_pos;
		}
		else
			return false;
	}
	bool operator!=(const flex_production& produc)const
	{
		if(production::operator==(produc))
		{
			return this->ptr_pos==produc.ptr_pos;
		}
		else
			return true;
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
	bool operator<(const LR_item& produc)const
	{
		if(element::operator==(produc))
			return flex_production::operator<(produc);
		else
			return element::operator<(produc);
	}
	bool operator>(const LR_item& produc)const
	{
		if(element::operator==(produc))
			return flex_production::operator>(produc);
		else
			return element::operator>(produc);
	}

};

//LR_item closure for LR(1)
class LR_item_closure
{
private:
	static int index_generator;
	int index;
public:
	set<LR_item> closure_instance;
	int size;
	
	//Return: successfully done or not.
	bool insert(const LR_item& item)
	{
		if(closure_instance.find(item)!=closure_instance.end())
			return false;
		
		closure_instance.insert(item);
		size++;
		return true;
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
	int get_index()
	{
		return index;
	}
};
int LR_item_closure::index_generator=0;

class LR1FA_node:public LR_item_closure
{
private:
	//Used to describe the element on the transmition arrow.
	//Should not be in the FA_vertex_node, so "private" is confined.
	element trans_condition;
	static int index_generator;
	int index;
public:
	
	LR1FA_node():LR_item_closure()
	{
		index=index_generator;
		index_generator++;
	}
	LR1FA_node(const LR1FA_node& node):LR_item_closure(node)
	{
		trans_condition=element(node.trans_condition);
		index=node.index;
	}
	void set_trans_condition(element& elem)
	{
		trans_condition=element(elem);
	}
	void set_trans_condition(bool isVar,int index)
	{
		trans_condition.isVar=isVar;
		trans_condition.index=index;
	}
	element& get_trans_condition()
	{
		return trans_condition;
	}
	int get_index()
	{
		return index;
	}


};
int LR1FA_node::index_generator=0;

class LR1FA_vertex_node:public LR1FA_node
{
private:
	static int index_generator;
	int index;
public:
	list<LR1FA_node> adj_list;
	int adj_list_length;
	LR1FA_vertex_node():LR1FA_node()
	{
		adj_list_length=0;
		index=index_generator;
		index_generator++;
	}
	LR1FA_vertex_node(const LR1FA_vertex_node& node):LR1FA_node(node)
	{
		list<LR1FA_node>::const_iterator it;
		for(it=node.adj_list.begin();it!=node.adj_list.end();it++)
		{
			push_back(*it);
		}
		adj_list_length=node.adj_list_length;
		index=node.index;

	}
	void push_back(const LR1FA_node& node)
	{
		adj_list.push_back(node);
		adj_list_length++;
	}
	int get_index()
	{
		return index;
	}

};
int LR1FA_vertex_node::index_generator=0;

class LR1FA_graph
{
//How to create a LR1FA_graph:
//First: create all the vertex nodes, and push them back.
//Second: create all the common nodes, and push them back to the corresponding adjcent list.
public:
	int vertex_node_num;
	vector<LR1FA_vertex_node> adj_list_array;
	LR1FA_graph()
	{
		vertex_node_num=0;
	}
	LR1FA_graph(const LR1FA_graph& graph)
	{
		vector<LR1FA_vertex_node>::const_iterator it;
		for(it=graph.adj_list_array.begin();it<graph.adj_list_array.end();it++)
		{
			push_back(*it);
		}
	}

	//Push back vertex node to the adjcent list array.
	void push_back(const LR1FA_vertex_node& node)
	{
		adj_list_array.push_back(node);
		vertex_node_num++;
	}

	//Push back a node to selected adjcent list.
	bool push_back(int index,const LR1FA_node& node)
	{
		if(index<adj_list_array.size())
		{
			adj_list_array[index].push_back(node);
			return true;
		}
		else
			return false;
	}

	LR1FA_vertex_node& operator[](int index)
	{
		return adj_list_array[index];
	}
	
	void BFS(void (*operation)(LR1FA_graph& graph,const LR1FA_node& node),LR1FA_graph& graph)
	{
		queue<LR1FA_vertex_node> visit_queue;
		visit_queue.push(graph[0]);

		while(!visit_queue.empty())
		{
			//Visit the node.
			operation(graph,visit_queue.front());
			

			//Push the adjacent nodes.
			list<LR1FA_node>::iterator it;
			for(it=visit_queue.front().adj_list.begin();it!=visit_queue.front().adj_list.end();it++)
				visit_queue.push(adj_list_array[it->get_index()]);
			
			//Pop the parent node.
			visit_queue.pop();

		}


	}

};



void main()
{


	int i=1;
	string s=" ";
	string str;
	string ter_var;//����ʽ�ұߵĴ�
	vector<string> var_ter_vec;//����ʽ�Ҳ��Ļ���
	
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
	pos=str.find('��');
	if(pos>0)
	{
		var=str.substr(0,pos-1);
		if((var_list.find(var))==var_list.end())//û�ҵ�
		{
			var_list.insert(pair<string,int>(var,i));//�����Ԫ��
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
	pos=str.find('��');
	if(pos>0)
	{
		production product;
		product.set_l_part(true,var_list.find(str.substr(0,pos-1))->second);
		//ter_var=str.substr(pos+1,str.length()-pos-2);//����ʽ�Ҳ�
		ter_var=str.substr(pos+1,str.length()-pos-1);//����ʽ�Ҳ�
		split(ter_var,s,&var_ter_vec);
		for(int j=0;j<var_ter_vec.size();j++)
		{
			if((var_list.find(var_ter_vec[j]))==var_list.end())//���Ǳ�Ԫ����Ϊ�ս��
			{

				if(ter_list.find(var_ter_vec[j])==ter_list.end())
				{
				   product.insert_elem(element(false,i));//����ʽ�����Ҳ��ս��
				   ter_list.insert(pair<string,int>(var_ter_vec[j],i));//�����ս����				   
				   re_ter_list[i]=var_ter_vec[j];//re_ter_list.insert(pair<int,string>(i,var_ter_vec[j]));
				   i++;
				}
				else
					product.insert_elem(element(false,ter_list.find(var_ter_vec[j])->second));//����ʽ�����Ҳ��ս��
			}
			else//��Ԫ
			{
				
				product.insert_elem(element(true,(var_list.find(var_ter_vec[j]))->second));//����ʽ�����Ҳ���Ԫ
			}
		}	
		produc_set.push_back(product);//����produc_set
	}    
	}
	fclose(f);
	init_first_sets(var_list);
	first_sets.print();
	/*cout<<"��Ԫ��"<<endl;
	for(it=var_list.begin();it!=var_list.end();it++)
	{
		cout<<it->first<<"===="<<it->second<<endl;
	}
	cout<<"�ս����"<<endl;
	for(it=ter_list.begin();it!=ter_list.end();it++)
	{
		cout<<it->first<<"===="<<it->second<<endl;
	}
	*/
	system("pause");

}
void split(std::string& s, std::string& delim,std::vector< std::string >* ret)  
{  //��s����delim���ִ浽ret��
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
{//�����б�Ԫ��first��
	bool isChanged=false;
	map<string,int>::iterator it;
	set<element>::iterator set_it;

	while(1)
	{
		isChanged =false;
		for(it=var_list.begin();it!=var_list.end();it++)
		{//������Ԫ��
			 for(int i=0;i<produc_set.size();i++)
			 {//��������ʽ����
				 if((it->first.compare(produc_set[i].l_part.toString()))==0)//�ҵ��ñ�ԪX�Ĳ���ʽ
				 {
					if(!produc_set[i].r_part[0].isVar)//X->a,ע���Ҳ���ս��
						isChanged|=first_sets.insert(it->first,produc_set[i].r_part[0]);//��a����first(X)						
					else//X->Y
					{
						for(int k=0;k<produc_set[i].r_part.size();k++)
						{//��Y1-YN����
							if(!(var_list.find(produc_set[i].r_part[k].toString())==var_list.end()))
							{//Yk�Ǳ�Ԫ							
								set<element> first_yk=first_sets.find(produc_set[i].r_part[k].toString());//first(Yk)
								for(set_it=first_yk.begin();set_it!=first_yk.end();set_it++)//��first(Yk)�����зǿ�Ԫ�ؼ���first(X)
									if((*set_it).toString().compare(EPSILON)!=0)	
										isChanged|=first_sets.insert(it->first,*set_it);
								if(first_sets.find(produc_set[i].r_part[k].toString()).find(element(false,ter_list.find(EPSILON)->second))!=(first_sets.find(produc_set[i].r_part[k].toString())).end())
								{//���first(Yk)�п�
									if(k==produc_set[i].r_part.size()-1)
									{									
										isChanged|=first_sets.insert(it->first,element(false,ter_list.find(EPSILON)->second));//�����
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