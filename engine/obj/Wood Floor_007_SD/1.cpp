#include <bits/stdc++.h>

using namespace std;

int main(){
	vector <int> x;
	x.push_back(1);
	x.push_back(2);
	vector <int> y;
	y.push_back(11);
	y.push_back(21);
	
	for (vector<int>::iterator iter1 = x.begin(), vector<int>::iterator iter2 = y.begin(); iter1!=x.cend(); iter2++,iter1++)
		cout << *iter1<<" "<<*iter2<<"\n";
	
} 
