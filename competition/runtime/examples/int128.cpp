#include "../int128_input.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    string s;
    while (cin >> s) {
        __int128 x = parse_i128(s);
        cout << to_string_i128(x) << '\n';
    }
}
