#include"��ͷ.h"
static int state = 1;
int location1 = 0;
char str[10000];
char strc[]="text.cpp";
char ch;
char token[25];
char know[30]="wordsequence.dat";
enum style{I,C,K,P,Ch,St,def};
static char *keyword[18] = { "int","float","char","void","if","else","switch","case","for","do","while","continue","break","default","sizeof","return","cout","cin" };//�ؼ��ֱ�
static char *delimiters[18] = { ">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")" ,"[","]"};//�����
char variate[16][10] = {};//������t
int serial = 0;

static struct two_elements {//��Ԫʽ���ݽṹ
	style kind;
	char value1[25];
	int value2;
};

static two_elements TOKEN[1000];//�ʷ����У���Ԫʽ�ṹ)
static void init_twoele(two_elements*token) {
	//��ʼ����Ԫʽ�ͱ�����
	for (int i = 0; i < 1000; i++) {
		token[i].kind = def;
		strcpy_s(token[i].value1, "#");
		token[i].value2 = 0;
	}
	for (int i = 0; i < 16; i++) strcpy_s(variate[i], "#");
	return;
}

void fvalue(char **cmp, int id) {
	//����ؼ��ֻ���
	cout << cmp[id - 1];
}

int is_keyword(char*cmp) {
	//��ѯ��õ����Ƿ�Ϊ�ؼ��֣����򷵻����
	for (int i = 0; i < 18; i++) {
		if (strcmp(keyword[i], cmp) == 0)return i + 1;
	}
	return 0;
}

void printf_token(two_elements*token) {
	//�����������
	cout << "����                 ֵ";
	cout << "            ����                 ֵ";
	cout << "            ����                 ֵ";
	cout << "            ����                 ֵ" << endl;
	float ck = 10;
	for (int i = 0; i < 1000; i++) {
		if (token[i].kind == def) break;//������ֹͣ���
		int j=0;
		float number=0;
		cout << '('<<i + 1<<')' ;
		if (i < 9)cout << ' ';
		switch (token[i].kind) {
		case I:cout << "��ʶ��           " << token[i].value1; j=strlen(token[i].value1); break;
		case C:cout << "����             ";
			sscanf_s(token[i].value1, "%f", &number);
			printf_token("%g", number);
			for (int f = 0;; f++) {
				if (token[i].value1[f] == '\0')break;
				if (token[i].value1[f] == '0'&&token[i].value1[f + 1] == '0'&&token[i].value1[f + 2] == '0')break;
				if (token[i].value1[f] == '.'&&token[i].value1[f + 1] == '0'&&token[i].value1[f + 2] == '0')break;
				j++;
			}
			break;
		case K:cout << "�ؼ���           "; fvalue(keyword,token[i].value2); j = strlen(keyword[token[i].value2-1]); break;
		case P:cout << "���             "; fvalue(delimiters,token[i].value2); j = strlen(delimiters[token[i].value2 - 1]); break;
		case Ch:cout << "�ַ�             " << token[i].value1  ; j = strlen(token[i].value1); break;
		case St:cout << "�ַ���           " << token[i].value1; j = strlen(token[i].value1); break;
		}
		while (j < 14) {
			cout << " ";
			j++;
		}
		if ((i+1) % 4 == 0)cout << endl;
	}
	cout << endl;
}

void add_to_varT(char*cmp) {//���������
	int i = 0;
	for (; i < 16; i++) {
		if (strcmp(variate[i],"#") == 0)break;
		if (strcmp(variate[i], cmp) == 0)return;
	}
	strcpy_s(variate[i], cmp);
	return;
}

void prep(char ch) {//��Դ����Ԥ����
	int state = 1;
	int cycle = 0;
	int back;
	if (ch == '/') {//����'/'��׼������ע��
		state = 2;
		back = location1 - 1;
	}
	else {
		location1--;
		return;
	}
	ch = str[location1++];
	while (ch != EOF) {
		switch (state) {
		case 2:
			if (ch == '*')/*����'*'����Ϊ�ѽ���ע���ֶ�*/
				state = 3;
			else {
				state = 1;
				cycle = 1;
			}
			break;
		case 3:if (ch == '*')/*��һ�γ���'*'��׼������ע���ֶ�*/
			state = 4; break;
		case 4:if (ch == '/')/*��һ�γ���'/'������ע���ֶΣ������������ע�͹���*/
			state = 5;
			   else
				   state = 4; break;
		case 5:cycle = 1;
		}
		if (cycle == 1)
			break;
		ch = str[location1++];
	}
	if (state == 1) {
		location1 = back;
	}
	return;
}

static void scanner() {//�ʷ�ɨ��
	int i = 0;
	ch = str[location1++];
	for (int i = 0; i < 25; i++)
	{
		token[i] = NULL;//��յ��ʱȽϴ���
	}

	prep(ch);
	ch = str[location1++];

	while (ch == ' ') {
		ch = str[location1++];
	}

	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z')) {//����ؼ��ֻ�����ж�
		token[i++] = ch;
		ch = str[location1++];
		while ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z') || (ch >= '0'&&ch <= '9')||(ch=='_'))
		{
			token[i++] = ch;
			ch = str[location1++];
		}
		token[i] = '\0';
		location1--;
		TOKEN[serial].kind = I;
		if (is_keyword(token)) {//��ؼ��ֱ��������������
			TOKEN[serial].kind = K;
			TOKEN[serial].value2 = is_keyword(token);
		}
		else {//��������������������бȽ�
			strcpy_s(TOKEN[serial].value1, token);
			add_to_varT(token);
		}
		serial++;
	}
	else if (ch >= '0'&&ch <= '9') {//��������
		int num = ch - '0';
		bool key = false;
		float dot = 1;
		ch = str[location1++];
		while ((ch >= '0'&&ch <= '9') || (ch == '.')) {
			if (ch == '.') {
				ch = str[location1++];
				key = true;
				continue;
			}
			if (key == true) {
				dot /= 10;
			}
			num = num * 10 + ch - '0';//ѭ����10���������λ
			ch = str[location1++];
		}
		float numget = (float)num;
		numget *= dot;
		location1--;
		TOKEN[serial].kind = C;
		sprintf_s(TOKEN[serial++].value1, "%f", numget);
	}
	else {
		TOKEN[serial].kind = P;
		switch (ch) {//����ж�
		case '>':
			if (str[location1] == '=') {
				TOKEN[serial].value2 = 1;
				location1++;
			}
			else
				TOKEN[serial].value2 = 5;
			serial++;
			break;
		case'<':
			if (str[location1] == '=') {
				TOKEN[serial].value2 = 2;
				location1++;
			}
			else
				TOKEN[serial].value2 = 6;
			serial++;
			break;
		case'=':
			if (str[location1] == '=') {
				TOKEN[serial].value2 = 3;
				location1++;
			}
			else
				TOKEN[serial].value2 = 4;
			serial++;
			break;
		case'+':TOKEN[serial++].value2 = 7; break;
		case'-':TOKEN[serial++].value2 = 8; break;
		case'*':TOKEN[serial++].value2 = 9; break;
		case'/':TOKEN[serial++].value2 = 10; break;
		case'{':TOKEN[serial++].value2 = 11; break;
		case'}':TOKEN[serial++].value2 = 12; break;
		case',':TOKEN[serial++].value2 = 13; break;
		case';':TOKEN[serial++].value2 = 14; break;
		case'(':TOKEN[serial++].value2 = 15; break;
		case')':TOKEN[serial++].value2 = 16;  break;
		case'[':TOKEN[serial++].value2 = 17;  break;
		case']':TOKEN[serial++].value2 = 18;  break;
		case'\'': {/*�ַ���ȡ*/
			TOKEN[serial].kind = Ch;
			token[i++] = str[location1++];
			token[i] = '\0';
			strcpy_s(TOKEN[serial++].value1, token);
			if (str[location1++] != '\'') {
				cout << "���������⣬�ַ�ӦΪ��������..." << endl;
				getchar();
				exit(1);
			}
		}break;
		case'"': {/*�ַ�����ȡ*/
			TOKEN[serial].kind = St;
			for (; i < 25; ) {
				if (str[location1] != '"') {
					token[i++] = str[location1++];
				}
				else break;
			}
			token[i] = '\0';
			if (str[location1++] != '"') {
				cout << "���������⣬�ַ���Ӧ�ԡ�����" << endl;
				getchar();
				exit(1);
			}
			strcpy_s(TOKEN[serial++].value1, token);
		}break;
		}
	}
}

char* tokenaly() {
	system("cls");
	/*
	cout << "�ֶ����������ļ���λ��·��:";
	cin >> strc;
	*/
	std::fstream outfile(strc, ios::out | ios::in);/*�򿪴����ļ�*/
	for (int i = 0; i < 10000; i++) {
		str[i] = NULL;
	}
	if (!outfile) {
		cout << "open error!";
		exit(1);
	}
	outfile.get(str, 10000, EOF);/*���������ݸ��ƽ�ȥ�������ַ���*/
	outfile.close();
	init_twoele(TOKEN);
	cout << "����������£�" << endl << str << endl << "�����ʷ��������:" << endl;
	while (str[location1] != NULL) {
		scanner();
	}
	printf_token(TOKEN);
	char* path = know; // ��Ҫ�����ļ���·��
	ofstream fout(path);
	for (int i = 0; i < serial; i++) {/*�������ֵ�͵�������*/
		fout << TOKEN[i].kind << endl;
		switch (TOKEN[i].kind)
		{
		case P:
		case K:fout << TOKEN[i].value2<<endl; break;
		case I:
		case Ch:
		case St:
		case C:fout << TOKEN[i].value1<<endl;
		}
	}
	cout << "�����ϣ��밴��������������﷨����";
	fout.close();
	getchar();
	return know;
}
