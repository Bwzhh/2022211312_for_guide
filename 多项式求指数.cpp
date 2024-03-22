//怎么牛顿迭代都是nlogn
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define N 300100
const int mod=998244353,G=3,Gi=332748118;
ll power(ll base,ll b){
    ll ret=1;
    while(b){
        if(b&1)ret=ret*base%mod;
        base=base*base%mod;
        b>>=1;
    }
    return ret;
}
int pos[N],limit,len;
void getlen(int n){
    limit=1,len=0;
    while(limit<n+n)limit<<=1,++len;
    for(int i=0;i<limit;++i)
        pos[i]=(pos[i>>1]>>1)|((i&1)<<(len-1));
}
void ntt(ll *a,int n,int type){
    for(int i=0;i<limit;++i)
        if(i<pos[i])swap(a[i],a[pos[i]]);
    for(int mid=1;mid<limit;mid<<=1){
        ll wn=power(type==1?G:Gi,(mod-1)/(mid<<1));
        for(int j=0;j<limit;j+=(mid<<1)){
            ll w0=1,x,y;
            for(int k=0;k<mid;++k,w0=w0*wn%mod){
                x=a[j+k],y=a[j+mid+k]*w0%mod;
                a[j+k]=(x+y)%mod;
                a[j+mid+k]=(x-y+mod)%mod;
            }
        }
    }
    if(type==-1){
        ll inv=power(limit,mod-2);
        for(int i=0;i<n;++i)a[i]=a[i]*inv%mod;
        for(int i=n;i<limit;++i)a[i]=0;
    }
}
ll f_inv[N],f_qd[N],lng[N],h[N];
void getinv(ll *f,ll *g,int n){
    if(n==1){
        g[0]=power(f[0],mod-2);
        return;
    }
    getinv(f,g,(n+1)>>1);
    getlen(n);
    for(int i=0;i<n;++i)h[i]=f[i];
    for(int i=n;i<limit;++i)h[i]=0;
    ntt(g,n,1);
    ntt(h,n,1);
    for(int i=0;i<limit;++i)
        g[i]=g[i]*(2LL-h[i]*g[i]%mod+mod)%mod;
    ntt(g,n,-1);
}
void derivative(ll *a,ll *b,int n){
    b[n-1]=0;
    for(int i=0;i<n-1;++i)
        b[i]=a[i+1]*(ll)(i+1)%mod;
}
void integration(ll *a,ll *b,int n){
    b[0]=0;
    for(int i=1;i<n;++i)
        b[i]=a[i-1]*power(i,mod-2)%mod;
}
void getln(ll *f,ll *g,int n){
    getlen(n);
    for(int i=0;i<limit;++i)
        f_qd[i]=f_inv[i]=g[i]=0;
    derivative(f,f_qd,n);
    getinv(f,f_inv,n);
    getlen(n);
    ntt(f_qd,n,1);
    ntt(f_inv,n,1);
    for(int i=0;i<limit;++i)
        f_qd[i]=f_qd[i]*f_inv[i]%mod;
    ntt(f_qd,n,-1);
    integration(f_qd,g,n);
}
int n;
ll f[N],ans[N];
void getexp(ll *g,int n){
    if(n==1){
        g[0]=1;
        return;
    }
    getexp(g,(n+1)>>1);
    getln(g,lng,n);
    getlen(n);
    for(int i=0;i<n;++i)h[i]=f[i];
    for(int i=n;i<limit;++i)h[i]=0;
    ntt(g,n,1);
    ntt(lng,n,1);
    ntt(h,n,1);
    for(int i=0;i<limit;++i)
        g[i]=g[i]*(1-lng[i]+h[i]+mod)%mod;
    ntt(g,n,-1);
}
int main(){
    scanf("%d",&n);
    for(int i=0;i<n;++i){
        scanf("%lld",&f[i]);
    }
//    getln(f,ans,n);
    getexp(ans,n);
    for(int i=0;i<n;++i)
        printf("%lld ",ans[i]);
    return 0;
}