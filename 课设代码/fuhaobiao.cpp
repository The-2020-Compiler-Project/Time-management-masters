#include"标头.h"
enum Kind { I, C, K, P, Ch, St, error };/*二元式种类//style*/
static char *keyword[18] = { "int","float","char","void","if","else","switch","case","for","do","while","continue","break","default","sizeof","return" ,"cout","cin"};/*关键字表*/
static char *delimiters[18] = { ">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")" ,"[","]" };/*界符表*/
char var[16][15] = {};/*变量表variable*/
enum TYP{in,real,ch,b,error1};/*类型，包括int，float,char,bool型*/
enum CAT{f,c,t,d,v,vn,vf,error2};/*种类，包括f函数，c常量，t类型，d域名，v变量，vn换名形参，vf，赋值形参*/
enum ADDR{PFINFL,LENL,VALL,error3};/*地址表，包括函数表，活动记录表*/
int ID[16];/*记录标识符在代码中首次出现的位置//idlocate*/
int hg = 0;//hg

struct duality {/*二元式数据结构*/
    Kind kind;
    char value1[25];
    int value2;
};
duality Token[1000];/*词法序列（二元式结构）*/

struct SYNBL {/*符号表*/
    char name[15];
    TYP type;
    CAT kind;
    ADDR addr;
};

struct pfinfl {/*函数表*/
    int level;
    int off;
    int fn;
    SYNBL para[5];/*参数表*/
    int entry;
};

struct vall {/*活动记录表，采用链表结构*/
    char name[15];
    char nextname[15];//name1
    int low;
    int up;
    struct vall *next;
};
vall *headnode=new vall;//firstnode

struct lenl {/*长度表*/
    char name[10];
    int length;
};
lenl length1[10];//lengt

void INIT_lenl(lenl*length1) {//lengt
    int i = 0;
    while (i < 10) {
        length1[i].length = -1;
        strcpy(length1[i++].name, "#");
    }
    length1[0].length = 4;
    strcpy(length1[0].name, "int");
    length1[1].length = 4;
    strcpy(length1[1].name, "float");
    length1[2].length = 1;
    strcpy(length1[2].name, "char");
    return;
}

void ADD_vall(vall* head, int t1, int t2, char*id,char*id1) {/*添加活动记录*/
    vall *a = new vall;
    a->low = t1;
    a->up = t2;
    strcpy(a->name, id);
    strcpy(a->nextname, id1);
    a->next = NULL;
    vall *b = head;
    while (b->next != NULL)b = b->next;
    b->next = a;
    return;
}

void INIT_vall(vall* head) {/*初始化活动记录*/
    head->low = 0;
    head->up = 0;
    strcpy(head->nextname, "");
    strcpy(head->name, "old sp");
    head->next = NULL;
    ADD_vall(head, 1, 1, "返回地址","");
    ADD_vall(head, 2, 2, "全局display","");
    return;
}

void PRINT_vall(vall*head) {/*打印活动记录表*/
    vall *p = head;
    char s[] = "vall.dat";
    std::fstream ADD_vall(s, ios::out | ios::in);/*打开活动记录文件*/
    cout << "活动记录表如下:" << endl << "名字"<<'\t'<<"        "<<"标识符"<<'\t'<<"      "<<"下界存储"<<'\t'<<"上界存储" << endl;
    while (p!= NULL) {
        cout << p->name;
        ADD_vall << p->name<<endl;
        int c = strlen(p->name);
        while (c < 16) {
            cout << " ";
            c++;
        }
        cout << p->nextname << '\t' << "      ";
        ADD_vall << p->nextname << endl;
        cout << p->low << "      " << '\t';
        ADD_vall << p->low << endl;
        cout << p->up  << endl;
        ADD_vall << p->up << endl;
        p = p->next;
    }
    ADD_vall.close();
    return;
}

SYNBL symbol[100];/*符号表//envirnment*/

void INIT_SYNBL(SYNBL*symbol,int length) {
    int i = 0;
    while (i<length) {
        strcpy(symbol[i].name, "#");
        symbol[i].type = error1;
        symbol[i].kind = error2;
        symbol[i++].addr = error3;
    }
    return;
}

int getlen(SYNBL k) {
    switch (k.type) {
    case in:return 4;
    case real:return 4;
    case ch:return 1;
    case b:return 1;
    }
    return 0;
}

void Finish_fun(pfinfl*fun) {
    INIT_SYNBL(fun->para, 5);
    fun->fn = 0;
    fun->level = 0;
    fun->off = 3;
    fun->entry = -1;
    int c = 0;
    if (Token[ID[0] + 1].kind != P) {
        cout << "函数有问题...";
        getchar();
        exit(1);
    }
    int end=0;
    for (int i = ID[0] + 2;; i++) {
        if ((Token[i].kind == P) && (Token[i].value2 == 16)) {
            break;
            end = i;
        }
        else if ((Token[i].kind == P) && (Token[i].value2 == 13))continue;
        fun->fn++;
        strcpy(fun->para[c].name, Token[i + 1].value1);
        switch (Token[i].value2) {
        case 1:fun->para[c].type = in; break;
        case 2:fun->para[c].type = real; break;
        case 3:fun->para[c].type = ch; break;
        }
        fun->para[c++].kind = v;
        i++;
    }
    c = 0;
    char add[10];
    _itoa_s(fun->fn, add, 10);
    ADD_vall(headnode, 3, 3, "参数个数", add);
    int flag = 4;//st
    for (int i = fun->fn; i > 0; i--) {
        if (i == fun->fn) {
            ADD_vall(headnode, flag, flag - 1 + getlen(fun->para[c]), "形式单元", fun->para[c].name);
            flag = flag - 1 + getlen(fun->para[c]);
            continue;
        }
        ADD_vall(headnode, flag, flag - 1 + getlen(fun->para[c]), "", fun->para[c].name);
        flag = flag + getlen(fun->para[c]);
    }
    ADD_vall(headnode, flag, flag++, "display表", "");
    for (int i = 0;; i++) {
        if (ID[i] > end) {
            end = i;
            break;
        }
        if (ID[i] == -1)return;
    }
    for (int i = end;; i++) {
        if (ID[i] == -1)break;
        int ge;
        switch (Token[ID[i]-1].value2) {
        case 1:
        case 2:ge = 4; break;
        case 3:ge = 1;
        }
        if (i == end) {
            ADD_vall(headnode, flag,flag+ge-1 , "局部变量", Token[ID[i]].value1);
            flag = flag + ge - 1;
            continue;
        }
        ADD_vall(headnode, flag, flag + ge - 1, "", Token[ID[i]].value1);
        flag = flag + ge;
    }
    return;
}

static void INIT_duality(duality*token) {/*初始化二元式，同时将变量表初始化*/
    for (int i = 0; i < 1000; i++) {
        token[i].kind = error;
        strcpy(token[i].value1, "#");
        token[i].value2 = 0;
    }
    for (int i = 0; i < 16; i++) strcpy(var[i], "#");
    return;
}

void ADD_Identifier (char id[16][15],char*cmp,int flag) {/*标识符表元素添加*/
    int i = 0;
    for (;; i++) {
        if (strcmp(id[i], "#") == 0)break;
        if (strcmp(id[i], cmp) == 0)return;
    }
    strcpy(id[i], cmp);
    ID[hg++] = flag;
    return;
}

void Finsh_SYNBL() {/*完成符号表*/
    for (int g = 0;; g++) {
        if (strcmp(var[g], "#") == 0)break;
        if (g == 0)symbol[g].kind = f;
        else symbol[g].kind = v;
        strcpy(symbol[g].name, var[g]);
        if (strcmp(keyword[Token[ID[g] - 1].value2-1],"int")==0) {
            symbol[g].type = in;
        }
        else if (strcmp(keyword[Token[ID[g] - 1].value2 - 1], "float") == 0) {
            symbol[g].type = real;
        }
        else if (strcmp(keyword[Token[ID[g] - 1].value2 - 1], "char") == 0){
            symbol[g].type = ch;
        }
        else if (strcmp(keyword[Token[ID[g] - 1].value2 - 1], "bool") == 0){
            symbol[g].type = b;
        }
        else {
            cout << "变量定义有问题...";
            getchar();
            exit(1);
        }
    }
    return;
}

void Write_SYNBL(SYNBL*out,int flag) {
    if (flag == 1) {
        char s1[] = "symboltable.dat";
        std::fstream symboltable(s1, ios::out | ios::in);/*打开代码文件*/
        for (int g = 0;; g++) {
            if (out[g].type == error1)break;
            cout << out[g].name;
            symboltable << out[g].name << endl;
            int j = strlen(out[g].name);
            while (j < 10) {
                cout << " ";
                j++;
            }
            switch (out[g].type) {
            case in:cout << "int        "; symboltable << "int" << endl; break;
            case real:cout << "float      "; symboltable << "float" << endl;  break;
            case ch:cout << "char       "; symboltable << "char" << endl; break;
            }
            switch (out[g].kind) {
            case f:cout << "function  "; symboltable << "function" << endl; break;
            case v:cout << "var       "; symboltable << "var" << endl; break;
            }
            switch (out[g].addr) {
            case PFINFL:cout << "PFINFL    "; symboltable << "PFINFL" << endl; break;
            case LENL:cout << "LENL      "; symboltable << "LENL" << endl; break;
            case VALL:cout << "VALL      "; symboltable << "VALL" << endl;  break;
            default:cout << "      "; symboltable << " " << endl;
            }
            cout << endl;
        }
        symboltable.close();
    }
    else {
        for (int g = 0;; g++) {
            if (out[g].type == error1)break;
            cout << out[g].name;
            int j = strlen(out[g].name);
            while (j < 10) {
                cout << " ";
                j++;
            }
            switch (out[g].type) {
            case in:cout << "int        ";break;
            case real:cout << "float      ";   break;
            case ch:cout << "char       ";  break;
            }
            switch (out[g].kind) {
            case f:cout << "function  "; break;
            case v:cout << "var       ";  break;
            }
            switch (out[g].addr) {
            case PFINFL:cout << "PFINFL    "; break;
            case LENL:cout << "LENL      "; break;
            case VALL:cout << "VALL      ";  break;
            }
            cout << endl;
        }
    }
}

void PRINT_SYNBL(pfinfl*function) {/*打印符号表、函数表、活动记录表、长度表*/
    cout << "符号表组织如下" << endl;
    cout << "名字      类型       种类        地址" << endl;
    Write_SYNBL(symbol,1);
    cout << "长度表如下:" << endl<<"名字"<<'\t'<<"长度"<<endl;
    for (int g = 0;; g++) {
        if (length1[g].length == -1)break;
        cout << length1[g].name << '\t';
        cout << length1[g].length << endl;
    }
    cout << "函数表如下(包含参数表）:" << endl;
    cout << "层次号    区距离     参数个数    入口地址" << endl;
    cout << function->level << "         " << function->off << "          " << function->fn << "           " << "未定义" << endl;
    cout << "名字      类型       种类        地址" << endl;
    Write_SYNBL(function->para,0);
    PRINT_vall(headnode);
    return;
}

void Begin_SYNBL() {
    pfinfl *func=new pfinfl;
    char sequence[] = "wordsequence.dat";
    for (int i = 0; i < 16; i++) {/*初始化标识符位置记录*/
        ID[i] = -1;
    }
    std::fstream outfile(sequence, ios::out | ios::in);/*打开代码文件*/
    int i = 0;
    if (!outfile) {
        cout << "open error!";
        getchar();
        getchar();
        exit(1);
    }
    INIT_duality(Token);
    while (!outfile.eof()) {
        int sty;
        outfile >> sty;
        Token[i].kind = (Kind)sty;
        switch (Token[i].kind) {
        case P:
        case K:outfile >> Token[i++].value2; break;
        case I: {
            outfile >> Token[i++].value1;
            ADD_Identifier(var, Token[i-1].value1,i-1);
            break;
        }
        case Ch:
        case St:
        case C:outfile >> Token[i++].value1; break;
        }
    }
    Token[i - 1].kind = error;
    INIT_SYNBL(symbol,100);
    INIT_lenl(length1);
    INIT_vall(headnode);
    Finsh_SYNBL();
    Finish_fun(func);
    PRINT_SYNBL(func);
    cout << "符号表填写完成，按下任意键以进入中间代码翻译";
    getchar();
    translate1();
    return;
}
