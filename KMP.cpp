#include <bits/stdc++.h>
using namespace std;
using ll = long long;


/*
正常在一个字符串s里找到一个字符串p
暴力的话就一个一个比较 o(n * m)

现在我们利用p的特点
pi[i] 表示到i这个位置p最长的相等的前后缀长度
abababa
pi[0] a 0


*/

#include <bits/stdc++.h>
using namespace std;

vector<int> get_pi(string p) {
    int m = p.size();
    vector<int> pi(m);

    for(int i = 1; i < m; i++) {
        int j = pi[i - 1];

        while(j > 0 && p[i] != p[j]) {
            j = pi[j - 1];
        }

        if(p[i] == p[j]) j++;

        pi[i] = j;
    }

    return pi;
}

vector<int> kmp(string s, string p) {
    int n = s.size(), m = p.size();
    vector<int> pi = get_pi(p);
    vector<int> ans;

    int j = 0;

    for(int i = 0; i < n; i++) {
        while(j > 0 && s[i] != p[j]) {
            j = pi[j - 1];
        }

        if(s[i] == p[j]) j++;

        if(j == m) {
            ans.push_back(i - m + 1);
            j = pi[j - 1];
        }
    }

    return ans;
}

int main() {
    string s, p;
    cin >> s >> p;

    vector<int> ans = kmp(s, p);

    for(auto x : ans) {
        cout << x << '\n';
    }

    return 0;
}