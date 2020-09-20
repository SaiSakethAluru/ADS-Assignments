/*
Team details:
Sai Saketh Aluru - 16CS30030
K Sai Surya Teja - 16CS30015
Sasi Bhushan Seelaboyina - 16CS30032
*/

// Program to create random input data
#include<fstream>
#include<iostream>
#include<cstdlib>
#include<ctime>
  
using namespace std;

int main()
{
	int n;
	cout<<"Enter number of dimensions (n): ";
	cin>>n;
	cout<<"Generating data"<<endl;
	int N=500000, rand1, rand2;
	srand(time(0));
	ofstream fout;
	fout.open("data.txt");
	for(int i=0;i<N;i++){
		for(int j=0;j<n;j++){
			rand1=30*(float)rand()/RAND_MAX;
			fout<<rand1<<" "<<rand1<<" ";
		}
		fout<<endl;
	}
	fout.close();
	return 0;
}