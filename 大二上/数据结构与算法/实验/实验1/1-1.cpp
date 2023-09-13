#include<stdio.h> 
#include<stdlib.h>

typedef struct node {
	int data;    //���������ﶨ��Ϊint���� 
	struct node *next; //ָ����
}Node,*LinkList;

//����������
LinkList create() {
	LinkList list=(LinkList)malloc(sizeof(Node));//����ռ�
	if(list!=NULL) list->next=NULL;
	else printf("Create failed!\n");
	return(list);
}

//�жϿ�����
int isempty(LinkList list) {
	return(list->next==NULL);
}

//����������ĳԪ�صĴ洢λ��
int find(LinkList list,int x) {
	Node* p=list->next;
	int i=1; 
	while(p->next!=NULL && p->data!=x){
		p=p->next;
		i++;
	}
	if(p->data!=x)i=0; 
	return i;
}

//������Ĳ��루����ĩβ����Ԫ��x�� 
int insert(LinkList list,int x) {
	Node *p=list,*q=(Node*)malloc(sizeof(Node));
	while(p->next!=NULL){
		p=p->next;
	}
	if(p==NULL) {
		printf("Insert failed!\n");
		return 0;
	} else {
		q->data=x;
		q->next=NULL;
		p->next=q;
		return 1;
	}
}

// �������ɾ��(ɾ����һ��ֵΪx�Ľ��)
int delete_node(LinkList list,int x) {
	Node *p,*q;
	p=list;
	if(p->next==NULL)  return 0;
	while(p->next!=NULL && p->next->data!=x)
		p=p->next;//��ֵΪx�Ľ���ǰ�����Ĵ洢λ��
	if(p->next==NULL) { //û�ҵ�ֵΪx�Ľ��
		printf("Not exist!\n");
		return 0;
	} else {
		q=p->next;
		p->next=q->next;
		free(q);
		return 1;
	}
}

//��ӡ���� 
void show(LinkList list){
	Node *p=list->next;
	while(p!=NULL) {
		if(p->next==NULL)
			printf("%d",p->data);
		else
			printf("%d->",p->data);
		p=p->next;
	}
	printf("\n");
}

int main() {
	int i,x;
	Node *p;
	LinkList list1;
	list1=create();
	if(list1!=NULL)
		printf("�����ձ�ɹ���\n");
	for(i=0; i<5; i++) {
		printf("������� %d ������ֵ��",i+1);
		scanf("%d",&x);
		insert(list1,x);
	}
	printf("\n");
	show(list1);
	printf("\n");
	printf("��������ҽ���ֵ��\n");
	scanf("%d",&x);
	if(!find(list1,x)){
		printf("δ���ҵ���\n");
	}else{
		printf("�ڵ�%d���ڵ��ҵ���Ԫ�أ�\n", find(list1,x));
	}
	printf("������ɾ������ֵ��",i+1);
	scanf("%d",&x);
	delete_node(list1,x);
	printf("\n");
	show(list1);
	return 0;
}
