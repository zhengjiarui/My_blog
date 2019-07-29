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
int (*in)() = io ::read<int>;
const int maxn = (5e5) + 10;
const ll mod = 998244353;

int n, m, D, N, d, k;
ll jc[maxn], iv[maxn], ivjc[maxn];
ll a[maxn], b[maxn], fa[maxn], fb[maxn];
ll rev[maxn], ans, tmp;

ll ksm(ll x, ll y)
{
    if (x < 0)
        x += mod;
    ll res = 1;
    while (y)
    {
        if (y & 1)
            res = res * x % mod;
        x = x * x % mod;
        y /= 2;
    }
    return res;
}

ll C(int x, int y)
{
    return jc[x] * ivjc[y] % mod * ivjc[x - y] % mod;
}

void init(int n)
{
    N = 1;
    int lg = 0;
    while (N < n)
        N *= 2, lg++;
    for (int i = 1; i <= N; i++)
        for (int j = 1, ii = i; j <= lg; j++, ii /= 2)
            rev[i] = rev[i] * 2 + (ii % 2);
}

void fft(ll *a, ll *out, int flag)
{
    static ll tmp[maxn];
    for (int i = 0; i < N; i++)
        tmp[rev[i]] = a[i];
    for (int step = 1; step < N; step *= 2)
    {
        ll wn = ksm(3, (mod - 1) / (step * 2));
        if (flag == -1)
            wn = ksm(wn, mod - 2);
        for (int i = 0; i < N; i += step * 2)
        {
            ll w = 1;
            for (int k = i; k < i + step; k++)
            {
                ll u = tmp[k], v = tmp[k + step] * w % mod;
                tmp[k] = (u + v) % mod;
                tmp[k + step] = (u - v + mod) % mod;
                w = w * wn % mod;
            }
        }
    }
    for (int i = 0; i < N; i++)
        out[i] = tmp[i];
    if (flag == -1)
    {
        ll t = ksm(N, mod - 2);
        for (int i = 0; i < N; i++)
            out[i] = out[i] * t % mod;
    }
}

void main()
{
    jc[0] = iv[0] = ivjc[0] = 1;
    jc[1] = iv[1] = ivjc[1] = 1;
    for (int i = 2; i < maxn / 5; i++)
    {
        jc[i] = jc[i - 1] * i % mod;
        iv[i] = (mod - mod / i) * iv[mod % i] % mod;
        ivjc[i] = ivjc[i - 1] * iv[i] % mod;
    }
    D = in(), n = in(), m = in();
    d = D - 1;
    k = min(D, n - 2 * m);
    for (int j = 0; j <= k; j++)
    {
        a[j] = ivjc[j] * ivjc[k - j] % mod;
        if ((k - j) & 1)
            a[j] = mod - a[j];
    }
    for (int j = 0; j <= d - k; j++)
        b[j] = ivjc[j] * ivjc[d - k - j] % mod;
    init(d * 2);
    fft(a, fa, 1);
    fft(b, fb, 1);
    for (int i = 0; i < N; i++)
        fa[i] = fa[i] * fb[i] % mod;
    fft(fa, a, -1);
    for (int i = 0; i <= d; i++)
        a[i] = a[i] * jc[i] % mod * jc[d - i] % mod;
    for (int i = 0; i <= d; i++)
        ans = (ans + a[i] * C(D, i) % mod * ksm(2 * i - D, n) % mod) % mod;
    for (int i = 0; i <= k; i++)
        tmp = (tmp + C(D, i)) % mod;
    ans = (ans + tmp * ksm(D, n) % mod) % mod;
    io ::writeln(ans * ksm(ksm(2, mod - 2), D) % mod);
}
} // namespace solve_the_problem

int main()
{
    solve_the_problem ::main();

    return 0;
}
