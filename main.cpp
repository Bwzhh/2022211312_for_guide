#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=250100;
inline int read(){
    int x=0;
    char ch=getchar();
    while(ch<'0'|ch>'9')ch=getchar();
    while('0'<=ch&&ch<='9')x=(x<<3)+(x<<1)+(ch^48),ch=getchar();
    return x;
}
int n,m,k,a[N<<1],fir[N],cnt=1;
struct node{
    int nxt,to;
    ll w;
}e[N<<1];
int dep[N],dfn[N],top[N],fa[N],sz[N],son[N],tot;
ll minn[N],dp[N];//dp[i]记录i这个子树不与子树根父亲相连的最小代价，对于根1等价于断所有关键点孩子
bool vis[N];
void add(int u,int v,int w){
    e[++cnt].nxt=fir[u];
    fir[u]=cnt;
    e[cnt].to=v,e[cnt].w=w;
}
void dfs1(int u,int f){
    fa[u]=f;
    sz[u]=1;
    dep[u]=dep[f]+1;
    for(int v,i=fir[u];i;i=e[i].nxt){
       v=e[i].to;
       if(v==f)continue;
       minn[v]=min(minn[u],e[i].w);
       dfs1(v,u);
       sz[u]+=sz[v];
       if(sz[son[u]]<sz[v])son[u]=v;
    }
}
void dfs2(int x,int t){
    top[x]=t;
    dfn[x]=++tot;
    if(son[x])
        dfs2(son[x],t);
    for(int v,i=fir[x];i;i=e[i].nxt){
        v=e[i].to;
        if(v!=fa[x]&&v!=son[x])
            dfs2(v,v);
    }
}
int getlca(int x,int y){
    while(top[x]!=top[y]){
        if(dep[top[x]]<dep[top[y]])swap(x,y);
        x=fa[top[x]];
    }
    return dep[x]<dep[y]?x:y;
}
bool cmp(const int x,const int y){
    return dfn[x]<dfn[y];//dfs序排序
}
int s[N],Top;//单调栈建虚树
vector<int>vec[N];
void addedge(int u,int v){
    vec[u].push_back(v);
//    printf("%d %d\n",u,v);
}
void build(){
    s[Top=1]=1;
    for(int i=1;i<=k;++i){
        int lca=getlca(a[i],s[Top]);
        if(lca!=s[Top]){
            while(dfn[lca]<dfn[s[Top-1]]){
                addedge(s[Top-1],s[Top]);
                --Top;
            }
            if(dfn[lca]>dfn[s[Top-1]]){
                addedge(lca,s[Top]);
                s[Top]=lca;
            }
            else{
                addedge(s[Top-1],s[Top]);
                --Top;
            }
        }
        s[++Top]=a[i];
    }
    while(Top>1){
        addedge(s[Top-1],s[Top]);
        --Top;
    }
}
void solve(int u){
    ll ret=0;
    for(auto &v:vec[u]){
        solve(v);
        ret+=dp[v];
    }
    if(!vis[u])dp[u]=min(minn[u],ret);
    else dp[u]=minn[u];
    vec[u].clear();
}
int main(){
    n=read();
    memset(minn,0x3f,sizeof(minn));
    int u,v,w;
    for(int i=1;i<n;++i){
        u=read(),v=read(),w=read();
        add(u,v,w),add(v,u,w);
    }
    dfs1(1,0);
    dfs2(1,1);
    m=read();
    while(m--){
        k=read();
        for(int i=1;i<=k;++i){
            a[i]=read();
            vis[a[i]]=1;
        }
        sort(a+1,a+k+1,cmp);
        build();
        solve(1);
        printf("%lld\n",dp[1]);
        for(int i=1;i<=k;++i)
            vis[a[i]]=0;
    }
    return 0;
}