/**
 🍪 the_hyp0cr1t3
 🍪 26.03.2022 23:23:34
**/
#include <chrono>
#include <iostream>
#include <random>

// random engine
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// Policy based data structure (pbds) ordered_set to support ordered statistic
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

template <class key, class value = __gnu_pbds::null_type, class cmp = std::less<key>>
using ordered_set = __gnu_pbds::tree<key, value, cmp, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;


// Generator parameters
const int q_min = 100000, q_max = q_min;
const int val_min = -1000, val_max = 1000;
const int INSERT_MISS = 30, ERASE_HIT = 70;


int main() {

    // [L, R] inclusive
    auto randInt = [](int L, int R) {
        return std::uniform_int_distribution<int>(L, R)(rng);
    };

    int q = randInt(q_min, q_max);
    std::cout << q << std::endl;

    ordered_set<int> os;
    while(q--) {
        int t = randInt(0, 1), val;
        int roll = randInt(0, 100);
        if(t == 0) { // insert
            if(os.size() and roll < INSERT_MISS) {
                int idx = randInt(0, os.size() - 1);
                val = *os.find_by_order(idx);
            } else {
                do {
                    val = randInt(val_min, val_max);
                } while(os.find(val) != os.end());
                os.insert(val);
            }

        } else {    // erase
            if(os.size() and roll < ERASE_HIT) {
                int idx = randInt(0, os.size() - 1);
                val = *os.find_by_order(idx);
                os.erase(val);
            } else {
                do {
                    val = randInt(val_min, val_max);
                } while(os.find(val) != os.end());
            }
        }

        std::cout << t << ' ' << val << std::endl;
    }

} // ~W