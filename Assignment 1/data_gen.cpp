#include<fstream>
#include<iostream>
#include<cstdlib>
#include<ctime>
  
using namespace std;

int main()
{
	int n;
	cin>>n;
	int N=10000, rand1, rand2;
	srand(time(0));
	ofstream fout;
	fout.open("data.txt");
	for(int i=0;i<N;i++){
		for(int j=0;j<n;j++){
			rand1=20*(float)rand()/RAND_MAX;
			rand2=20*(float)rand()/RAND_MAX;
			fout<<min(rand1,rand2)<<" "<<max(rand1,rand2)<<" ";
		}
		fout<<endl;
	}
	fout.close();
	return 0;
}