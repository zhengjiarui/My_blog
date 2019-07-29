#### CTS 2019 珍珠

日期：2019-05-22

题目来源：https://loj.ac/problem/3120

题目标签：生成函数、组合计数

###### 题目大意

有 $n$ 个在 $[1,D]$ 范围内的随机变量，求使得相同变量能够配成至少 $m$ 对的方案数（$1\le D\le 10^5，1\le m\le n\le 10^9$）

###### 解题思路

根据 ECNU 游族杯 2019 的抽乌龟那题，可以轻松获得 50+ 的部分分，并联想到权值出现次数为奇数的个数 $\le n-2m$

根据出现次数考虑生成函数，由 $\sum\frac {x^k}{k!}$ 联想到一些前置技能：
$$
\begin{aligned}
1+x+\frac {x^2} {2!}+\frac {x^3}{3!}+...&=e^x\\
1-x+\frac {x^2} {2!}-\frac {x^3}{3!}+...&=e^{-x}\\
1+\frac {x^2} {2!}+\frac {x^4} {4!}+\frac {x^6} {6!}+...&=\frac {e^x+e^{-x}} 2\\
x+\frac {x^3} {3!}+\frac {x^5} {5!}+\frac {x^7} {7!}+...&=\frac {e^x-e^{-x}} 2
\end{aligned}
$$
于是出现偶数次的生成函数为 $\frac {e^x+e^{-x}} 2$，出现奇数次的生成函数为 $\frac {e^x-e^{-x}} 2$。我们所求的答案如下，其中 $[x^n]$ 表示求 $x^n$ 的系数，$y$ 表示出现奇数次的个数。显然，只有当 $x$ 刚好被选择了 $n$ 个且 $y$ 不超过 $n-2m$ 个时，需要统计如答案，接着便可以进行一系列的化简：
$$
n!\sum_{k=0}^{n-2m}(\frac {e^x+e^{-x}} 2+y\frac {e^x-e^{-x}} 2)^D[x^n][y^k]\\
\begin{aligned}
&=n!\frac 1 {2^D}\sum_{k=0}^{n-2m}(e^x(1+y)+e^{-x}(1-y))^D[x^n][y^k]\\
\end{aligned}
$$
使用两次二项式定理，将 D 次方展开、交换求和顺序即可直接消去 $[x^n]$
$$
\begin{aligned}
&=n!\frac 1 {2^D}\sum_{k=0}^{n-2m}\sum_{i=0}^D(e^x(1+y))^i(e^{-x}(1-y))^{D-i}[x^n][y^k]\\
&=n!\frac 1 {2^D}\sum_{k=0}^{n-2m}\sum_{i=0}^D\binom Die^{(2i-D)x}(1+y)^i(1-y)^{D-i}[x^n][y^k]\\
&=\frac 1 {2^D}\sum_{i=0}^D\binom Di(2i-D)^n\sum_{k=0}^{n-2m}(1+y)^i(1-y)^{D-i}[y^k]\\
\end{aligned}
$$
当 $i$ 确定时，我们只需考虑 $S=\sum_{k=0}^{n-2m}(1+y)^i(1-y)^{D-i}[y^k]$ 的求法。当 $i=D$ 时暴力枚举 $k$ 即可，当 $i≠D$ 时可以将 $[y^k]$ 换个形式：
$$
S=(1+y)^i(1-y)^{D-i}(1+y+y^2+...)[y^{n-2m}]
$$
后面的 $1+y+y^2+...=\frac 1{1-y}$ 即为补足指数的部分，这样的好处在于多出可以抵消掉的 $1-y$ 与指数统一的 $y^{n-2m}\implies S=(1+y)^i(1-y)^{D-i-1}[y^{n-2m}]$

$(1+y)^i$ 取指数 $j$，$(1-y)^{D-i-1}$ 取指数 $n-2m-j$，因此 $S=\sum_{j=0}^k\binom ij\binom{d-i}{k-j}$，其中便于书写设 $d=D=1,k=n-2m$
$$
\sum_{j=0}^k\binom ij\binom{d-i}{k-j}=\sum_{j=0}^k\frac{i!}{j!(i-j)!}\cdot\frac{(d-i)!}{(k-j)!(d-k-i+j)}
$$
此时如果将 $j$ 和 $i-j$ 看成指数，那么已经是卷积形式了——对应指数为 $i$ 的系数，而且不需要对于每个 $i$ 单独计算，直接跑一遍 NTT 即可

最后别忘了 $i=D$ 需单独处理，以及前面剩下的系数 $\frac 1 {2^D}\binom Di(2i-D)^n$

###### 代码

终于推导完啦，现在只要把公式都敲进去就好了

```c++
#include <bits/stdc++.h>
#define dbug(x) cout<<#x<<"="<<x<<endl
using namespace std;
template <typename T> void read(T &t) {
	t=0; char ch=getchar(); int f=1;
	while ('0'>ch||ch>'9') { if (ch=='-') f=-1; ch=getchar(); }
	do {(t*=10)+=ch-'0';ch=getchar();} while ('0'<=ch&&ch<='9'); t*=f;
}
typedef long long ll;
const int maxn=(5e5)+10;
const ll mod=998244353;
int n,m,D,N,d,k;
ll jc[maxn],iv[maxn],ivjc[maxn];
ll a[maxn],b[maxn],fa[maxn],fb[maxn];
ll rev[maxn],ans,tmp;
ll ksm(ll x,ll y) {
	if (x<0) x+=mod;
	ll res=1;
	while (y) {
		if (y&1) res=res*x%mod;
		x=x*x%mod; y/=2;
	} return res;
}
ll C(int x,int y) {
	return jc[x]*ivjc[y]%mod*ivjc[x-y]%mod;
}
void init(int n) {
	N=1; int lg=0;
	while (N<n) N*=2,lg++;
	for (int i=1;i<=N;i++)
		for (int j=1,ii=i;j<=lg;j++,ii/=2)
			rev[i]=rev[i]*2+(ii%2);
}
void fft(ll *a,ll *out,int flag) {
	static ll tmp[maxn];
	for (int i=0;i<N;i++) tmp[rev[i]]=a[i];
	for (int step=1;step<N;step*=2) {
		ll wn=ksm(3,(mod-1)/(step*2));
		if (flag==-1) wn=ksm(wn,mod-2);
		for (int i=0;i<N;i+=step*2) {
			ll w=1;
			for (int k=i;k<i+step;k++) {
				ll u=tmp[k],v=tmp[k+step]*w%mod;
				tmp[k]=(u+v)%mod; tmp[k+step]=(u-v+mod)%mod;
				w=w*wn%mod;
			}
		}
	}
	for (int i=0;i<N;i++) out[i]=tmp[i];
	if (flag==-1) {
		ll t=ksm(N,mod-2);
		for (int i=0;i<N;i++)
			out[i]=out[i]*t%mod;
	}
}
int main() {
	jc[0]=iv[0]=ivjc[0]=1;
	jc[1]=iv[1]=ivjc[1]=1;
	for (int i=2;i<maxn/5;i++) {
		jc[i]=jc[i-1]*i%mod;
		iv[i]=(mod-mod/i)*iv[mod%i]%mod;
		ivjc[i]=ivjc[i-1]*iv[i]%mod;
	}
	read(D); read(n); read(m); d=D-1; k=min(D,n-2*m);
	for (int j=0;j<=k;j++) {
		a[j]=ivjc[j]*ivjc[k-j]%mod;
		if ((k-j)&1) a[j]=mod-a[j];
	}
	for (int j=0;j<=d-k;j++) b[j]=ivjc[j]*ivjc[d-k-j]%mod;
	init(d*2);
	fft(a,fa,1); fft(b,fb,1);
	for (int i=0;i<N;i++) fa[i]=fa[i]*fb[i]%mod;
	fft(fa,a,-1);
	for (int i=0;i<=d;i++) a[i]=a[i]*jc[i]%mod*jc[d-i]%mod;
	for (int i=0;i<=d;i++)
		ans=(ans+a[i]*C(D,i)%mod*ksm(2*i-D,n)%mod)%mod;
	for (int i=0;i<=k;i++) tmp=(tmp+C(D,i))%mod;
	ans=(ans+tmp*ksm(D,n)%mod)%mod;
	printf("%lld\n",ans*ksm(ksm(2,mod-2),D)%mod);
	return 0;
}
```

