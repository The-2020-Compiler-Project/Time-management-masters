#include"��ͷ.h"
enum dif{ I, C, K, P, Ch, St, default };//��ʶ�� ���� �ؼ��� ��� �ַ� �ַ���
char t[100][15];/*��ʱ����*/
enum type{number,ch,id,undefine};
char actid[20][10] = { "+","-","*","/",">","<",">=","<=","==","=","if","else","ie","while","do","we" ,"return","cout","cin","#"};/*ö�ٶ���*/
int sem[1000];/*����ջ*/

struct quaternion {/*��Ԫʽ�ṹ*/
	int idact;	//�������
	char id1[15];//��������
	char id2[15];
	char id3[15];
	type type1;
	type type2;
};

struct duality {/*���µ����Ԫʽ*/
	dif kind;
	char value1[25];
	int value2;
}; 
duality token[1000];
quaternion quater[100];

struct actsequence {/*��������*/
	char name[15];
	char value1[15];
	float value2;
};
actsequence sport[100];

void initact() {
	for (int i = 0; i < 100; i++) {
		quater[i].idact = -1;
		strcpy_s(quater[i].id1, "#");
		strcpy_s(quater[i].id2, "#");
		quater[i].type1 = undefine;
		quater[i].type2 = undefine;
		strcpy_s(quater[i].id3, "#");
	}
	return;
}

int act(char acti[20][10],char *cmp) {
	for (int i = 0;i<20;i++){
		if (strcmp(acti[i], cmp) == 0) 
			return i+1;
	}
	return 0;
}

void actanalysis() {
	int i = 0;
	int j = 0;
	while (1) {
		if (strcmp(sport[i].name, "#") == 0)break;
		if (act(actid, sport[i].name))
			quater[j++].idact = act(actid, sport[i].name)-1;
		i++;
	}
	int acloca = j - 1;//��Ԫʽ����
	for (int i = 0;; i++) {
		if (token[i].kind == default) {
			j = i-1;//TOKEN����
			break;
		}
	}
	int loca = 0;//����ջ����
	int tn = -1;
	int s = 0;
	for (int i = j; i >= 0; i--) {/*�����Ԫʽ����ȡ�Ե����Ϸ����ķ�*/
		if ((quater[acloca].idact == 12) | (quater[acloca].idact == 15) | (quater[acloca].idact == 11)|(quater[acloca].idact==14)) {//����ifend��whileend��do��elseֱ������������Ԫʽ��������
			acloca--;
			i++;
			continue;
		}
		if ((token[i].kind == I) | (token[i].kind == C) | (token[i].kind == Ch)) {//��ʶ�����������ַ�
			sem[loca++] = i;
			continue;
		}
		if ((token[i].kind == P) && (token[i].value2 <= 10)) { //���
			if ((token[i - 2].value2 == 17)|(token[i-2].value2==18)) {
				i--;
				continue;
			}
			if (token[i].value2 == 4) {//��ֵ���
				loca--;
				if (sem[loca] < 0) {
					char add[15];
					add[0] = 't';
					add[1] = fabs(sem[loca]) + 48;//*
					add[2] = '\0';
					strcpy_s(quater[acloca].id1, add);
					quater[acloca].type1 = id;
				}
				else {
					strcpy_s(quater[acloca].id1, token[sem[loca]].value1);
					switch (token[sem[loca]].kind) {
					case I:quater[acloca].type1 = id; break;
					case C:quater[acloca].type1 = number; break;
					case Ch:quater[acloca].type1 = ch;
					}
				}
				sem[loca] = 0;
				strcpy_s(quater[acloca].id3, token[--i].value1);
			}
			else {
				loca--;
				strcpy_s(quater[acloca].id1, token[sem[loca]].value1);
				switch (token[sem[loca]].kind) {
				case I:quater[acloca].type1 = id; break;
				case C:quater[acloca].type1 = number; break;
				case Ch:quater[acloca].type1 = ch;
				}
				sem[loca] = 0;
				strcpy_s(quater[acloca].id2, token[--i].value1);
				switch (token[i].kind) {
				case I:quater[acloca].type2 = id; break;
				case C:quater[acloca].type2 = number; break;
				case Ch:quater[acloca].type2 = ch;
				}
				sem[loca] = tn--;
				char add[15];
				add[0] = 't';
				add[1] = fabs(sem[loca++]) + 48;
				add[2] = '\0';
				strcpy_s(quater[acloca].id3, add); 
			}
			acloca--;
		}
		if (token[i].kind == K) {
			if ((token[i].value2 == 5) | (token[i].value2 == 11) | (token[i].value2 == 16)|(token[i].value2==17)|(token[i].value2==18)) {
				char add[15];
				char num[15];
				loca--;
				if (sem[loca] < 0) {
					add[0] = 't';
					add[1] = fabs(sem[loca]) + 48;
					cout << add[1];
					add[2] = '\0';
				}
				else {
					strcpy_s(add, token[sem[loca]].value1);
				}
				sem[loca] = 0;
				strcpy_s(quater[acloca--].id3, add);
			}
		}
	}
	return;
}

void range(char c[30]) {
	int i = strlen(c);
	while (i++ < 16)cout << " ";
	return;
}

void PrintQuar() {
	cout << "��Ԫʽ����:" << endl;
	for (int i = 0; i < 100; i++) {
		if (quater[i].idact == -1)break;
		cout << actid[quater[i].idact]; range(actid[quater[i].idact]);
		cout << quater[i].id1; range(quater[i].id1);
		cout << quater[i].id2; range(quater[i].id2);
		cout << quater[i].id3 << endl;
	}
	char four[] = "four_elem.dat";
	std::fstream act3(four, ios::out | ios::in);/*����Ԫʽ�ļ�*/
	int he = 0;
	for (int i = 0; i < 100; i++) {
		act3 << actid[quater[i].idact];
		he = strlen(actid[quater[i].idact]);
		while (he++ < 16)act3 << " ";
		act3 << quater[i].id1; 
		he = strlen(quater[i].id1);
		while (he++ < 16)act3 << " ";
		act3 << quater[i].type1 << " ";
		act3 << quater[i].id2; 
		he = strlen(quater[i].id2);
		while (he++ < 16)act3 << " ";
		act3 << quater[i].type2 << " ";
		act3 << quater[i].id3 << endl;
		if (quater[i].idact == -1)break;
	}
	act3.close();
	char f[] = "four_element.dat";
	std::fstream act4(f, ios::out | ios::in);/*����Ԫʽ�ļ�*/
	for (int i = 0; i < 100; i++) {
		act4 << quater[i].idact << endl;
		act4 << quater[i].id1 << endl;
		act4 << quater[i].type1 << endl;
		act4 << quater[i].id2 << endl;
		act4 << quater[i].type2 << endl;
		act4 << quater[i].id3 << endl;
		if (quater[i].idact == -1)break;
	}
	act4.close();
	return;
}

void translate1() {
	char receive[] = "action.dat";
	char get[] = "word_sequence.dat";
	std::fstream act1(receive, ios::out | ios::in);/*�򿪶��������ļ�*/
	std::fstream act2(get, ios::out | ios::in);/*�򿪵��������ļ�*/
	for (int i = 0; i < 100; i++) {
		sport[i].value2 = -100;
		strcpy_s(sport[i].value1, "#");
		strcpy_s(sport[i].name, "#");
	}
	for (int i = 0;; i++) {/*��ȡ��������*/
		act1 >> sport[i].name;
		if (strcmp(sport[i].name, "#") == 0)break;
	}
	act1.close();
	int i = 0;
	while (!act2.eof()) {
		int sty;
		act2 >> sty;
		token[i].kind = (dif)sty;
		switch (token[i].kind) {
		case P:
		case K:act2 >> token[i++].value2; break;
		case I:
		case Ch:
		case St:
		case C:act2 >> token[i++].value1; break;
		}
	}
	token[i - 1].kind = default;
	act2.close();
	initact();
	actanalysis();
	PrintQuar();
	cout << "�м���뷭�����,����������Խ���Ŀ���������";
	getchar();
	targetcode();
	return;
}