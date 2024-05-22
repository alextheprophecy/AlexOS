
int digit_count(int n) {
    int r = 1;
    if (n < 0) n = -n;//(n == INT_MIN) ? INT_MAX: -n;
    while (n > 9) {
        n /= 10;
        r++;
    }
    return r;
}