/**
 🍪 the_hyp0cr1t3
 🍪 26.03.2022 23:37:53
**/
#include <fstream>
#include <iostream>
#include <red_black_tree.tpp>

int main(int argc, char *argv[]) {
    using namespace std;

    ifstream fin(argv[1]);
    int n; fin >> n;

    BBST::red_black_tree<int> s;
    while(n--) {
        int t, val; fin >> t >> val;
        cout << s.contains(val) << '\n';
        if(t == 0)
            s.insert(val);
        else
            s.erase(val);
    }

} // ~W