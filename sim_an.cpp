#include<iostream>
#include<list>
#include<map>
#include<stdlib.h>
#include<time.h>
#include<cmath>
using namespace std ; 


class mod{
	int label;
	int i , j;
	list<int> connetedMods;
	public:
	mod(int l, int ii , int jj ){
		i=ii;
		j=jj;
		label=l;
	}	
	void update(int x , int y){
		i=x ; 
		j=y;
	}
	void add(int x){
		connetedMods.push_back(x);
	}
	friend int costFunct();
};

class net {

	int label; 
	list<int> mods;
	public: 
	net ( int l ) {		
		label =l ; 
	}
	void addMod(int m ) { 
		mods.push_back(m);
	}
};

int Solution[3][3];// cell number in the grid. 
list<net> nets ; 
map<int , mod*> modules;

int costFunct (){
	int cost=0;
	map<int , mod*>::iterator it = modules.begin();
	for ( ;it!=modules.end();it++){
		mod* m = it->second; 
		list <int> ::iterator it2= m->connetedMods.begin();
		for (;it2!=m->connetedMods.end();it2++){
			mod* m2= modules[(*it2)];
			cost+= abs(m->i - m2->i)+ abs(m->j-m2->j);
		}
	}
	return cost/2;
}

void pushSolution(int S[][3]){//This step is done to run cost function . 
	for( int i =0;i<3;i++){
		for(int j =0;j<3;j++){
			int l = S[i][j];
			if(modules.count(l)>0){
				modules[l]->update(i,j);
			}else {

				modules[l] = new mod(l,i,j);
			}
		}
	}
}
void applyConstraint(int S[][3]){
	S[2][2]=9;
}
void dumpSolution(int s[][3]){
	for( int i =0;i<3;i++){
		for( int j =0; j<3;j++){
			cout<<s[i][j]<<" " ;
					
		}
		cout <<endl;
	}

}
void initalSolution(int  S[][3]){
	for( int i =0;i<3;i++){
		for( int j =0; j<3;j++){
			S[i][j]=-1;		
		}
	}
	applyConstraint(S);
	for( int i=1;i<9;i++){
		int x , y;
		do{
			x=rand()%3;
			y=rand()%3;
		}while(S[x][y]!=-1);
		S[x][y]=i;
	}
}
void addNet(int i , int j ) {
	modules[i]->add(j);
	modules[j]->add(i);
}
void neigbour(int S[][3], int newS[][3]){
	for( int i =0;i<3;i++){
		for( int j =0; j<3;j++){
			newS[i][j]=S[i][j];		
		}
	}
 	for ( int i =0;i<2;i++){// 8 swaps
			int x1=rand()%3;
			int y1=rand()%3;
			int x2=rand()%3;
			int y2=rand()%3;
			if((x1==2 && y1==2 )||(x2==2&&y2==2))
				continue;
			int temp =newS[x1][y1];
			newS[x1][y1]=newS[x2][y2];
			newS[x2][y2]=temp;
	}
}

void copy(int S[][3], int newS[][3]){
	for( int i =0;i<3;i++){
		for( int j =0; j<3;j++){
			S[i][j]=newS[i][j];		
		}
	}
}
int hillClimbingCount =0;
void metropol(int S[][3], int T , int M){
	do {
		int newS [3][3];
		neigbour(S,newS);
		pushSolution(newS);
		int costNew= costFunct();
		pushSolution(S);
		int deltah= costNew-costFunct();
			float g = (float)(rand()%10000)/10000;
		if(deltah<0 ||  g<exp(- ((float)(deltah)/T))){
			hillClimbingCount++;
			copy(S,newS);
		}
		M=M-1;
	}while(M>0);
}

int  main(){
	initalSolution(Solution);
	srand (time(NULL));
	pushSolution(Solution);
	addNet(8,9);
	addNet(8,3);
	addNet(1,9);
	addNet(4,3);
	addNet(5,3);
	addNet(7,6);
	addNet(2,8);
	int T=10;
	float alpha =0.9;
	float beta=1.2;
	int MaxTime=1000;
	int M=10;

	int time =0;
	do{
		metropol(Solution,T,M);
		time +=M;
		T=T*alpha;
		M=M*beta;
	}while(time <MaxTime);


	pushSolution(Solution);
	cout<<"Placement of modules for a given netlist:"<<endl;
	cout<<"************"<<endl;
	dumpSolution(Solution);
	cout<<"************";
	cout<<"Cost of total wire length is minimized to "<<costFunct()<<endl;
	cout<<"Total hill climbing moves:"<<hillClimbingCount<<endl;
	
	
	return 0;
}





