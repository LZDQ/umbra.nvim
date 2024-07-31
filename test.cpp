#include<cstdio>
#include<algorithm>
#include<cstring>
#include<cmath>
#include<map>
#include<vector>
using namespace std;
typedef long long ll;
typedef vector<int> Poly;
const int MAXN=1e5+5,mod=998244353,G=3,invG=(mod+1)/3;
int n;
ll fac[MAXN],inv[MAXN];
ll Fstpw(ll a,int b){
	ll res=1;
	while(b){
		if(b&1) res=res*a%mod;
		b>>=1;
		a=a*a%mod;
	}
	return res;
}
void Butterfly(Poly &a){
	static const int g = G;
    int n(a.size()), h(__builtin_ctz(n));

    static bool first = true;
    static int sum_e[30];  // sum_ie[i] = es[0] * ... * es[i - 1] * ies[i]
    if (first) {
        first = false;
        int es[30], ies[30];  // es[i]^(2^(2+i)) == 1
        int cnt2 = __builtin_ctz(mod - 1);
        ll e = Fstpw(g, mod - 1 >> cnt2), ie = Fstpw(e, mod - 2);
        for (int i = cnt2; i >= 2; i--) {
            // e^(2^i) == 1
            es[i - 2] = e;
            ies[i - 2] = ie;
			e = e * e % mod;
			ie = ie * ie % mod;
        }
        ll nw = 1;
        for (int i = 0; i <= cnt2 - 2; i++) {
            sum_e[i] = es[i] * nw % mod;
			nw = nw * ies[i] % mod;
        }
    }
    for (int ph = 1; ph <= h; ph++) {
        int w = 1 << (ph - 1), p = 1 << (h - ph);
        ll nw = 1;
        for (int s = 0; s < w; s++) {
            int offset = s << (h - ph + 1);
            for (int i = 0; i < p; i++) {
                int l = a[i + offset];
                int r = a[i + offset + p] * nw % mod;
                a[i + offset] = (l + r) % mod;
                a[i + offset + p] = (mod + l - r) % mod;
            }
            nw = nw * sum_e[__builtin_ctz(~(unsigned int)(s))] % mod;
        }
    }
	return ;
}
void Butterfly_inv(Poly &a) {
    static const int g = G;
    int n(a.size()), h(__builtin_ctz(n));

    static bool first = true;
    static int sum_ie[30];  // sum_ie[i] = es[0] * ... * es[i - 1] * ies[i]
    if (first) {
        first = false;
        int es[30], ies[30];  // es[i]^(2^(2+i)) == 1
        int cnt2 = __builtin_ctz(mod - 1);
        ll e = Fstpw(g, mod - 1 >> cnt2), ie = Fstpw(e, mod - 2);
        for (int i = cnt2; i >= 2; i--) {
            // e^(2^i) == 1
            es[i - 2] = e;
            ies[i - 2] = ie;
			e = e * e % mod;
			ie = ie * ie % mod;
        }
        ll nw = 1;
        for (int i = 0; i <= cnt2 - 2; i++) {
            sum_ie[i] = ies[i] * nw % mod;
			nw = nw * es[i] % mod;
        }
    }

    for (int ph = h; ph; ph--) {
        int w = 1 << (ph - 1), p = 1 << (h - ph);
        ll inw = 1;
        for (int s = 0; s < w; s++) {
            int offset = s << (h - ph + 1);
            for (int i = 0; i < p; i++) {
                int l = a[i + offset];
                int r = a[i + offset + p];
                a[i + offset] = (l + r) % mod;
                a[i + offset + p] = (mod + l - r) % mod * inw % mod;
            }
            inw = inw * sum_ie[__builtin_ctz(~(unsigned int)(s))] % mod;
        }
    }
	return ;
}
Poly operator *(Poly a,Poly b){
	if(a.empty() || b.empty()) return Poly{};
    int n(a.size() - 1), m(b.size() - 1);
    if (min(n, m) <= 30) {
        if (n < m) {
            swap(n, m);
            swap(a, b);
        }
        Poly ans(n + m + 1);
        for (int i = 0; i <= n; i++)
            for (int j = 0; j <= m; j++)
                ans[i + j] = (ans[i + j] + 1ll * a[i] * b[j]) % mod;
        return ans;
    }
    int z = 1;
	while(z <= n + m) z <<= 1;
    a.resize(z);
    Butterfly(a);
    b.resize(z);
    Butterfly(b);
    for (int i = 0; i < z; i++) 
        a[i] = 1ll * a[i] * b[i] % mod;
    Butterfly_inv(a);
    a.resize(n + m + 1);
    ll iz = Fstpw(z, mod - 2);
    for (int i = 0; i <= n + m; i++)
		a[i] = a[i] * iz % mod;
    return a;
}
Poly operator +(Poly a,Poly b){
	if(a.size()<b.size()) swap(a,b);
	for(int i=0; i<b.size(); i++)
		a[i]=(a[i]+b[i])%mod;
	return a;
}
Poly operator -(Poly a,Poly b){
	if(a.size()<b.size()) a.resize(b.size());
	for(int i=0; i<b.size(); i++)
		a[i]=(a[i]-b[i]+mod)%mod;
	return a;
}
Poly Shift(Poly a,int b){
	if(a.empty()) return a;
	a.resize(a.size()+b);
	for(int i=a.size()-1; i>=b; i--)
		a[i]=a[i-b];
	for(int i=0; i<b; i++)
		a[i]=0;
	return a;
}
void ntt(int *a,int n,int tp){
	int bit=0;
	while(1<<bit<n) bit++;
	static int rev[MAXN<<2];
	for(int i=1; i<n; i++){
		rev[i]=rev[i>>1]>>1|((i&1)<<bit-1);
		if(i<rev[i]) swap(a[i],a[rev[i]]);
	}
	for(int mid=1; mid<n; mid<<=1){
		ll w=1,w1=Fstpw(tp==1?G:invG,(mod-1)/mid/2);
		for(int j=0; j<mid; j++,w=w*w1%mod)
			for(int i=0; i<n; i+=mid*2){
				int x=a[i+j],y=w*a[i+j+mid]%mod;
				a[i+j]=(x+y)%mod;
				a[i+j+mid]=(x-y+mod)%mod;
			}
	}
	if(tp==-1){
		ll t=Fstpw(n,mod-2);
		for(int i=0; i<n; i++)
			a[i]=a[i]*t%mod;
	}
	return ;
}
void NTT(int *a,int n,int *b,int m,int *res,int k){
	int siz=1;
	while(siz<=n+m) siz<<=1;
	static int f[MAXN<<2],g[MAXN<<2];
	for(int i=0; i<siz; i++){
		f[i]=i<=n?a[i]:0;
		g[i]=i<=m?b[i]:0;
	}
	ntt(f,siz,1);
	ntt(g,siz,1);
	for(int i=0; i<siz; i++)
		f[i]=1ll*f[i]*g[i]%mod;
	ntt(f,siz,-1);
	for(int i=0; i<=k; i++)
		res[i]=f[i];
	return ;
}
void Ntt(int *a,int n,int *b,int m,int *res,int k){	// n>=k
	static int f[MAXN<<2];
	for(int i=0; i<=m; i++)
		f[i]=b[m-i];
	NTT(a,n,f,m,f,k+m);
	for(int i=0; i<=k; i++)
		res[i]=f[i+m];
	return ;
}
void GetInv(int *a,int n,int *res){
	static int f[MAXN<<2],g[MAXN<<2];
	g[0]=Fstpw(a[0],mod-2);
	for(int siz=2; siz<=n<<1; siz<<=1){
		int mid=siz>>1,m=siz<<1;
		for(int i=0; i<m; i++){
			f[i]=i<siz&&i<=n?a[i]:0;
			if(i>=mid) g[i]=0;
		}
		ntt(f,m,1);
		ntt(g,m,1);
		for(int i=0; i<m; i++)
			g[i]=1ll*g[i]*(2-1ll*f[i]*g[i]%mod+mod)%mod;
		ntt(g,m,-1);
	}
	for(int i=0; i<=n; i++)
		res[i]=g[i];
	return ;
}
void GetLn(int *a,int n,int *res){	//a[0]=1
	static int f[MAXN<<2],g[MAXN<<2];
	for(int i=0; i<n; i++)
		g[i]=a[i+1]*(i+1ll)%mod;
	g[n]=0;
	GetInv(a,n,f);
	NTT(f,n,g,n,f,n);
	for(int i=1; i<=n; i++)
		res[i]=Fstpw(i,mod-2)*f[i-1]%mod;
	res[0]=0;
	return ;
}
void GetExp(int *a,int n,int *res){	// a[0]=0
	static int f[MAXN<<2],ln[MAXN<<2],aa[MAXN<<2];
	// F = F0 * (1 - ln F0 + A)
	f[0]=1;
	for(int siz=2; siz<=n<<1; siz<<=1){
		int mid=siz>>1,m=siz<<1;
		for(int i=0; i<m; i++){
			if(i>=mid) f[i]=0;
			aa[i]=i<siz&&i<=n?a[i]:0;
			ln[i]=i<mid?f[i]:0;
		}
		GetLn(ln,siz-1,ln);
		ntt(f,m,1);
		ntt(ln,m,1);
		ntt(aa,m,1);
		for(int i=0; i<m; i++)
			f[i]=(1ll-ln[i]+aa[i]+mod)%mod*f[i]%mod;
		ntt(f,m,-1);
		//printf("siz %d\n",siz);
	}
	for(int i=0; i<=n; i++)
		res[i]=f[i];
	return ;
}
void GetPow(int *a,int n,int k,int *res){	// k int
	int pos=0;
	while(pos<=n&&!a[pos]) pos++;
	if(pos>n){
		for(int i=0; i<=n; i++)
			res[i]=0;
		return ;
	}
	static int f[MAXN];
	for(int i=pos; i<=n; i++)
		f[i-pos]=a[i];
	ll a0=f[0],t=Fstpw(a0,mod-2);
	for(int i=0; i<=n-pos; i++)
		f[i]=a[i+pos]*t%mod;
	GetLn(f,n-pos,f);
	for(int i=0; i<=n-pos; i++)
		f[i]=1ll*f[i]*k%mod;
	GetExp(f,n-pos,f);
	t=Fstpw(a0,k);
	for(int i=0; i<=n; i++)
		res[i]=i<1ll*pos*k?0:f[i-pos*k]*t%mod;
	return ;
}
void GetSqrt(int *a,int n,int *res){	// a[0] Quadratic residue
	static int f[MAXN<<2],g[MAXN<<2];
	if(a[0]==1){
		f[0]=1;
	}else{
		const int blks=1e5;
		static int pw1[blks+5],pw2[blks+5];
		map<int,int> mp;
		pw1[0]=pw2[0]=1;
		mp[1]=0;
		for(int i=1; i<=blks; i++)
			pw1[i]=1ll*pw1[i-1]*G%mod,mp[pw1[i]]=i;
		for(int i=1; i<=blks; i++)
			pw2[i]=1ll*pw2[i-1]*pw1[blks]%mod;
		for(int i=0; i<=blks; i++){
			int t=a[0]*Fstpw(pw2[i],mod-2)%mod;
			if(mp.count(t)){
				f[0]=(1ll*i*blks+mp[t])%(mod-1)/2;
				f[0]=Fstpw(G,f[0]);
				break;
			}
		}
	}
	for(int siz=2; siz<=n<<1; siz<<=1){
		int mid=siz>>1;
		for(int i=mid; i<siz; i++)
			f[i]=0;
		GetInv(f,siz-1,g);
		NTT(f,siz-1,f,siz-1,f,siz-1);
		for(int i=0; i<siz; i++)
			f[i]=(f[i]+a[i])%mod;
		NTT(f,siz-1,g,siz-1,f,siz-1);
		for(int i=0; i<siz; i++)
			f[i]=f[i]*(mod+1ll)/2%mod;
	}
	for(int i=0; i<=n; i++)
		res[i]=f[i];
	return ;
}
namespace MultiPoint{
	int n,m,a[MAXN],x[MAXN],y[MAXN];
	inline void Ntt(int *a,int n,int *b,int m,int *c,int k){
		static int f[MAXN<<1];
		for(int i=0; i<=n; i++)
			f[i]=a[n-i];
		NTT(f,n,b,m,f,n+m);
		for(int i=0; i<=k; i++)
			c[i]=f[i+n];
		return ;
	}
	int *p[MAXN<<2],len[MAXN<<2],*v[MAXN<<2];
#define lc k<<1
#define rc k<<1|1
#define ls lc,l,mid
#define rs rc,mid+1,r
	void Dfs1(int k,int l,int r){
		if(l==r){
			p[k]=new int[2];
			p[k][0]=1;
			p[k][1]=(mod-x[l])%mod;
			len[k]=1;
			return ;
		}
		int mid=l+r>>1;
		Dfs1(ls);
		Dfs1(rs);
		len[k]=len[lc]+len[rc];
		p[k]=new int[max(len[k]+1,k==1?n+1:0)];
		NTT(p[lc],len[lc],p[rc],len[rc],p[k],len[k]);
		return ;
	}
	void Dfs2(int k,int l,int r){
		if(l==r){
			y[l]=*v[k];
			return ;
		}
		v[lc]=new int[len[lc]];
		Ntt(p[rc],len[rc],v[k],len[k]-1,v[lc],len[lc]-1);
		v[rc]=new int[len[rc]];
		Ntt(p[lc],len[lc],v[k],len[k]-1,v[rc],len[rc]-1);
		int mid=l+r>>1;
		Dfs2(ls);
		Dfs2(rs);
		return ;
	}
	void Calc(){
		scanf("%d%d",&n,&m);
		for(int i=0; i<=n; i++)
			scanf("%d",a+i);
		for(int i=1; i<=m; i++)
			scanf("%d",x+i);
		Dfs1(1,1,m);
		v[1]=new int[max(n,m)+1];
		GetInv(p[1],n,v[1]);
		Ntt(v[1],n,a,n,v[1],m-1);
		Dfs2(1,1,m);
		for(int i=1; i<=m; i++)
			printf("%d\n",y[i]);
		return ;
	}
#undef lc
#undef rc
#undef ls
#undef rs
}
int main(){
	fac[0]=1;
	for(int i=1; i<=n; i++)
		fac[i]=fac[i-1]*i%mod;
	inv[n]=Fstpw(fac[n],mod-2);
	for(int i=n; i; i--)
		inv[i-1]=inv[i]*i%mod;
	return 0;
}
/*
p   r  k  g
3   1   1   2
5   1   2   2
17  1   4   3
97  3   5   5
193 3   6   5
257 1   8   3
7681    15  9   17
12289   3   12  11
40961   5   13  3
65537   1   16  3
786433  3   18  10
5767169 11  19  3
7340033 7   20  3
23068673    11  21  3
104857601   25  22  3
167772161   5   25  3
469762049   7   26  3
1004535809  479 21  3
2013265921  15  27  31
2281701377  17  27  3
3221225473  3   30  5
75161927681 35  31  3
77309411329 9   33  7
206158430209    3   36  22
2061584302081   15  37  7
2748779069441   5   39  3
6597069766657   3   41  5
39582418599937  9   42  5
79164837199873  9   43  5
263882790666241 15  44  7
1231453023109121    35  45  3
1337006139375617    19  46  3
3799912185593857    27  47  5
4222124650659841    15  48  19
7881299347898369    7   50  6
31525197391593473   7   52  3
180143985094819841  5   55  6
1945555039024054273 27  56  5
4179340454199820289 29  57  3
*/
