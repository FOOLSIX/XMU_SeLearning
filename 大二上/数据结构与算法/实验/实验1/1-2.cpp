#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
struct index {
	int key;
	int link;
} index_table[10];
typedef struct Slist*List;
struct Slist {
	int data[100];
	int n;
	int N;
};
List creatSlist(int num, List L) {
	int i;
	List P = (List)malloc(sizeof(Slist));
	for (i = 0; i < num; i++) {
		scanf("%d", &P->data[i]);
	}
	L = P;
	return L;
}
void creatindex(int a[][10], int hierarchy, int hierarchynum[], List L) {
	int i = 0, j = 0, k = 0;
	for (i = 0; i < hierarchy; i++) {
		for (j = 0; j < hierarchynum[i]; j++) {
			a[i][j] = L->data[k];
			k++;
		}
	}
}
void printfindex(int a[][10], int hierarchy, int hierarchynum[]) {
	int i = 0, j = 0, k = 0;
	for (i = 0; i < hierarchy; i++) {
		for (j = 0; j < hierarchynum[i]; j++) {
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
}
void findkey(struct index index_table[], int a[][10], int hierarchynum[], int hierarchy) {
	int i;
	for (i = 0; i < hierarchy; i++) {
		index_table[i].key = a[i][hierarchynum[i] - 1];
		index_table[i].link = i;
	}
}
int search_hierarchy(struct index index_table[], int hierarchy, int number) {
	int i, flag = 0;
	for (i = 0; i < hierarchy; i++) {
		if (number <= index_table[i].key) {
			flag = 1;
			break;
		}
	}
	if (flag == 1)return index_table[i].link;
	else {
//		printf("�����������\n");
		return -1;
	}
}
int searchpos(struct index index_table[], int hierarchy, int ind, int a[][10], int number, int hierarchynum[]) {
	int flag = 0,j;
	for (j = 0; j < hierarchynum[ind]; j++) {
		if (a[ind][j] == number) {
			flag = 1;
			break;
		}
	}
	if (flag == 1) {
		return j;
	} else {
//		printf("�����������\n");
		return -1;
	}
}
int main() {
	List L;
	L = NULL;
	int num, hierarchy,tot=0;
	int hierarchynum[10];
	int ind = -1, pos, number;
//	printf("������˳���Ĵ�����\n");
//	scanf("%d", &num);
	printf("������Ҫ�ֳɼ���\n");
	scanf("%d", &hierarchy);
	printf("������ÿ��Ҫ�ֳɼ���\n");
	for (int i = 0; i < hierarchy; i++) {
		scanf("%d", &hierarchynum[i]);
		tot+=hierarchynum[i];
	}
	printf("������%d����\n", tot);
	L = creatSlist(tot, L);
	int a[10][10];
	creatindex(a, hierarchy, hierarchynum, L);
	printf("����㼶���\n");
	printfindex(a, hierarchy, hierarchynum);
	findkey(index_table, a, hierarchynum, hierarchy);
	printf("������Ҫ���ҵ�����\n");
	scanf("%d", &number);
	int flag=0;
	ind = search_hierarchy(index_table, hierarchy, number);
	pos = searchpos(index_table, hierarchy, ind, a, number, hierarchynum);
	if (ind != -1 && pos != -1 && ind < hierarchy) {
		printf("����λ�ڵ�%d����%d��\n", ind + 1, pos + 1);
		flag = 1;
	}
	if(flag == 0)printf("���������ڣ�");
	return 0;
}
