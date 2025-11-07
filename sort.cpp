#include <iostream>
#include <algorithm>
using namespace std;
int a[10] = {0, 4, 8, 1, 9}, b[10] = {0, 10, 2, 315, 14};


bool cmp(int a, int b) {return a > b;}

struct S {
	int score;
	char name[20];
};

S m[10] = {
	{0, "0"},
	{1, "xiaoming"},
	{2, "xiaohong"}
};



int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
//	sort(a + 1, a + 5, cmp);
	sort(a + 1, a + 5, [&](int a, int b) {return a > b;});
//	sort(a + 1, a + 5, [&](int i, int j) {return a[i] > a[j];}); // wrong i, j present the value of a 
	
	sort(m + 1, m + 3, [&](S a, S b) {return a.score > b.score;});
	for(int i = 1; i <= 2; i++) cout << m[i].score << " " << m[i].name << endl;
//	for(int i = 1; i <= 4; i++) cout << a[i] << " ";
	cout << endl;
	
    return 0;
}
