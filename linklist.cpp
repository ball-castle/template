#include <bits/stdc++.h>
using namespace std;

typedef int ElemType;
 
/*
p -> data (int) 
Node *p = new Node (p like a string connect the node)
Node * is a type like int
new dynamically distibute the memory
*/

struct LinkNode {
    ElemType data;
    LinkNode* next;
};

// initial linklist 
void Initlist(LinkNode *&L) {
    L = new LinkNode;	
    L->next = nullptr;
}

// return the length 
int ListLength(LinkNode *L) {
    int len = 0;
    LinkNode *p = L->next;
    while(p) {
        len++;
        p = p->next;
    }
    return len;
}

// insert e into pos i
bool ListInsert(LinkNode *&L, int i, ElemType e) {
    LinkNode *p = L;
    int j = 0;
    while (p && j < i - 1) {
        p = p->next;
        j++;
    }
    if(!p || j > i - 1) return false;
    LinkNode *s = new LinkNode;
    s->data = e;
    s->next = p->next;
    p->next = s;
    return true;
}

// delete i ele
bool ListDelete(LinkNode *&L, int i, ElemType &e) {
    LinkNode *p = L;
    int j = 0;
    while(p->next && j < i - 1) {
        p = p->next;
        j++;
    }
    if(!p->next || j > i - 1) return false;
    LinkNode *q = p->next;
    e = q->data;
    p->next = q->next;
    delete q;
    return true;
}

// print linklist
void DispList(LinkNode *L) {
    LinkNode *p = L->next;
    while (p) {
        cout << p->data << " ";
        p = p->next;
    }
    cout << endl;
}

// make linklist by insert io the first 
void CreateListT(LinkNode *&L, ElemType A[], int n) {
    Initlist(L);
    for (int i = 0; i < n; i++) {
        LinkNode *s = new LinkNode;
        s->data = A[i];
        s->next = L->next;
        L->next = s;
    }
}

// delete the max
void DelMaxNode(LinkNode *&L) {
    if (!L->next) return;

    LinkNode *pre = L, *p = L->next;
    LinkNode *maxPre = pre, *maxP = p;
    ElemType maxValue = p->data;
    while(p) {
        if(p->data > maxValue) {
            maxValue = p->data;
            maxPre = pre;
            maxP = p;
        }
        pre = p;
        p = p->next;
    }
    if(maxP) {
        maxPre->next = maxP->next;
        delete maxP;
    }
}


int main() {
    int arr[] = {9,5,8,7,5,10,4,3,2,3,1};
    int n = sizeof(arr)/sizeof(arr[0]);
    LinkNode *L;
    CreateListT(L, arr, n);
    
    cout << "链表为：";
    DispList(L);
	cout << "链表长度为：" << ListLength(L) << endl;

    int delVal;
    if(ListDelete(L, 5, delVal))
        cout << "删除第5个元素：" << delVal << endl;
    else
        cout << "删除第5个元素失败" << endl;

    if (ListInsert(L, 7, 4))
        cout << "在第7个位置插入4成功" << endl;
    else
        cout << "插入失败" << endl;

    DelMaxNode(L);

    cout << "最终链表为：";
    DispList(L);

    return 0;
}
