/**
 🍪 the_hyp0cr1t3
 🍪 26.03.2022 23:35:37
**/
#include <fstream>
#include <iostream>
#include <set>

int main(int argc, char *argv[]) {
    using namespace std;
    ifstream fin(argv[1]);
    int n; fin >> n;

    std::set<int> s;
    while(n--) {
        int t, val; fin >> t >> val;
        cout << s.count(val) << '\n';
        if(t == 0)
            s.insert(val);
        else
            s.erase(val);
    }

} // ~W