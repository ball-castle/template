#include <bits/stdc++.h>
#define ll long long 
using namespace std;



const int N = 1e5 + 5; //最大数组长度
int a[N];      //原数组
int tree[N*4]; //线段树数组

// 建树
void build(int node, int l, int r) {
    if(l == r) {
        tree[node] = a[l];
        return;
    }
    int mid = (l + r) >> 1;
    build(node*2, l, mid);         //左子树
    build(node*2+1, mid+1, r);     //右子树
    tree[node] = tree[node*2] + tree[node*2+1]; //父节点合并信息
}

// 单点修改，把 a[pos] 变成 val
void update(int node, int l, int r, int pos, int val) {
    if(l == r) {
        tree[node] = val;
        return;
    }
    int mid = (l + r) >> 1;
    if(pos <= mid) update(node*2, l, mid, pos, val);
    else update(node*2+1, mid+1, r, pos, val);
    tree[node] = tree[node*2] + tree[node*2+1];
}

// 区间查询，查询 [L, R] 的和
int query(int node, int l, int r, int L, int R) {
    if(R < l || r < L) return 0;           //不相交
    if(L <= l && r <= R) return tree[node];//完全包含
    int mid = (l + r) >> 1;
    return query(node*2, l, mid, L, R) +
           query(node*2+1, mid+1, r, L, R);
}
