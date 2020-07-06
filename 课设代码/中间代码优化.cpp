#include"标头.h"
enum ntype { number, ch, id, undefine };
char actions[20][10] = { "+","-","*","/",">","<",">=","<=","==","=","if","else","ie","while","do","we" ,"return","cout","cin","#" };//枚举动作

struct ready_opts {//准备优化的四元式结构
	int idact;
	char id1[15];
	char id2[15];
	ntype type1;
	ntype type2;
	char id3[15];
};
ready_opts _four[100];

struct DAG_node {//DAG图节点及链节点
	char information[15];//节点信息
	ntype it;//节点类型
	struct DAG_node *lnode;
};

struct DAG {//DAG图
	int ope_ch;//节点运算符
	int level;
	struct DAG *left;//左叶子
	struct DAG *right;//右叶子
	struct DAG_node ide;
};

void opt_process(int low, int up) {//中间代码优化主要程序
	DAG dag_graph[100];//DAG图
	DAG *temp_dag1[100];//临时变量DAG
	DAG *temp_dag2[100];
	for (int i = 0; i < 100; i++) {
		temp_dag1[i] = new DAG;
		temp_dag2[i] = new DAG;
	}
	for (int i = low; i <= up; i++) {//产生树结点
		dag_graph[i].ope_ch = _four[i].idact;
		dag_graph[i].level = 0;
		switch (_four[i].idact) {
		case 0:
		case 1:
		case 2:
		case 3: {//加减乘除
			dag_graph[i].left = temp_dag1[i];
			dag_graph[i].right = temp_dag2[i];
			temp_dag1[i]->ope_ch = 9;
			temp_dag2[i]->ope_ch = 9;
			dag_graph[i].ide.it = id;
			strcpy_s(dag_graph[i].ide.information, _four[i].id3);
			strcpy_s(temp_dag1[i]->ide.information, _four[i].id1);
			strcpy_s(temp_dag2[i]->ide.information, _four[i].id2);
			switch (_four[i].type1) {
			case number:temp_dag1[i]->ide.it = number; break;
			case ch:temp_dag1[i]->ide.it = ch; break;
			case id:temp_dag1[i]->ide.it = id;
			}
			switch (_four[i].type2) {
			case number:temp_dag2[i]->ide.it = number; break;
			case ch:temp_dag2[i]->ide.it = ch; break;
			case id:temp_dag2[i]->ide.it = id;
			}
			temp_dag1[i]->level = -1;
			temp_dag2[i]->level = -1;
			temp_dag1[i]->right = NULL;
			temp_dag1[i]->left = NULL;
			temp_dag2[i]->right = NULL;
			temp_dag2[i]->left = NULL;
			temp_dag1[i]->ide.lnode = NULL;
			temp_dag2[i]->ide.lnode = NULL;
		}break;
		case 9: {//等于
			dag_graph[i].left = NULL;
			dag_graph[i].right = NULL;
			strcpy_s(dag_graph[i].ide.information, _four[i].id3);
			switch (_four[i].type1) {
			case number:dag_graph[i].ide.it = number; break;
			case ch:dag_graph[i].ide.it = ch; break;
			case id:dag_graph[i].ide.it = id;
			}
			dag_graph[i].ide.lnode = new DAG_node;
			strcpy_s(dag_graph[i].ide.lnode->information, _four[i].id1);
			dag_graph[i].ide.lnode->it = dag_graph[i].ide.it;
			dag_graph[i].ide.lnode->lnode = NULL;
		}break;
		}
	}
	//产生树
	for (int i = low; i <= up; i++) {//常数合并
		switch (dag_graph[i].ope_ch) {
		case 0:
		case 1:
		case 2:
		case 3: {
			if ((temp_dag1[i]->ide.it == number) && (temp_dag2[i]->ide.it == number)) {
				int value1 = atoi(temp_dag1[i]->ide.information);
				int value2 = atoi(temp_dag2[i]->ide.information);
				int value3 = 0;
				switch (dag_graph[i].ope_ch) {
				case 0:value3 = value1 + value2; break;
				case 1:value3 = value1 - value2; break;
				case 2:value3 = value1*value2; break;
				case 3:value3 = value1 / value2; break;
				}
				temp_dag1[i] = NULL;
				temp_dag2[i] = NULL;
				dag_graph[i].ope_ch = 9;
				dag_graph[i].left = NULL;
				dag_graph[i].right = NULL;
				dag_graph[i].ide.lnode = new DAG_node;
				_itoa_s(value3, dag_graph[i].ide.lnode->information, 10);
				dag_graph[i].ide.lnode->it = number;
				dag_graph[i].ide.lnode->lnode = NULL;
			}
		}
		}
	}
	for (int i = low; i <= up; i++) {//节省常值表达式
		if (dag_graph[i].ope_ch == 9) {
			if (dag_graph[i].ide.lnode->it == id) {
				char cmp[15];
				strcpy_s(cmp, dag_graph[i].ide.lnode->information);
				for (int j = low; j < i; j++) {
					if (dag_graph[j].ope_ch == 9) {
						if (strcmp(cmp, dag_graph[j].ide.information) == 0) {
							if (dag_graph[j].ide.lnode->it == number) {
								if (dag_graph[j].ide.information[0] == 't') {
									dag_graph[i].ide.lnode->it = number;
									strcpy_s(dag_graph[i].ide.lnode->information, dag_graph[j].ide.lnode->information);
									dag_graph[j].ope_ch = -2;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	for (int i = low; i <= up; i++) {//节省公共子表达式
		if ((dag_graph[i].ope_ch >= 0) && (dag_graph[i].ope_ch <= 3)) {
			for (int j = low; j < i; j++) {
				if (dag_graph[i].ope_ch == dag_graph[j].ope_ch) {
					if (strcmp(temp_dag1[i]->ide.information, temp_dag1[j]->ide.information) == 0) {
						if (strcmp(temp_dag2[i]->ide.information, temp_dag2[j]->ide.information) == 0) {
							char cmp[15];
							char change[15];
							strcpy_s(cmp, dag_graph[i].ide.information);
							dag_graph[i].ope_ch = -2;
							strcpy_s(change, dag_graph[j].ide.information);
							for (int k = i + 1; k <= up; k++) {
								if (dag_graph[k].ope_ch == 9) {
									if (strcmp(dag_graph[k].ide.lnode->information, cmp) == 0)strcpy_s(dag_graph[k].ide.lnode->information, change);
								}
							}
							break;
						}
					}
				}
			}
		}
	}
	for (int i = low; i <= up; i++) {
		if ((_four[i].idact == 17)|(_four[i].idact==18))continue;
		_four[i].idact = dag_graph[i].ope_ch;
		if (dag_graph[i].ope_ch == -1)break;
		strcpy_s(_four[i].id3, dag_graph[i].ide.information);
		switch (dag_graph[i].ope_ch) {
		case -2:break;
		case 0:
		case 1:
		case 2:
		case 3: {
			strcpy_s(_four[i].id1, temp_dag1[i]->ide.information);
			strcpy_s(_four[i].id2, temp_dag2[i]->ide.information);
			_four[i].type1 = temp_dag1[i]->ide.it;
			_four[i].type2 = temp_dag2[i]->ide.it;
		}break;
		case 9:
			strcpy_s(_four[i].id1, dag_graph[i].ide.lnode->information);
			_four[i].type1 = dag_graph[i].ide.lnode->it;
		}
	}
	return;
}

void div_basicblock() {//划分基本块
	int low = 0;
	int up = 0;
	for (;; up++) {
		switch (_four[up].idact) {
		case 10://if
		case 13: {//while
			opt_process(low, up - 2);
			if (_four[up].idact == 13)up++;
			low = up + 1;
			up = low;
		}break;
		case 16://return
		case 11://else
		case 12://ie
		case 15: {//whileend
			opt_process(low, up - 1);
			low = up + 1;
			up = low;
		}
		}
		if (_four[up].idact == -1)break;
	}
	opt_process(low, up);
	return;
}

void optimize_s() {
	int i = 0;
	int c1 = 3;
	int c2 = 3;
	char optimize[] = "optimize.dat";
	std::fstream optimizations(optimize, ios::out | ios::in);//打开四元式优化文件
	while (!optimizations.eof()) {
		optimizations >> _four[i].idact;
		optimizations >> _four[i].id1;
		optimizations >> c1;
		_four[i].type1 = (ntype)c1;
		optimizations >> _four[i].id2;
		optimizations >> c2;
		_four[i].type2 = (ntype)c2;
		optimizations >> _four[i++].id3;
	}
	div_basicblock();
	char end[] = "end.dat";
	std::fstream opt_finish(end, ios::out | ios::in);//打开四元式完成优化文件
	for (int i = 0;; i++) {
		if (_four[i].idact == -2)continue;
		opt_finish << _four[i].idact<<endl;
		opt_finish << _four[i].id1<<endl;
		opt_finish << _four[i].type1<<endl;
		opt_finish << _four[i].id2<<endl;
		opt_finish << _four[i].type2<<endl;
		opt_finish << _four[i].id3<<endl;
		if (_four[i].idact == -1)break;
	}
	opt_finish.close();
	cout << "中间代码优化结束，前端加优化基本完成，按下任意键以继续生成目标代码...";
	optimizations.close();
	getchar();
	return;
}