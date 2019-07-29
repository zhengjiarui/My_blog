#include <bits/stdc++.h>

#define RG register
#define IL inline
#define ll long long
#define ld long double
#define ui unsigned int
#define ull unsigned long long
#define LL long long
//#define FILE
//#define DEBUG

namespace stdoier
{
template <typename T>
IL T max(RG const T &a, RG const T &b)
{
    if (a > b)
        return a;
    else
        return b;
}

template <typename T>
IL T min(RG const T &a, RG const T &b)
{
    if (a < b)
        return a;
    else
        return b;
}

template <typename T>
IL void cmin(RG T &a, RG const T &b)
{
    if (a > b)
        a = b;
}

template <typename T>
IL void cmax(RG T &a, RG const T &b)
{
    if (a < b)
        a = b;
}
} // namespace stdoier

using namespace stdoier;

namespace io
{
const int MaxBuff = 1 << 15;
const int MaxOut = 1 << 24;
char b[MaxBuff], *S = b, *T = b;

#define getc() (S == T && (T = (S = b) + fread(b, 1, MaxBuff, stdin), S == T) ? 0 : *S++)

template <class Type>
IL Type read()
{
    RG char ch;
    RG Type ans = 0;
    RG bool neg = 0;
    while (ch = getc(), (ch < '0' || ch > '9') && ch != '-')
        ;
    ch == '-' ? neg = 1 : ans = ch - '0';
    while (ch = getc(), '0' <= ch && ch <= '9')
        ans = ans * 10 + ch - '0';
    return neg ? -ans : ans;
}

IL int gets(RG char *s)
{
    RG char *iter = s;
    while (*iter = getc(), *iter == ' ' || *iter == '\n' || *iter == '\r')
        ;
    while (*++iter = getc(), *iter && *iter != ' ' && *iter != '\n' && *iter != '\r')
        ;
    *iter = 0;
    return iter - s;
}

char buff[MaxOut], *iter = buff;

template <class T>
IL void writeln(RG T x, RG char ch = '\n')
{
    static int stack[110];
    RG int O = 0;
    RG char *iter = io::iter;
    if (!x)
        *iter++ = '0';
    else
    {
        (x < 0) ? x = -x, *iter++ = '-' : 1;
        for (; x; x /= 10)
            stack[++O] = x % 10;
        for (; O; *iter++ = '0' + stack[O--])
            ;
    }
    *iter++ = ch, io::iter = iter;
}

template <class T>
IL void write(RG T x)
{
    static int stack[110];
    RG int O = 0;
    RG char *iter = io::iter;
    if (!x)
        *iter++ = '0';
    else
    {
        (x < 0) ? x = -x, *iter++ = '-' : 1;
        for (; x; x /= 10)
            stack[++O] = x % 10;
        for (; O; *iter++ = '0' + stack[O--])
            ;
    }
    io::iter = iter;
}

IL void puts(RG const char *s)
{
    while (*s)
        *iter++ = *s++;
}

struct Output
{
    ~Output() { fwrite(buff, 1, iter - buff, stdout), iter = buff; }
} output_hlpr;
} // namespace io

namespace solve_the_problem
{
int (*in)() = io::read<int>;
const int N = 800003, mod = 998244353, g = 3, gi = 332748118;

int D, n, m, F[N], G[N];
int rev[N], fac[N], inv[N], invfac[N], ans;

inline int POW(int a, int b)
{
    int res = 1;
    while (b)
    {
        if (b & 1)
            res = (ll)res * a % mod;
        a = (ll)a * a % mod;
        b >>= 1;
    }
    return res;
}

inline void init(int n)
{
    fac[0] = 1;
    for (int i = 1; i <= n; i++)
        fac[i] = (ll)fac[i - 1] * i % mod;
    invfac[n] = POW(fac[n], mod - 2);
    for (int i = n; i; i--)
    {
        invfac[i - 1] = (ll)i * invfac[i] % mod;
        inv[i] = (ll)invfac[i] * fac[i - 1] % mod;
    }
}

inline int calrev(int n)
{
    int limit = 1, L = -1;
    while (limit <= n)
    {
        limit <<= 1;
        L++;
    }
    for (int i = 0; i < limit; i++)
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << L);
    return limit;
}

inline void NTT(int *A, int limit, int type)
{
    for (int i = 0; i < limit; i++)
        if (i < rev[i])
            std ::swap(A[i], A[rev[i]]);
    for (int mid = 1; mid < limit; mid <<= 1)
    {
        int Wn = POW(type == 1 ? g : gi, (mod - 1) / (mid << 1));
        for (int j = 0; j < limit; j += (mid << 1))
        {
            int w = 1;
            for (int k = 0; k < mid; k++, w = (ll)w * Wn % mod)
            {
                int x = A[j + k], y = (ll)w * A[j + k + mid] % mod;
                A[j + k] = (x + y) % mod;
                A[j + k + mid] = (x - y + mod) % mod;
            }
        }
    }
    if (type == -1)
    {
        int inv = POW(limit, mod - 2);
        for (int i = 0; i < limit; i++)
            A[i] = (ll)A[i] * inv % mod;
    }
}

void main()
{
    D = in(), n = in(), m = in();
    if (n < 2 * m)
    {
        io::puts("0");
        return;
    }
    if (D <= n - 2 * m)
    {
        io::writeln(POW(D, n));
        return;
    }
    init(D);
    for (int i = 0; i <= D; i++)
    {
        F[i] = (ll)POW((D - 2 * i + mod) % mod, n) * invfac[i] % mod;
        if (i & 1)
            F[i] = mod - F[i];
        G[i] = invfac[i];
    }
    int limit = calrev(D << 1);
    NTT(F, limit, 1);
    NTT(G, limit, 1);
    for (int i = 0; i < limit; i++)
        F[i] = (ll)F[i] * G[i] % mod;
    NTT(F, limit, -1);
    for (int i = 0; i < limit; i++)
        G[i] = 0;
    for (int i = D + 1; i < limit; i++)
        F[i] = 0;
    for (int i = 0; i <= D; i++)
    {
        F[i] = (ll)F[i] * fac[i] % mod * fac[D] % mod * POW(2, mod - 1 - i) % mod * invfac[D - i] % mod;
        G[D - i] = (i & 1) ? (mod - invfac[i]) : invfac[i];
    }
    NTT(F, limit, 1);
    NTT(G, limit, 1);
    for (int i = 0; i < limit; i++)
        F[i] = (ll)F[i] * G[i] % mod;
    NTT(F, limit, -1);
    for (int i = 0; i <= n - 2 * m; i++)
        ans = (ans + (ll)F[i + D] * invfac[i] % mod) % mod;
    io::writeln(ans);
}
} // namespace solve_the_problem

int main()
{
    solve_the_problem ::main();

    return 0;
}
