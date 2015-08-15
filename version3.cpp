#include <iostream>
#include <cstdio>
#include <cstring>
#include <set>
#include <map>
#include <queue>
#define maxn 10000
using namespace std;

struct Node
{
    char data;
    int point;
    Node* nex;
};
Node* Head[maxn] = {NULL};
Node* Head2[maxn] = {NULL};
int node_num;
struct Expression
{
    int error;
    int start;
    int end;
    Expression();
};


Expression::Expression():
error(0),
start(0),
end(0)
{
}



void print()
{
    for(int i=1;i<maxn;i++)
    {
        for(Node* j = Head[i]; j != NULL; j = j -> nex)
        {
            printf("%d %d %c\n",i,j->point,j->data);
        }
        Head[i] = NULL;
    }
    puts("");
}


void add(int x,int y,char v,Node** Head)
{
    Node* p = new Node;
    node_num++;
    p -> nex = Head[x];
    p -> point = y;
    p -> data = v;
    Head[x] = p;
}
void m_delete(Node*& node)
{
    node_num--;
    delete node;
}

void destroy(Node** Head)
{
    for(int i = 0; i < maxn; i++)
    {
        for(Node* j = Head[i]; j != NULL;)
        {

            Node* p = j -> nex;
            m_delete(j);
            j = p;
        }
    }
}

void del(Node*& head, Node*& no, Node*& pre)
{
    if(no == head)
    {
        Node* p = head->nex;
        m_delete(head);
        head = p;
        no = p;
        pre = no;
    }
    else
    {
        pre -> nex = no -> nex;
        delete no;
        no=pre -> nex;
    }
}



int valid[maxn],p[maxn],h=0,p_num = 0,visit[maxn],new_p=0,en[maxn],en2

[maxn];
char gang[10];



Expression GetTemp(char*& Stream);

void dfs(int start,int k)
{
    for(Node* i = Head[k]; i != NULL; i = i->nex)
    {
        int u = i -> point;
        if (visit[u])continue;
        if (i -> data == '@')
        {
            if(en[u])en[start] = 1;
            dfs(start,u);
        }
        else if (start != k)
        {
            add(start,u, i -> data,Head);
        }
    }
    visit[k] = 1;
}




void errase(int start,int end)
{
    valid[start] = 1;
    p[++h] = start;
    en[end] = 1;
    for(int i = 1; i <= h; i++)
    {
        memset(visit,0,sizeof(visit));
        dfs(p[i], p[i]);
    }
    for(int i = 1; i <= p_num; i++)
    {
        for(Node *j = Head[i], *pre = Head[i]; j != NULL;)
        {
            if (!valid[j -> point] || j -> data == '@' || !valid[i])
            {
                //printf("destroy %d %d %c\n",i,j -> point,j->data);
                del(Head[i],j,pre);
                //j = NULL;
            }
            else
            {
                pre = j;
                j = j -> nex;
            }
        }
    }
}

void Nfa_To_Dfa(int start)
{
    queue< set<int> > q;
    map<set<int>,int> set_set;
    set<int> fir;
    fir.insert(start);
    set_set[fir] = ++new_p;
    q.push(fir);
    int f[200] = {0};
    while(!q.empty())
    {
        set <int> fron = q.front();
        q.pop();
        set <int> ::iterator it;
        set <int> alg[200];
        for(it = fron.begin(); it != fron.end(); it++)
        {
            int v = *it;
            for (Node* i = Head[v]; i != NULL; i = i -> nex)
            {
                int u = i -> point;
                if(en[u])
                {
                    f[(int)i -> data] = 1;
                }
                alg[(int)i -> data].insert(u);
            }
        }
        int temp = set_set[fron];
        for(int i = 1; i <= 130; i++) if (!alg[i].empty())
        {
            if(set_set.find(alg[i]) == set_set.end())
            {
                set_set[alg[i]] = ++new_p;
                if(f[i])en2[new_p] = 1;
                q.push(alg[i]);
            }
            add(temp, set_set[alg[i]], (char)i, Head2);
        }
    }
}



bool Is(char*& Stream ,const char* str)
{
    char* Read = Stream;
    int str_len = strlen(str);
    while(*Read == ' ')Read++;
    if(!strncmp(Read,str,str_len))
    {
        Stream = Read + str_len;
        return true;
    }
    return false;
}

bool IsAlg(const char ch)
{
    if('a' <= ch && ch <= 'z')return true;
    if('A' <= ch && ch <= 'Z')return true;
    if('0' <= ch && ch <= '9')return true;
    return false;
}


Expression getExtern(char*& Stream)
{
    const char* gg = gang;
    char* Read = Stream ;
    Expression ret;
    if (Is(Read,"["))
    {
        char ww[] =

"(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)";
        char WW[] =

"(A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)";
        char num[] = "(0|1|2|3|4|5|6|7|8|9)";
        if(Is(Read,"a") && Is(Read,"-") && Is(Read,"z") && Is(Read,

"]"))
        {
            char* ip = ww;
            ret = GetTemp(ip);
        }
        else if(Is(Read,"A") && Is(Read,"-") && Is(Read,"Z") && Is

(Read, "]"))
        {
            char* ip = WW;
            ret = GetTemp(ip);
        }
        else if(Is(Read,"0") && Is(Read,"-") && Is(Read,"9") && Is

(Read,"]"))
        {
            char* ip = num;
            ret = GetTemp(ip);
        }
    }
    else if(Is(Read, gg))
    {
        //puts("in");
        char ww[] = "([0-9]|[a-z]|[A-Z])";
        char* ip = ww;
        ret = GetTemp(ip);
        return ret;
    }
    else
    {
        ret.error = 1;
        return ret;
    }
    Stream = Read;
    return ret;
}


Expression GetTerm(char*& Stream)
{
    char* Read = Stream ;
    Expression ret;
    while(*Read == ' ')Read++;
    if(IsAlg(*Read))
    {
        ret.start = ++p_num;
        ret.end = ++p_num;
        valid[ret.end] = 1;
        p[++h] = ret.end;
        add(ret.start,ret.end,*Read,Head);
        Read++;
    }
    else if(Is(Read , "("))
    {
        Expression factor = GetTemp(Read);
        if(factor.error)
        {
            ret.error =1;
        }
        else
        {
            if (Is(Read,")"))
            {
                ret = factor;
            }
            else ret.error = 1;
        }
    }
    else
    {
        Expression factor = getExtern(Read);
        if(factor.error)
        {
            ret.error =1;
        }
        else ret = factor;
    }
    if(Is(Read,"*"))
    {
        Expression ret2;
        ret2.start = ++p_num;
        ret2.end = ++p_num;
        add(ret2.end,ret.start,'@',Head);
        add(ret2.start,ret2.end,'@',Head);
        add(ret.end,ret2.end,'@',Head);
        ret = ret2;
    }
    Stream = Read;
    return ret;
}


Expression GetFactor(char*& Stream)
{
    char* Read = Stream;
    Expression ret = GetTerm(Read);
    if(ret.error)
    {
        ret.error = 1;
        return ret;
    }
    while(1)
    {
        Expression term = GetTerm(Read);
        if(!term.error)
        {
            add(ret.end,term.start,'@',Head);
            ret.end = term.end;
        }
        else
        {
            break;
        }
    }
    Stream = Read;
    return ret;
}

Expression GetTemp(char*& Stream)
{
    char* Read = Stream;
    Expression term = GetFactor(Read), ret;
    ret.start = ++p_num;
    ret.end = ++p_num;
    add(ret.start, term.start,'@',Head);
    add(term.end, ret.end, '@',Head);
    if(Is(Read,"|"))
    {
        while(1)
        {
            Expression temp = GetTemp(Read);
            if(!term.error)
            {
                add(ret.start, temp.start, '@',Head);
                add(temp.end, ret.end, '@',Head);
            }
            if(!Is(Read, "|"))break;
        }
    }
    Stream = Read;
    return ret;
}

Expression GetExp(char*& Stream)
{
    gang[0] = '\\';
    gang[1] = '\0';
    destroy(Head);
    destroy(Head2);
    memset(en,0,sizeof(en));
    memset(en2,0,sizeof(en2));
    memset(valid,0,sizeof(valid));
    new_p=0;
    h=0,p_num = 0;
    Expression ret = GetTemp(Stream);
    print();
    errase(ret.start,ret.end);
    Nfa_To_Dfa(ret.start);
    ret.start = 1;
    destroy(Head);
    memcpy(Head,Head2,sizeof(Head));
    memcpy(en,en2,sizeof(en));
    print();
    return ret;
}


bool match(Expression Re,char* Text)
{
    char* Read = Text;
    int p = Re.start;
    en[Re.end] = 1;
    for(;;)
    {
        int flag = 0;
        for(Node* i = Head[p]; i != NULL; i = i -> nex)
        {
            int u = i -> point;
            if (i -> data == *Read)
            {
                flag = 1;
                p = u;
                Read++;
                break;
            }
        }
        if(!(*Read))break;
        if (flag == 0)return false;
    }
    if(en[p])return true;else return false;
}









class Exp
{
public:
    Exp();
    Expression GetExp(char*& Stream);
    bool match(Expression Re,char* Text);

private:
    Node* Head[maxn];
    Node* Head2[maxn];
    void dfs(int start,int k);
    void errase(int start,int end);
    void Nfa_To_Dfa(int start);
    bool Is(char*& Stream ,const char* str);
    bool IsAlg(const char ch);
    Expression getExtern(char*& Stream);
    Expression GetTerm(char*& Stream);
    Expression GetFactor(char*& Stream);
};


int main()
{
    char ch[100],text[100];
    gets(ch);
    char *pp = ch;
    Expression u = GetExp(pp);
    printf("%d\n",node_num);
    while(1)
    {
        gets(text);
        if(match(u,text))puts("match");else puts("error");
        //break;
    }
    return 0;
}

