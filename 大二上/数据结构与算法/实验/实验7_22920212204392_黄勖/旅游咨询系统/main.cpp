#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>
#define path "map.txt"
#include <windows.h>
#include <vector>
#include <iostream>
using namespace std;
const int MAX = 105;
const int inf = 0x3f3f3f;
// ����������Ϣ����ϵͳ

typedef struct node {
	char PlaceName[MAX]; // ��������֡�
	int number; //�������š�
	char PlaceNum[MAX]; // ���;
} Node;
typedef struct Tornode {
	int u;
	int v;
	int weight;
} TorNode; // ����յ�;���;
typedef struct matGraph {
	int map[MAX][MAX];
	int n , e;
} MatGraph; // ������ͼ�ڽӾ�������

typedef struct NODE {
	int key;// �ؼ�����
	Node infor; // ��Ϣ
	struct NODE *lchild;
	struct NODE *rchild;
} BSTNode;

typedef struct ANode {
	int adjvex; // ���
	struct ANode *nextarc;
	int weight;

} ArcNode; // �߽ڵ������
typedef struct Vnode {

	ArcNode *fitstarc;
	int count;
} VNode; // �ڽӱ��ͷ�������
typedef struct {
	VNode adjlist[MAX];
	int n , e; // �����Ͷ�����
} AdjGraph;
int vis[MAX];
int dis[MAX];
int state=0; 
int count1;
BSTNode *MapVex[MAX]; // ����
void CreatMatGragh(MatGraph &GT  ,int n ,int e , int pot, TorNode Gr[MAX][MAX]) {
	// ������������ͼ;
	for(int i = 1; i<=n; i++) {
		for(int j = 1; j <=n; j++) {
			if(i == j ) {
				GT.map[i][j] = 0;
			}
			GT.map[i][j] = inf;
		}
	}
	for(int i = 1; i<=e; i++) {
		GT.map[Gr[pot][i].u][Gr[pot][i].v] = Gr[pot][i].weight;
	}
	return;
}
int cmp(const Node &a , const Node &b) {
	return a.number <b.number;
}
int digit(int x ) {
	// ����λ��
	int t = x;
	int k = 0;
	while(t) {
		k++;
		t/=10;
	}
	return k;
}
char *transtr(int n) {
	//ת�����ַ���
	//char t[MAX];
	char *t;
	t = new char[MAX];
	int i = 0 ,x = n;
	while(x) {
		t[digit(n) - i-1] = (x%10) + '0';
		x/=10;
		i++;
	}
	return t;
}
void Read(Node Place[] ,int n ,int e) {
	// ��ȡ����
	FILE *fp;
	fp = fopen(path,"r");
	if(fp == NULL) {
		perror("fopen\n");
		exit(-1);
	}
	for(int i = 1; i<=n; i++) {
		fscanf(fp,"%s %d",Place[i].PlaceName,&Place[i].number);
		int d = digit(Place[i].number);
		char Tmp[MAX] = "000";
		if(d==1) {
			strcpy(Place[i].PlaceNum,transtr(Place[i].number));
			strcat(Tmp,Place[i].PlaceNum);
			strcpy(Place[i].PlaceNum,Tmp);
		} else if(d==2) {
			char Tmp[MAX] = "00";
			strcpy(Place[i].PlaceNum,transtr(Place[i].number));
			strcat(Tmp,Place[i].PlaceNum);
			strcpy(Place[i].PlaceNum,Tmp);

		} else if(d==3) {
			char Tmp[MAX] = "0";
			strcpy(Place[i].PlaceNum,transtr(Place[i].number));
			strcat(Tmp,Place[i].PlaceNum);

		}
		Place[i].PlaceNum[4]='\0';
	}

	return;
}

void MAtToList(MatGraph g , AdjGraph *&G) {
	//���ڽӾ���ת�����ڽӱ�
	ArcNode *p; // �߽ڵ�

	G = (AdjGraph *)malloc(sizeof(AdjGraph));
	for(int i = 1; i<=g.n; i++) {
		G->adjlist[i].fitstarc = NULL;
	}
	for(int i = 1; i<= g.n; i++) {
		for(int j = g.n; j >=1; j--) {
			if(g.map[i][j]!= 0 && g.map[i][j] !=inf ) {
				p = (ArcNode *)malloc(sizeof(ArcNode));
				p->adjvex = j;
				p->weight = g.map[i][j];
				p->nextarc = G->adjlist[i].fitstarc;
				G->adjlist[i].fitstarc = p;
			}
		}
	}
	G->n = g.n;
	G->e = g.e;
	return;
}
int DispAdj(AdjGraph *G ) {
	// ����ڽӱ�
	ArcNode *p;
	int count = 0;
	for(int i = 1; i <=G->n; i++ ) {
		p = G->adjlist[i].fitstarc;
		printf("%3d: " ,i );
		while(p!=NULL ) {
			printf("%3d[%d]-> ", p->adjvex , p->weight );
			p = p->nextarc;
			count++;
		}
		printf(" ^ \n");
	}

	return count;
}
BSTNode *SearchBST(BSTNode *bt , int k ) {
	// �ڶ����������в��� ���Ϊk �Ľڵ�
	// return �ڵ�ĵ�ַ
	if(bt == NULL || bt->infor.number == k ) {
		return bt;
	}
	if(k < bt->infor.number) {
		return SearchBST(bt->lchild , k);
	} else {
		return SearchBST(bt->rchild , k );
	}

}
void TopSort(AdjGraph *G) {
	// ��������;
	int St[MAX]  , top = -1;
	ArcNode * p;
	for(int i  = 1;  i<=G->n; i++) {
		G->adjlist[i].count = 0; // ��ʼ��
	}
	for(int i  = 1; i<=G->n; i++) {
		p = G->adjlist[i].fitstarc;
		while(p!=NULL) {
			G->adjlist[p->adjvex].count++;
			p = p->nextarc;

		}
	}

	for(int i = 1;  i <=G->n; i++) {
		if(G->adjlist[i].count == 0 ) {
			top++;
			St[top] = i;

		}
	}
	int i  , j;
	while(top>-1) {
		i = St[top];
		top --;
		cout<<i<<" ";
		p = G->adjlist[i].fitstarc;
		while(p!=NULL) {
			j = p->adjvex;
			G->adjlist[j].count --;
			if(G->adjlist[j].count ==0 ) {
				top++;
				St[top] = j;
			}
			p = p->nextarc;   // ��һ���ڽӵ�;
		}
	}


}

void DFS(AdjGraph *G , int v , Node Place[] ,BSTNode *bt  ) {
	// �����������
	ArcNode *p;
	BSTNode *Root;
	vis[v] = 1;
	//printf("%d ",v);
	Root = SearchBST(bt,v);// �ڶ������������ҵ�

	if(Root!=NULL) { // С��ʻ�����괬,���ӻ��쳣
//		if(vis[p->adjvex] = 0  )

		cout<<Root->infor.PlaceName <<"-> ";
		MapVex[++count1]= Root;// ��DFS�����Ľڵ����μ��뵽����ͼ��
		// ��������ͼ,
	}

	p = G->adjlist[v].fitstarc;
	while(p!=NULL) {
		if(vis[p->adjvex] == 0 ) {
			vis[p->adjvex] = 1; //
			DFS(G,p->adjvex,Place,bt);
			//vis[p->adjvex] = 0; //
		}
		p = p->nextarc;
	}

}
void Display(Node Place[] ,int n) {
	// ��ʾ���о�������
	cout<<"��������\t  ���\n";
	for(int i = 1; i<=n; i++) {
		printf("%8s\t%8s\n",Place[i].PlaceName,Place[i].PlaceNum);
		//	cout<<Place[i].PlaceName<< "\t"<<Place[i].PlaceNum<<endl;
		Sleep(100);
	}

}
void CreadMat(MatGraph &Map , int n , int e) {
	// �����ڽӾ���
	FILE *fp;
	fp = fopen("edge.txt","r");
	for(int i = 1; i<=n; i++) {
		for(int j = 1; j<=n; j++) {
			Map.map[i][j] = inf;
			if(i == j )
				Map.map[i][j] = 0;
		}

	}
	Map.n = n;
	Map.e = e;
	for(int i = 1; i<=e; i++) {
		int u , v ,w;
		fscanf(fp,"%d %d %d",&u,&v,&w);
		Map.map[u][v] = w;// ����ͼ
		Map.map[v][u] = w;
	}
	return;
}

bool InsertBST(BSTNode *&bt , Node k ) {
	// ��������������ڵ�
	if(bt==NULL) {

		bt = (BSTNode*)malloc(sizeof(BSTNode));
		bt->infor.number = k.number;
		strcpy(bt->infor.PlaceName,k.PlaceName);
		strcpy(bt->infor.PlaceNum,k.PlaceNum);
		bt->lchild = NULL;
		bt->rchild = NULL;
		return true;
	} else if (k.number == bt->infor.number) {
		return false;
	} else if (k.number < bt->infor.number ) {
		return InsertBST(bt->lchild , k);
	} else if (k.number > bt->infor.number ) {
		return InsertBST(bt->rchild , k);
	}

}
BSTNode *CreatBST(Node Place[] , int n) {
	// ��������������;
	BSTNode *bt = NULL;
	for(int i =  1; i <=n; i++) {
		InsertBST(bt,Place[i]);
	}

	return bt;
}
void Dijkstra(int cur,MatGraph Map,BSTNode *bt ,int End) {
	// ���·Dijkstra ʵ��
	int n , e;
	int Path[MAX];
	n = Map.n;
	e = Map.e;
	memset(vis,0,sizeof(vis));
	memset(Path,0,sizeof(Path));

	for(int i = 1; i<=n; i++) {
		dis[i] = Map.map[cur][i];

		if(Map.map[cur][i] < inf) {
			Path[i] = cur;
		} else {
			Path[i] = 0;
		}
	}
	vis[cur] = 1;
	Path[cur] = 0;
	for(int i = 1; i<n; i++) {
		int minn = inf;
		int u = -1;
		for(int j = 1; j<=n; j++) {
			if(!vis[j] && dis[j]<minn) {
				minn = dis[j];
				u = j;
			}

		}
		if(u !=-1) {
			vis[u] = 1;

			for(int v = 1; v <=n; v++) {
				if(Map.map[u][v]<inf && vis[v]== 0) {

					if(dis[v] > dis[u]+Map.map[u][v]) {
						dis[v] = dis[u] + Map.map[u][v];

						Path[v] = u;

					}

				}
			}

		}
	}
	BSTNode *pfind1 ,*pfind2,*pfind3;
	int pre;
	for(int i=1; i<=n; i++) { //�����������·��
		pfind1 = SearchBST(bt,cur);
		pfind2 = SearchBST(bt,i);
		if(pfind1 && pfind2) {
			if(End == pfind2->infor.number) {
				printf("%s �� %s����̾���Ϊ ",pfind1->infor.PlaceName,pfind2->infor.PlaceName);
				printf("%d m\n",dis[i]);  //��ӡ���
				pre = Path[i];
				printf("·����%s",pfind2->infor.PlaceName);
			}

			while(pre!=0) { //������ǰ������
				pfind3 = SearchBST(bt,pre);
				if(pfind1) {
					printf("<����%s",pfind3->infor.PlaceName);
					pre=Path[pre];
				} else {
					printf("\n");
					exit(0);
				}
			}

		} else {
			cout<<"������� "<<endl;
			exit(0);
		}

	}

	return;
}

void Prim(MatGraph Map ,int cur ,BSTNode *bt) {
	// ��С������
	int lowcost[MAX];
	int MIN;
	int closet[MAX] , i , j , k;

	cout<<"������·�滮  : "<<endl;
	for( i  = 1; i<=Map.n; i++) {
		//cout<<Map.map[cur][i]<<"  ";
		lowcost[i] = Map.map[cur][i];
		closet[i] = cur;
	}
	for( i = 1; i<Map.n; i++) {
		MIN = inf;
		for(j = 1; j<=Map.n; j++)

			if(lowcost[j]!=0 && lowcost[j] <MIN) {
				MIN = lowcost[j];
				k = j;
			}
		//printf("(%d ,%d)  : %d \n ",closet[k],k ,MIN);
		BSTNode *s = SearchBST(bt,closet[k]);
		BSTNode *sz = SearchBST(bt,k);
		if( s!=NULL && sz !=NULL) {
			cout<<s->infor.PlaceName <<" - "<<sz->infor.PlaceName <<endl;
		}

		lowcost[k] = 0;
		for(int j = 1; j <=Map.n; j++) {
			if(lowcost[j] !=0 && Map.map[k][j]<lowcost[j]) {
				lowcost[j] = Map.map[k][j];
				closet[j] = k;
			}
		}

	}
}
int Find(char *a, Node M[],int n  ) {
	//����
	int i;

	bool find = false;
	for( i = 1; i <=n; i++) {
		if(strcmp(a,M[i].PlaceName)==0) {
			find = true;
			break;
		}
	}

	return find?M[i].number:-1;
}
void InOrder(BSTNode *bt ) {
	// ����
	if(bt!=NULL) {
		InOrder(bt->lchild);
		cout<<bt->infor.number <<"   " <<bt->infor.PlaceName <<"  "<<bt->infor.PlaceNum <<endl;
		InOrder(bt->rchild);
	}
	return;
}
void PageInfor() {
	system("color 5E");
	cout<<"\t\t\t   *********************************************"<<endl;
	cout<<"\t\t\t   *                                           *"<<endl;
	cout<<"\t\t\t   *                                           *"<<endl;
	cout<<"\t\t\t   *\t       ����������Ϣ��ѯϵͳ            *"<<endl;
	cout<<"\t\t\t   *                �����о���                 *"<<endl;
	cout<<"\t\t\t   *                                           *"<<endl;
	cout<<"\t\t\t   *                                           *"<<endl;
	cout<<"\t\t\t   *\t"<<" ��"<<" 1  ������������ֲ�ͼ        ��    *"<<endl;
	cout<<"\t\t\t   *                                           *"<<endl;
	cout<<"\t\t\t   *\t"<<" ��"<<" 2  �����������ֲ�ͼ        ��    *"<<endl;
	cout<<"\t\t\t   *                                           *"<<endl;
	cout<<"\t\t\t   *\t"<<" ��"<<" 3  �������·��              ��    *"<<endl;
	cout<<"\t\t\t   *                                           *"<<endl;
	cout<<"\t\t\t   *\t"<<" ��"<<" 4  �����ѵ���·��          ��    *"<<endl;
	cout<<"\t\t\t   *                                           *"<<endl;
	cout<<"\t\t\t   *\t"<<" ��"<<" 5  ������·��              ��    *"<<endl;
	cout<<"\t\t\t   *                                           *"<<endl;
	cout<<"\t\t\t   *\t"<<" ��"<<" 6  �������·�߹滮ͼ        ��    *"<<endl;
	cout<<"\t\t\t   *                                           *"<<endl;
	cout<<"\t\t\t   *\t"<<" ��"<<" 7  �˳�ϵͳ                  ��    *"<<endl;
	cout<<"\t\t\t   *                                           *"<<endl;
	cout<<"\t\t\t   *                                           *"<<endl;
	cout<<"\t\t\t   *********************************************"<<endl;

	cout<<"����ѡ�� >>  :    ";

}
void DisplayMenu(int n ,int e ) {
	TorNode Gr[MAX][MAX];
	MatGraph Map ,TMP ,TorMap;
	int TourMap[MAX][MAX];
	AdjGraph *G;// �ڽӱ�
	Node Place[MAX]; // ���еľ�����Ϣ
	char start[MAX] ,end[MAX]; // �����յ�
	BSTNode *bt; // �������������ڵ�
	cout<<endl<<endl;
	int num;
	Read(Place,n,e); // ��������
	PageInfor(); // ��ʾҳ����Ϣ

	while(cin >> num && num!=7) {

		if(num == 1 ) {
			CreadMat(Map,n,e);
			cout<<" ��ͼ�����ɹ� "<<endl;
			state = 1;
		} else if(num == 2 ) {
			if(state < 1){
				cout << "��ͼδ������" << endl;system("pause");PageInfor();
				continue;
			}
			// ��ʾ����ͼ
			//Display(Place,n);

			cout<<"�����о�����Ϣ ��   \n";
			cout<<" ��������\t  ���\n";
			for(int i = 1; i <=n; i++) {
				cout<<Place[i].PlaceName<< "  : "<<Place[i].PlaceNum<<endl;
			}
			cout<<" ������ͼ "<<endl;
			for(int i = 1; i<= n; i++) {
				cout<<"  "<<i;
			}
			cout<<endl;
			int k = 1;

			for(int i = 1; i <=n; i++) {
				cout<<k++ <<"  ";

				for(int j = 1; j<=n; j++) {
					if(Map.map[i][j] == inf ) {
						cout<<"��"<<" ";

					} else {
						printf("%d ",Map.map[i][j]);

					}
				}
				cout<<endl;
			}
			cout<<endl;

		}

		else if(num == 3 ) {
			// ������е�����·��;
			if(state < 1){
				cout << "��ͼδ������" << endl;system("pause");PageInfor();
				continue;
			}
			state = 3; 
			bt = CreatBST(Place,n); // ��������������
			cout<<" ���о������� :  "<<endl;
			InOrder(bt);// �������������
			cout<<endl;
			MAtToList(Map ,G); // ת�����ڽӱ�
			//DispAdj(G);
			//cout<<"dfs   "<<endl;
			int v = 2;
			printf(" ���еĵ���·��  :   \n" );
			cout<<endl;
			int num;
			for(int i = 1; i<=n; i++) {

				cout<< "    ����     "<<i<<"   :  \n  ";
				memset(vis,0,sizeof(vis));
				memset(MapVex,0,sizeof(MapVex) );
				DFS(G,i,Place,bt);
				count1 = 0;
				cout<<endl<<endl;

				for(int j = 1; j<=n; j++) {
					int u = MapVex[j]->infor.number;
					TourMap[i][j] = u;
				}

			}

		} else if (num == 4 ) {
			if(state < 3){
				cout << "δ���ɵ���·�ߣ�" << endl;system("pause");PageInfor();
				continue;
			}
			TorMap.e = e;
			TorMap.n = n;
			cout<<endl;
			int StrageWeight[MAX][MAX];
			for(int i = 1; i <=n; i++) {
				for(int j = 1; j <=n; j++) {

					Gr[i][j].u = TourMap[i][j];// ���,
					Gr[i][j].v = TourMap[i][j+1];
					Gr[i][j].weight = Map.map[TourMap[i][j]][TourMap[i][j+1]];
					//StrageWeight[i][j] = Map.map[TourMap[i][j]][TourMap[i][j+1]];
				}


			}
			// ��ͼ 
			MatGraph GT[20]; // ����·��ͼ
			for(int i = 1; i<=n; i++)
				CreatMatGragh(GT[i],n,e,i,Gr);
			int number;

			int edgenum[MAX ];
			int ed = 0;
			for(int k = 1; k <=n; k++) {

				for(int i = 1; i <= n; i++) {

					for(int j = 1; j<=n; j++) {

						if(GT[k].map[i][j]!=inf) {
							edgenum[k]++;
						}
					}

				}

			}
			for(int i = 1; i<=n; i ++) {
				if(edgenum[i] == n-1) {
					number = i; // �ҵ���·ͼ;
					break;
				}
			}
			cout<<" ��ѵ���·�� "<<endl;
			for(int i = 1; i <=n; i++) {
				// ����������,�����������
				BSTNode *r = SearchBST(bt,TourMap[number][i]); // �������·��ͼ

				cout<<r->infor.PlaceName;
				if(i!=n)
					cout<<" -> ";

			}
			cout<<endl;

		} else if (num == 5 ) {
			if(state < 1){
				cout << "��ͼδ������" << endl;system("pause");PageInfor();
				continue;
			}
			bt = CreatBST(Place,n);
			BSTNode *pfind1 ,*pfind2;
			cout<<endl;
			Display(Place,n);
			cout<<endl;
			printf(" ������㾰�㣺\n");
			cin >> start;
			printf(" �����յ㾰�㣺\n");
			cin >>end;

			int Find_Start_Num = Find(start,Place,Map.n);
			int Find_End_Num = Find(end,Place,Map.n);

			pfind1 = SearchBST(bt,Find_Start_Num);//����
			pfind2 = SearchBST(bt,Find_End_Num); // �յ�
			if(!pfind1 && !pfind2){
				printf(" �������\n");system("pause");PageInfor();
				continue;
			}
			else
				cout<<pfind1->infor.PlaceName << " - > " <<pfind2->infor.PlaceName <<endl;
			if(Find_Start_Num != -1 && Find_End_Num != -1) {
				Dijkstra(Find_Start_Num,Map,bt,Find_End_Num);
				cout << endl;
			} else {
				printf(" �������\n");system("pause");PageInfor();
				continue;
			}

		} else if (num == 6 ) {
			if(state < 1){
				cout << "��ͼδ������" << endl;system("pause");PageInfor();
				continue;
			}
			bt = CreatBST(Place,n);
			Prim(Map ,1,bt );
		} else if (num == 7 ) {
			// ��ֹ����

			cout<<" �˳�ϵͳ�ɹ�!"<<endl;
			exit(0);
		}
		system("pause");
		PageInfor();
	}
	return;
}
int main() {
	
	int n = 13;
	int e = 14;
	DisplayMenu(n,e );
	return 0;

}


