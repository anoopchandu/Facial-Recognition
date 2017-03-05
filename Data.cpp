#include <cstdio>
#include <cstring>
#include <map>

using namespace std;

class Data
{
public:
	double inp[300][960];
	double out[300][20];
	double vinp[300][960];
	double vout[300][20];
	char train[100], valid1[100], valid2[100];
	int prob;

	int dataSize,dataSize1,dp,dp1;
	map<string,int> m;

	void Init(int p)
	{
		int i;
		prob =p;
		if(p==3)
		{
			sprintf(train,"lists/all_train.list");
			sprintf(valid1,"lists/all_test1.list");
			sprintf(valid2,"lists/all_test2.list");
		}
		else if(p==2)
		{
			sprintf(train,"lists/straighteven_train.list");
			sprintf(valid1,"lists/straighteven_test1.list");
			sprintf(valid2,"lists/straighteven_test2.list");
		}
		else if(p==1)
		{
			sprintf(train,"lists/straightrnd_train.list");
			sprintf(valid1,"lists/straightrnd_test1.list");
			sprintf(valid2,"lists/straightrnd_test2.list");
		}
		string s[20] ={"an2i","at33","boland","bpm","ch4f","cheyer","choon","danieln","glickman","karyadi","kawamura","kk49","megak","mitchell","night","phoebe""saavik","steffi","sz24","tammo"};
		for(i=0;i<20;i++)
			m[s[i]]=i;
	}

	void readImage(char* src, unsigned char* buf, double *d)
	{
		int i;
		char p;
		FILE *f;
		f = fopen(src,"r");
		// printf("%s",src);
		// fflush(stdout);
		fread(buf,1,13,f);
		fread(buf,1,960,f);
		fclose(f);

		for(i=0;i<960;i++)
			d[i]=((double)buf[i])/156;
		
		// for(i=0;i<960;i++)
		// 	printf("%lf ",d[i]);
		// printf("\n\n");
	}

	void readOutput(char *src, double *d)
	{
		int j;
		if(prob==3)
		{
			for(j=0;j<4;j++)
				d[j]=0.1;
			switch(*(strchr(src,'_')+1))
			{
				case 's':
					d[0] = 0.9;
					break;
				case 'u':
					d[1] = 0.9;
					break;
				case 'l':
					d[2] = 0.9;
					break;
				case 'r':
					d[3] = 0.9;
			}
		}
		else if(prob==2)
		{
			for(j=0;j<20;j++)
				d[j]=0.1;
			char *x = strchr(src,'/')+1;
			char *y=strrchr(src,'/');
			char s[20];
			j=0;
			while(x<y)
			{
			    s[j]=*x;
			    j++;
			    x++;
			}
			s[j]='\0';
			d[m[s]]=0.9;

		}
		else if(prob = 1)
		{
			d[0]=d[1]=0.1;
			char *x = strrchr(src,'_')-1;
			if((*x)=='n')
				d[0]=0.9;
			else
				d[1]=0.9;
		}

	}

	int loadTrainingData()
	{
		int i,j;
		unsigned char buf[1000];
		FILE *f;
		char src[200];

		f = fopen(train,"r");
		// printf("%s",train);
		// fflush(stdout);
		for(i=0;true;i++)
		{
			fgets(src,200,f);
			if(feof(f))
				break;
			src[strlen(src)-1]=0;
			readImage(src,buf,inp[i]);
			readOutput(src,out[i]);
		}
		fclose(f);
		dp = 0;
		dataSize = i;
		// for(i=0;i<1;i++)
		// {
		// 	for(j=0;j<960;j++)
		// 		printf("%lf\n",inp[i][j]);
		// 	printf("\n\n\n");
		// 	for(j=0;j<4;j++)
		// 		printf("%lf ",out[i][j]);
		// 	printf("\n---------------\n\n\n");
		// }
		return i;

	}

	int loadValidationData(int st)
	{
		dp1 = 0;
		int i,j;
		unsigned char buf[1000];
		char src[200];
		FILE *f;

		if(st==0)
			f = fopen(valid1,"r");
		else
			f = fopen(valid2,"r");

		for(i=0;true;i++)
		{
			fgets(src,200,f);
			if(feof(f))
				break;
			src[strlen(src)-1]=0;

			readImage(src,buf,vinp[i]);
			readOutput(src,vout[i]);
		}
		fclose(f);

		dataSize1 = i;
		return i;
	}

	pair<double *,double *> getTrainingData()
	{
		pair<double *,double*> p = make_pair(inp[dp],out[dp]);
		dp = (dp+1)%dataSize;
		return p;
	}

	pair<double *,double *> getValidationData()
	{
		pair<double *,double*> p = make_pair(vinp[dp1],vout[dp1]);
		dp1 = (dp1+1)%dataSize1;
		return p;
	}
};
