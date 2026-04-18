#include <bits/stdc++.h>
using namespace std;

int main() {
    // ========== 1. vector 动态数组 ==========
    vector<int> vec = {1, 2, 3};
    vec.push_back(4);           // 尾部插入
    vec.emplace_back(5);        // C++11，更快，原地构造
    vec.pop_back();             // 尾部删除
    cout << vec[0] << " " << vec.size() << endl;
    // 遍历
    for (int x : vec) cout << x << " ";
    cout << endl;
    
    // ========== 2. string 字符串 ==========
    string s = "hello";
    s += " world";              // 拼接
    s.push_back('!');
    cout << s.substr(0, 5) << endl;  // 截取 [0,5)
    
    // ========== 3. stack 栈 (LIFO) ==========
    stack<int> st;
    st.push(1);
    st.push(2);
    while (!st.empty()) {
        cout << st.top() << " ";  // 访问栈顶
        st.pop();                  // 删除栈顶
    }
    cout << endl;
    
    // ========== 4. queue 队列 (FIFO) ==========
    queue<int> q;
    q.push(1);
    q.push(2);
    while (!q.empty()) {
        cout << q.front() << " "; // 队首
        q.pop();
    }
    cout << endl;
    
    // ========== 5. priority_queue 优先队列 (堆) ==========
    // 大根堆 (默认)
    priority_queue<int> big;
    big.push(3); big.push(1); big.push(2);
    cout << big.top() << endl;  // 输出 3 (最大)
    
    
    // ========== 6. set / multiset 有序集合 ==========
    set<int> se;                // 有序，去重
    se.insert(1);
    se.insert(1);               // 重复插入无效
    auto it = se.find(1);       // 查找，返回迭代器
    if (it != se.end()) se.erase(it);  // 删除
    
    multiset<int> mse;          // 允许重复
    
    // ========== 7. unordered_set 哈希集合 (O(1)) ==========
    unordered_set<int> us;
    us.insert(1);
    if (us.count(1)) cout << "exist" << endl;  // 存在性检查 O(1)
    us.erase(1);
    
    // ========== 8. map / unordered_map 映射 ==========
    // 有序 map (红黑树)
    map<string, int> mp;
    mp["alice"] = 90;
    mp["bob"] = 85;
    for (auto &[k, v] : mp) {  // C++17 结构化绑定，按键升序
        cout << k << ":" << v << endl;
    }
    
    // 哈希 map (O(1))，常用
    unordered_map<int, int> ump;
    ump[1] = 100;
    cout << ump[1] << endl;
    
    // ========== 9. deque 双端队列 ==========
    deque<int> dq;
    dq.push_back(1);   // 尾部
    dq.push_front(2);  // 头部
    dq.pop_back();
    dq.pop_front();
    
    // ========== 10. pair / tuple ==========
    pair<int, string> p = {1, "one"};
    cout << p.first << " " << p.second << endl;
    
    tuple<int, int, int> t = {1, 2, 3};
    auto [x, y, z] = t;  // C++17 解包
    
    // ========== 11. algorithm 常用算法 ==========
    vector<int> a = {3, 1, 4, 1, 5};
    sort(a.begin(), a.end());                    // 排序
    reverse(a.begin(), a.end());                 // 翻转
    int mx = *max_element(a.begin(), a.end());   // 最大值
    int mn = *min_element(a.begin(), a.end());   // 最小值
    
    // 二分查找 (必须有序)
    sort(a.begin(), a.end());
    bool found = binary_search(a.begin(), a.end(), 3);
    // lower_bound: 第一个 >= x 的位置
    auto pos = lower_bound(a.begin(), a.end(), 3);
    if (pos != a.end()) cout << *pos << endl;
    
    // 去重 (配合erase)
    a.erase(unique(a.begin(), a.end()), a.end());
    
    return 0;
}