#include"��ͷ.h"
#include"windows.h"

//��������
/*void empty();//�ļ����
void database_interface();//���ݿ�ӿ�
void init_duality(duality* tok);//��ʼ����Ԫ�飬token
void init_action_seq();//��ʼ����������
bool traverse(char tra[100][15], char cmp[15]);//�����ս���ͷ��ս�����鿴�Ƿ����Ҫ����Ԫ��
void init_table();//��ʼ��������
bool special(char cmp[10]);
int table_prepare(char tra[100][15], char cmp[15]);//���׼��
int table_get(char tra[100][15], char cmp[15]);//ͬ��
void init_product();//��ʼ������ʽ
int special1(char cmp[10]);
void fill_v(int cmp);
void lookup_first(int i);//��first��
void first(int x1, int i, int local);//��first��
void follow(int i, int x1, int local);//��follow��
void get_select(int n);//�ж���first��follow
void finish_table();//��������
void print_table();//���������
void init_stack(PSTACK begin);//��ʼ������ջ
void push(PSTACK out, char* in);//ѹջ
bool empty(PSTACK out);//�ж�ջ�Ƿ�Ϊ��
char* pop(PSTACK out);//ȡջ��Ԫ��
void print_stack_out(PSTACK out);//��ӡ����ջ��ջ
int print_Syntx_match(duality gg);//��ӡ�﷨ƥ��
void free_pop(PSTACK out);//��ջ
void analysis(int m, int n, PSTACK out);//����ջƥ�䣬�鿴������
void parsing(PSTACK begin);//�������
bool val(char cmp[15]);//�Ƿ�Ϊ���
void brief();*/
void follow(int i, int x1, int local);//��follow��
char production[100][200];
int table[100][100];
char row[100][15];
char line[100][15];
char action_seq[1000][15];
int y = 0;
int x;
int v[10];
enum style { I, C, K, P, Ch, St, default };
enum type { vnk, vtk, nothing, def };
//�ؼ��ֱ�
char *keyword[18] = { "int","float","char","void","if","else","switch","case","for","do","while","continue","break","default","sizeof","return","cout","cin" };
//�����
char *delimiters[18] = { ">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")" ,"[","]" };
int gng;

struct productnode {//����ʽ�ڵ�
	type it;
	char symbol[15];
};

struct product {//����ʽ���ݽṹ
	char vn[15];
	productnode itself[10];
};

product c[100];

struct duality {//��Ԫʽ���ݽṹ
	style kind;
	char value1[25];
	int value2;
};

duality TOKEN[1000];//�ʷ����У���Ԫʽ�ṹ��

typedef struct Node//ջ��Ԫ�ؽڵ�
{
	char element[15];
	struct Node* pNext;
}NODE, * PNODE;

typedef struct Stack//��ջ������ջ����ջ��
{
	PNODE pTop;
	PNODE pBottom;
}STACK, * PSTACK;

void database_interface() {//���ݿ�ӿ�
	CoInitialize(NULL);
	_ConnectionPtr pConn(__uuidof(Connection));
	_RecordsetPtr pRst(__uuidof(Recordset));
	_bstr_t strConnect = "uid=;pwd=;DRIVER={Microsoft Access Driver (*.accdb)};DBQ=C:\trydb\try2.accdb;";
	try {
		pConn->Open("strConnect", "", "", adModeUnknown);
		pRst = pConn->Execute("select * from aaa", NULL, adCmdText);
	}
	catch (_com_error e)
	{
		cout << "ʧ��";
	}
	pRst->MoveFirst();
	_variant_t RecordsAffected;
	pConn->Execute("Insert into table(c1)Values (value1,value2)", &RecordsAffected, adCmdText);
}

void empty() {//�ڿ�ʼ����ǰ�������ļ���������ؽ�
	char r1[] = "word_sequence.dat";
	char r2[] = "action.dat";
	char r3[] = "optimize.dat";
	char r4[] = "vall.dat";
	char r5[] = "symbol_table.dat";
	char r6[] = "four_elem.dat";
	char r7[] = "four_element.dat";
	char r8[] = "Begin_SYNBL.dat";
	char r9[] = "func.dat";
	char r10[] = "target.asm";
	char r11[] = "end.dat";
	remove(r1);
	remove(r2);
	remove(r3);
	remove(r4);
	remove(r5);
	remove(r6);
	remove(r7);
	remove(r8);
	remove(r9);
	remove(r10);
	remove(r11);
	ofstream p1(r1);
	ofstream p2(r2);
	ofstream p3(r3);
	ofstream p4(r4);
	ofstream p5(r5);
	ofstream p6(r6);
	ofstream p7(r7);
	ofstream p8(r8);
	ofstream p9(r9);
	ofstream p10(r10);
	ofstream p11(r11);
}

void init_duality(duality*tok) {//��ʼ����Ԫʽ��ͬʱ���������ʼ��
	for (int i = 0; i < 1000; i++) {
		tok[i].kind = default;
		strcpy_s(tok[i].value1, "#");
		tok[i].value2 = 0;
	}
	return;
}

void init_action_seq() {//��ʼ����������
	for (int i = 0; i < 1000; i++) {
		strcpy_s(action_seq[i], "#");
	}
}

bool traverse(char tra[100][15], char cmp[15]) {//�����ս���ͷ��ս�����鿴�Ƿ����Ҫ����Ԫ��
	int i = 0;
	while (strcmp(tra[i],"#")!=0) {
		if (strcmp(tra[i++], cmp) == 0) {
			return false;
		}
		if (strcmp("$", cmp) == 0) {
			return false;
		}
	}
	return true;
}

void init_table() {//��ʼ��������
	int hang;
	char cmp[15];
	for (int i = 0; i < 100; i++) {
		row[i][0] = '#';
		line[i][0] = '#';
	}
	for (int i = 0;; i++) {
		if (strcmp(production[i], "#") == 0) {
			break;
			hang = i;
		}
		int j = 0;
		while (production[i][j++] != '%') {
		}
		strncpy_s(row[i], production[i], j - 1);
	}
	for (int i = 0;; i++) {
		if (strcmp(production[i], "#") == 0) {
			break;
			hang = i;
		}
		int j = 0;
		while (production[i][j++] != '%') {
		}
		while (production[i][j] != '#') {
			int length = 0;
			int length1 = j;
			while ((production[i][j] != '&') && (production[i][j] != '|')) {
				j++;
				length++;
			}
			strncpy_s(cmp, production[i] + j - length, length);
			j++;
			if (traverse(line, cmp) && traverse(row, cmp)) {
				strcpy_s(line[y++], cmp);
			}
		}
		x = i;
	}
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			table[i][j] = -1;
		}
	}
	return;
}

bool special(char cmp[10]) {
	int i = 0;
	while (strcmp(row[i], "#") != 0) {
		if (strcmp(row[i++], cmp) == 0) {
			return false;
		}
		if (strcmp("$", cmp) == 0) {
			return false;
		}
	}
	return true;
}

int table_prepare(char tra[100][15], char cmp[15]) {//׼�����
	int i = 0;
	while (strcmp(tra[i], "#") != 0) {
		if (strcmp(tra[i++], cmp) == 0)break;
	}
	i--;
	if (strcmp(tra[i], "#") == 0){
		cout << "�﷨������...";
		getchar();
		exit(0);
	}
	return (i);
}

int table_get(char tra[100][15], char cmp[15]) {//׼�����
	int i = 0;
	if (strcmp(cmp, "#") == 0) {
		return y;
	}
	else while (strcmp(tra[i], "#") != 0) {
		if (strcmp(tra[i++], cmp) == 0) {
			i--;
			break;
		}
	}
	if (strcmp(tra[i], "#") == 0) {
		return - 1;
	}
	return (i);
}

void init_product() {//��ʼ������ʽ�ṹ
	for (int i = 0; i < 100; i++) {
		strcpy_s(c[i].vn, "#");
		for (int j = 0; j < 10; j++) {
			c[i].itself[j].it = def;
			strcpy_s(c[i].itself[j].symbol, "#");
		}
	}
}

int special1(char cmp[10]) {
	int i = 0;
	while (strcmp(line[i], "#") != 0) {
		if (strcmp(line[i++], cmp) == 0)break;
	}
	i--;
	if (strcmp(line[i], "#") == 0) {
		cout << "�﷨������...";
		getchar();
		exit(0);
	}
	return (i);
}

void fill_v(int cmp) {//���
	int i = 0;
	for (; i < 10; i++) {
		if (v[i] == -1)break;
		else if (v[i] == cmp)return;
	}
	v[i] = cmp;
	return;
}

void lookup_first(int i) {//��first����
	int j = 0;
	if (c[i].itself[0].it != vtk) {
		for (; j < gng; j++) {
			if (strcmp(c[i].itself[0].symbol, c[j].vn) == 0)
				lookup_first(j);
		}
	}
	if (j == gng)return;
	int cmp = table_prepare(line, c[i].itself[0].symbol);
	fill_v(cmp);
	return;
}

void first(int x1, int i, int local) {//��first����
	if (c[i].itself[0].it == nothing) {
		follow(i, x1, local);
		return;
	}
	for (int i = 0; i < 10; i++) {
		v[i] = -1;
	}
	lookup_first(i);
	for (int i = 0; i < 10; i++) {
		if (v[i] == -1)break;
		table[x1][v[i]] = local;
	}
	return;
}

void follow(int i, int x1, int local) {//��follow����
	char cmp[15];
	for (int i = 0; i < 10; i++) {
		v[i] = -1;
	}
	strcpy_s(cmp, c[i].vn);
	for (int j = 0; j < gng; j++) {
		int s = -1;
		if (strcmp(c[j].vn, "#") == 0) {
			break;
		}
		for (int k = 0; k < 10; k++) {
			if (strcmp(c[j].itself[k].symbol, "#") == 0)break;
			if (strcmp(c[j].itself[k].symbol, cmp) == 0) {
				s = k + 1;
				break;
			}
		}
		if (s != -1) {
			if (c[j].itself[s].it == vtk) {
				s = special1(c[j].itself[s].symbol);
				fill_v(s);
			}
			else if (c[j].itself[s].it == vnk) {
				for (int n = 0; n < gng; n++) {
					if (strcmp(c[j].itself[s].symbol, c[n].vn) == 0)
						first(x1, n, local);
				}
			}
			else {
				if (strcmp(cmp, c[j].vn) == 0) {
					continue;
				}
				else follow(j, x1, local);
			}
		}
	}
	if (strcmp(cmp, row[0]) == 0) {
		int s = y;
		fill_v(y);
	}
	for (int i = 0; i < 10; i++) {
		if (v[i] == -1)break;
		table[x1][v[i]] = local;
	}
	return;
}

void get_select(int n) {//�ж�Ϊ��first��follow����
	for (int i = 0; i < n; i++) {
		int x1 = table_prepare(row, c[i].vn);
		first(x1, i, i);
	}
}

void finish_table() {//��ɷ�����
	int o;
	int m;
	int n = 0;
	for (int i = 0; i <= x; i++) {
		int j = 0;
		o = 0;
		while (production[i][j++] != '%') {
		}
		strncpy_s(c[n].vn, production[i], j - 1);
		m = j;
		for (;; j++) {
			if (production[i][j] == '&') {
				strncpy_s(c[n].itself[o].symbol, production[i] + m, j - m);
				m = j + 1;
				if (!traverse(row, c[n].itself[o].symbol)) {
					c[n].itself[o].it = vnk;
				}
				else if (!traverse(line, c[n].itself[o].symbol)) {
					c[n].itself[o].it = vtk;
				}
				if (strcmp("$", c[n].itself[o].symbol) == 0)  c[n].itself[o].it = nothing;
				o++;
			}
			if (production[i][j] == '|') {
				strncpy_s(c[n].itself[o].symbol, production[i] + m, j - m);
				m = j + 1;
				if (!traverse(row, c[n].itself[o].symbol)) {
					c[n].itself[o].it = vnk;
				}
				else if (!traverse(line, c[n].itself[o].symbol)) {
					c[n].itself[o].it = vtk;
				}
				if (strcmp("$", c[n].itself[o].symbol) == 0)  c[n].itself[o].it = nothing;
				n++;
				o = 0;
				strcpy_s(c[n].vn, c[n - 1].vn);
			}
			if (production[i][j] == '#') {
				break;
			}
		}
		n++;
	}
	gng = n;
	get_select(n);
}

void print_table() {//��ӡ����ʽ������
	cout << "�ɲ���ʽ���÷���������" << endl;
	cout << "                 ";
	char* analysistab = "Begin_SYNBL.dat";
	std::fstream selectout(analysistab, ios::out | ios::in);//�򿪴����ļ�
	int i = 0;
	if (!selectout) {
		cout << "open error!";
		getchar();
		getchar();
		exit(1);
	}
	selectout << "vn/vt ";
	for (int i = 0; i <= y; i++) {
		cout << line[i];
		selectout << line[i] << " ";
		int k = strlen(line[i]);
		for (int g = 0; g < 7 - k; g++)cout << " ";
	}
	selectout << endl;
	cout << endl;
	for (int i = 0; i <= x; i++) {
		cout << row[i];
		selectout << row[i] << " ";
		cout << "       ";
		int k = strlen(row[i]);
		for (int g = 0; g < 10 - k; g++)cout << " ";
		for (int j = 0; j <= y; j++) {
			if (table[i][j] + 1 >= 10) {
				cout << table[i][j] + 1 << "     ";
				selectout << table[i][j] + 1 << " ";
			}
			else {
				cout << table[i][j] + 1 << "      ";
				selectout << table[i][j] + 1 << " ";
			}
		}
		cout << endl;
		selectout << endl;
	}
	cout << "����������Լ����﷨����...";
	getchar();
	selectout.close();
	return;
}

void init_stack(PSTACK begin) {//��ʼ������ջ
	begin->pBottom = new NODE;
	strcpy_s(begin->pBottom->element, "#");
	begin->pBottom->pNext = NULL;
	begin->pTop = new NODE;
	begin->pTop->pNext = begin->pBottom;
	strcpy_s(begin->pTop->element, row[0]);
	return;
}

void push(PSTACK out, char* in) {//���ս�������ջ
	NODE* k = new NODE;
	strcpy_s(k->element, in);
	k->pNext = out->pTop;
	out->pTop = k;
}

bool empty(PSTACK out) {//���ջ�Ƿ�Ϊ��
	if ((out->pBottom == out->pTop) && (out->pBottom == NULL))
		return true;
	else
		return false;
}

char* pop(PSTACK out) {//��ȡջ��Ԫ�����ڱȽ�
	if (empty(out)) {
		cout << "error!�﷨������" << endl;
		getchar();
		exit(1);
	}
	return out->pTop->element;
}

void print_stack_out(PSTACK out) {//��ӡ����ջ��Ԫ�س�ջ����
	cout << out->pTop->element;
	int k = strlen(out->pTop->element);
	while (k < 30) {
		cout << " ";
		k++;
	}
	return;
}

int print_Syntx_match(duality gg) {//��ӡ�﷨ƥ�����
	int length;
	switch (gg.kind) {
	case P:cout << delimiters[gg.value2 - 1]; length = strlen(delimiters[gg.value2 - 1]); break;
	case K:cout << keyword[gg.value2 - 1]; length = strlen(keyword[gg.value2 - 1]); break;
	case I:if (strcmp(gg.value1, "") == 0) {
		cout << '#';
		length = 1;
		break;
	}
	case Ch:
	case St:
	case C:cout << gg.value1; length = strlen(gg.value1); break;
	case default:cout << '#'; length = 1; break;
	}
	return length;
}

void free_pop(PSTACK out) {//ջ��Ԫ�س�ջ
	PNODE clean = out->pTop;
	out->pTop = out->pTop->pNext;
	free(clean);
	return;
}

void analysis(int m, int n, PSTACK out) {//����ջ����ƥ�䣬�����������
	cout << "����";
	print_stack_out(out);
	free_pop(out);
	int k = table[m][n];
	int j = 0;
	int end;
	for (int i = 0;; i++) {
		if (strcmp(c[k].itself[i].symbol, "#") == 0) {
			end = i;
			break;
		}
	}
	for (int i = end - 1; i >= 0; i--) {
		if (c[k].itself[i].it == nothing)
		{
			cout << "$";
			j++;
			break;
		}
		push(out, c[k].itself[i].symbol);
		cout << out->pTop->element << " ";
		j += strlen(out->pTop->element) + 1;
	}
	cout << "��ջ";
	while ((j++) < 46)cout << " ";
}

void parsing(PSTACK begin) {//�﷨�����������
	int i = 0;
	int gh = 1;
	while ((TOKEN[i].kind != default) || (strcmp(begin->pTop->element, "#") != 0)) {//������ջ����������ж�Ϊ������ʱ�����﷨����
		char* cmp = pop(begin);
		if ((strcmp(begin->pTop->element, "if") == 0) | (strcmp(begin->pTop->element, "while") == 0) | (strcmp(begin->pTop->element, "else") == 0) | (strcmp(begin->pTop->element, "=") == 0) | (strcmp(begin->pTop->element, "+") == 0) | (strcmp(begin->pTop->element, "-") == 0) | (strcmp(begin->pTop->element, "*") == 0) | (strcmp(begin->pTop->element, "/") == 0) | (strcmp(begin->pTop->element, "id") == 0) | (strcmp(begin->pTop->element, "number") == 0) | (strcmp(begin->pTop->element, "ch") == 0) | (strcmp(begin->pTop->element, "we") == 0) | (strcmp(begin->pTop->element, "do") == 0) | (strcmp(begin->pTop->element, "ie") == 0) | (strcmp(begin->pTop->element, ">") == 0) | (strcmp(begin->pTop->element, "<") == 0) | (strcmp(begin->pTop->element, "==") == 0) | (strcmp(begin->pTop->element, ">=") == 0) | (strcmp(begin->pTop->element, "<=") == 0) | (strcmp(begin->pTop->element, "return") == 0) | (strcmp(begin->pTop->element, "cout") == 0) | (strcmp(begin->pTop->element, "cin") == 0)) {
			strcpy_s(action_seq[gh++], begin->pTop->element);
		}
		char cc[15];
		int m = table_get(row, begin->pTop->element);
		int n;
		switch (TOKEN[i].kind) {
		case I: {
			if (strcmp(TOKEN[i].value1, "") == 0) {
				n = y;
				break;
			}
			n = table_get(line, "id"); break;
		}
		case P:strcpy_s(cc, delimiters[TOKEN[i].value2 - 1]); n = table_get(line, cc); break;
		case K:strcpy_s(cc, keyword[TOKEN[i].value2 - 1]); n = table_get(line, cc); break;
		case Ch:n = table_get(line, "ch"); break;
		case St:n = table_get(line, "string"); break;
		case C:n = table_get(line, "number"); break;
		case default:n = table_get(line, "#"); break;
		}
		if (m >= 0 && m <= x && n >= 0 && n <= y)
		{
			analysis(m, n, begin);
		}
		else if (strcmp(begin->pTop->element, line[n]) == 0) {
			cout << "����";
			print_stack_out(begin);
			int rab = print_Syntx_match(TOKEN[i++]);
			cout << "ƥ��ɹ�";
			while ((rab++) < 42)cout << " ";
			free_pop(begin);
		}
		else {
			cout << "���ִ��󣬴������﷨��������ջ������" << endl;
			getchar();
			exit(1);
		}
		cout << "��ǰջ��Ԫ��Ϊ:";
		print_stack_out(begin);
		cout << "��ǰ������ĸΪ��";
		print_Syntx_match(TOKEN[i]);
		if ((strcmp(begin->pTop->element, "#") == 0) && TOKEN[i].kind == default) {
			cout << endl << "#ƥ��ɹ�" << endl;
			return;
		}
		cout << endl;
	}
	return;
}

bool val(char cmp[15]) {//�ж��ǲ��ǽ�����Ƿ���false�����Ƿ���true
	if (strcmp(cmp, "+") == 0) {
		return false;
	}
	else if (strcmp(cmp, "-") == 0) {
		return false;
	}
	else if (strcmp(cmp, "*") == 0) {
		return false;
	}
	else if (strcmp(cmp, "/") == 0) {
		return false;
	}
	else if (strcmp(cmp, ">") == 0) {
		return false;
	}
	else if (strcmp(cmp, "<") == 0) {
		return false;
	}
	else if (strcmp(cmp, "==") == 0) {
		return false;
	}
	else if (strcmp(cmp, ">=") == 0) {
		return false;
	}
	else if (strcmp(cmp, "<=") == 0) {
		return false;
	}
	else if (strcmp(cmp, "=") == 0) {
		return false;
	}
	return true;
}

void brief() {
	for (int i = 1; i < 1000; i++) {
		if (strcmp(action_seq[i], "#") == 0)break;
		if (strcmp(action_seq[i], "id") == 0) {
			bool g = val(action_seq[i - 1]) && val(action_seq[i + 1]);
			while (g) {
				for (int j = i; j < 1000; j++) {
					if (strcmp(action_seq[j], "#") == 0)break;
					strcpy_s(action_seq[j], action_seq[j + 1]);
				}
				g = val(action_seq[i - 1]) && val(action_seq[i + 1]);
			}
		}
	}
	cout << "������������" << endl;
	int i = 1;
	while (1) {
		if (strcmp(action_seq[i], "#") == 0)break;
		cout << action_seq[i++] << endl;
	}
}

int main() {//�����������ڶ���ʷ�������ɵõ��ĵ�������
	empty();
	char*word_seq = tokenaly();
	PSTACK analysis_stack = new STACK;
	std::fstream outfile(word_seq, ios::out | ios::in);//�򿪴����ļ�
	int i = 0;
	if (!outfile) {//�򿪴���
		cout << "open error!";
		getchar();
		exit(1);
	}
	init_duality(TOKEN);//��ʼ����Ԫʽ
	while (!outfile.eof()) {
		int dual;
		outfile >> dual;
		TOKEN[i].kind = (style)dual;
		switch (TOKEN[i].kind) {
		case P:
		case K:outfile >> TOKEN[i++].value2; break;
		case I:
		case Ch:
		case St:
		case C:outfile >> TOKEN[i++].value1; break;
		}
	}
	TOKEN[i - 1].kind = default;
	char grammar[] = "grammar.txt";
	std::fstream gram(grammar, ios::out | ios::in);//�򿪲���ʽ�ļ�
	cout << "����ʽ����" << endl;
	for (int i=0;;i++) {
		gram >> production[i];
		if (strcmp(production[i], "#") == 0) {
			break;
		}
		cout << i + 1 << '.';
		for (int j = 0;; j++) {
			if (production[i][j] == '\0') break;
			if (production[i][j] == '%') {
				cout << "->";
				continue;
			}
			if (production[i][j] == '&') {
				cout << " ";
				continue;
			}
			cout << production[i][j];
		}
		cout << endl;
	}
	init_action_seq();//��ʼ����������
	gram.close();//�رղ���ʽ�ļ�
	outfile.close();//�رմ����ļ�
	init_table();//��ʼ��������
	init_product();//��ʼ������ʽ�ṹ
	finish_table();//��ɷ�����
	print_table();//��ӡ������
	init_stack(analysis_stack);//��ʼ������ջ
	cout << "�﷨������������(ʹ��LL(1)������):" << endl;
	parsing(analysis_stack);//�﷨��������
	brief();
	char ac[] = "action.dat";
	std::fstream action(ac, ios::out | ios::in);//�򿪶��������ļ�
	for (int i = 1;; i++) {
		if (strcmp(action_seq[i], "#") == 0) {
			action << action_seq[i] << endl;
			break;
		}
		action << action_seq[i] << endl;
		if (strcmp(action_seq[i], "cout") == 0)i = i + 2;
		if (strcmp(action_seq[i], "cin") == 0)i = i + 2;
	}
	cout << endl;
	action.close();//�رն��������ļ�
	cout << "�������У��﷨�����޴���,����������Խ�����ű���";
	getchar();
	Begin_SYNBL();//����ű�
	return 0;
}

