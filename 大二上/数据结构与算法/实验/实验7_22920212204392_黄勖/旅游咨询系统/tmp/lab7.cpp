#include <bits/stdc++.h> 
using namespace std;
#define N 20
int a[N][N],times[N][N];//a���ٽӾ���times���ڼ�¼ÿ���߱��߹��Ĵ���  
int len=1000000,rec[N*N],res[N*N];//len���ڼ�¼��·��,rec���ڼ�¼�߹����˳�� ,res���ڼ�¼���еĽ�� 
int n_v=0,n_e=0;//�ֱ��ǵ����ͱ��� 

void display()//չʾ���� 
{
	for(int i=0;i<n_v;i++)
	{
		for(int j=0;j<n_v;j++)
		{
			if(a[i][j]>0)
			    cout<<a[i][j]<<' ';
			else if(a[i][j]==-1)
			    cout<<"* ";
		}
		cout<<endl;
	}
}

bool judge(int des[],int n)//�ж��Ƿ�������ȫ��Ŀ�� 
{
	bool *flags=new bool[n_v];
	memset(flags, false,n_v);//��ʼ��
	for(int i=0;rec[i]!=-1;i++)
		flags[rec[i]]=true;
    for(int i=0;i<n;i++)
    	if(flags[des[i]]==false)
    	    return false;
	return true;
}

void find_way(int des[],int n,int x,int cou=0)//n��Ŀ�������x����㣬cou���Ѿ��߹�������� 
{
	if(judge(des,n))//������һ�ֽ�� 
	{
		int sum=0;//���ڼ�¼�ý�����ܳ��� 
		for(int i=1;rec[i]!=-1;i++)
		    sum+=a[rec[i-1]][rec[i]];
		if(sum<len)
		{
			cout<<"��ϲ���ҵ���һ��·:";
			len=sum;
			for(int i=0;rec[i]!=-1;i++)
			{
			    res[i]=rec[i];//���浱ǰ���Ž� 
			    cout<<res[i]+1<<' ';
		    }
		    cout<<"����·�ĳ�����:"<<len<<endl;
	    }
	    return;
	}
	
	
	for(int y=0;y<n_v;y++)
	{
		if(a[x][y]>0 && times[x][y]<2)//��������� 
		    {
		    	//cout<<"One step forward"<<y+1<<endl;
		    	times[x][y]++;
		    	times[y][x]++;
		    	rec[cou]=x;
		    	cou++;//���� 
		    	
		    	find_way(des,n,y,cou);
		    	
		    	times[x][y]--;
		    	times[y][x]--;
                rec[cou]=-1;
                cou--;//�ָ��ֳ�
                //cout<<"One step back"<<x+1<<endl;
			}
	}
}

int main()
{
	cout<<"�����뾰�����(20����)��";
	cin>>n_v;
	cout<<"�����뾰���ϵ������";
	cin>>n_e;
	for(int i=0;i<N*N;i++)
	rec[i]=res[i]=-1;
	
	for(int i=0;i<n_v;i++)//��ʼ�� 
	{
	    for(int j=0;j<n_v;j++)
	    {
	        a[i][j]=-1;
	        times[i][j]=0;
	    }
	}
	
	cout<<"��ɶ������������������Լ�Ȩֵ��"<<endl; 
	for(int i=0;i<n_e;i++)
	{
		int t1,t2,weight;
		cin>>t1>>t2>>weight;	
		a[t1-1][t2-1]=a[t2-1][t1-1]=weight;
	}
	cout<<"�ڽӾ���"<<endl; 
	display(); 
	cout<<"���������"<<endl;
	int ori;
	cin>>ori;
	ori--; 
	cout<<"������Ҫ�����ĵ�ĸ���"<<endl;
	int n;
	cin>>n; 
	cout<<"������Ҫ�����ĵ�����"<<endl;
	int *des=new int[n];
	for(int i=0;i<n;i++)
	{
		cin>>des[i];
		des[i]--;
	}
	
	cout<<"��ѯ��"<<endl;
	find_way(des,n,ori);
	cout<<"��̵�·����:"<<endl;
	for(int i=0;res[i]!=-1;i++)
	cout<<res[i]+1<<' ';
	cout<<"��̵�·��Ϊ:"<<len<<endl;
	return 0;
}

/*
9
9
1 2 1
2 3 1
3 4 1
4 5 1
5 6 1
6 2 1
6 7 2
6 8 3
8 9 4
2
4
1 4 7 9
*/

