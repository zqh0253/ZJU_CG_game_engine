#include <bits/stdc++.h>
using namespace std;

enum x{
a,b,c 
};

class A{
	public:
		A(){
			a = 5;
		}
		int a;
		void p(){
			cout << this->a;
		}
};

int main(){
	A _;
	_.p();
} 
