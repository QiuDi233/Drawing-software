#include<iostream>
#include<cmath>
#include<algorithm>
#include"utils.h"

int C(int m, int n) {
    if (n == 0 || m == n) return 1;
    return C(m - 1, n) + C(m - 1, n - 1);
}
