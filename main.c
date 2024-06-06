//战斗，爽！

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
//#include "filetest1.h"

#define N 550 //点
#define M 100010 //边
#define period 3 //时间间隔
#define Len 150 //账号，密码，用户名长度限制
#define maxn 100 //其他以外的数值上限
#define A 20010//日记长度
#define T 200//标题长度,日记总数
// #define Place 46
// #define Attribution 18
const int P = 31;
const int Q = 1331;//hash参数
const double p = 0.5;
char vehicle[4][10] = {"walk","bike","motorbike","all"};
                        //0:步行  1:自行车   2:摩托车
#define WEBSITE_PROCESS 1
#define USER_PROCESS 2
const double alpha=0.3;
//常量设计

//事件名称（废弃）

int map[N][N],edge_tot,point_tot,hash_tot,last_rand;
//全局变量

typedef struct{
    int tim,//时间（通过加权拥塞值计算）
    length,//长度
    exist[3];//三种交通方式，认为所有路都可以步行
}Edge;//边
//type:0,1,2,3,4:4是时间
typedef struct{
    int id;//后台处理的编号
    char number[Len];//显示给用户的编号
    char account[Len];
    char password[Len];
}User;//用户
typedef struct{
    int id;
    int num1;
    double num2;
    int destination[T];
    char t[T];//标题
    char a[A];//文章
}Diary;
typedef struct{
    double v[3];//景点三个属性：热度、评论、个人兴趣;
    int id,x,y,type;//id,xy坐标,0/1是学校/景区 
    int is_head;
    double d;
    char name[Len],bel[Len];//二级地点名、一级地点名
    int lei;//菜系
    char dish[Len];//菜品名称

    //川菜，鲁菜，苏菜，湘菜，粤菜，浙菜
}Point;
typedef struct{
    int nxt,to;
    Edge w;
}Node;
//结构体建立

int get_val(int n){//n: 生成数据范围
    int next_rand=(rand()%n+n)%n;
    last_rand=(last_rand+next_rand)%n;
    // printf("%d\n",last_rand);//基本随机
    return last_rand;
}//返回一个[0,n)的数据

int file_exist;
//文件是否存在
int path_not_exist;

FILE* read_file_by_number(char *dictionary, char *base_name, int number) {
    char file_name[Len+Len];
    FILE *file;
    // 构造文件名
    snprintf(file_name, Len, "%s/%s%d.txt", dictionary, base_name, number);
    // 尝试打开文件
    file = fopen(file_name, "r");
    file_exist = 1;
    if (file == NULL) {
        file_exist = 0;
        printf("文件 %s 不存在\n", file_name);
        return NULL;
    }
    return file;
}
FILE* write_file_by_number(char *dictionary, char *base_name, int number) {
    char file_name[Len+Len];
    FILE *file;
    // 构造文件名
    snprintf(file_name, Len, "%s/%s%d.txt", dictionary, base_name, number);
    // 尝试打开文件
    file = fopen(file_name, "w");
    return file;
}
void omit_line(char *a){
    if(a[strlen(a)-1] == '\n')
        a[strlen(a)-1] = '\0';
}
//省略换行
void user_build(char *dictionary, char *base_name, User *person){
    FILE *f;
    char a[Len];
    for(int i=0; i<10; ++i) {
        f = read_file_by_number(dictionary,base_name,i);
        fgets(a, sizeof(a), f);
        omit_line(a);//忽略行末换行
        memcpy(person[i].account,a,sizeof(a));
        fgets(a, sizeof(a), f);
        omit_line(a);//忽略行末换行
        memcpy(person[i].password,a,sizeof(a));
        fgets(a, sizeof(a), f);
        omit_line(a);//忽略行末换行
        memcpy(person[i].number,a,sizeof(a));
        person[i].id=i;
        fclose(f);
    }
}//必要初始化函数,适时clear函数内容
void swap(int *a,int *b){
    int tmp=*a;
    *a=*b;
    *b=tmp;
}
//用户登录相关

int add(int *fir,int cnt,Node *e,int u,int v,Edge w){
    ++cnt;
    e[cnt].nxt=fir[u];
    fir[u]=cnt;
    e[cnt].to=v;
    e[cnt].w=w;
    return cnt;
}//链式前向星建图
char Lei[][Len]={"川菜","鲁菜","苏菜","湘菜","粤菜","浙菜"};
void read_rand(int *fir,Node *e,Point*scene,int n,int m, char places[N][Len], char attribution[N][Len]){

    FILE *tmp_f=fopen("name.txt","r");
    char dish[24][Len];
    for(int i=0;i<24;++i){
        fscanf(tmp_f,"%s",dish[i]);
    }
    fclose(tmp_f);

    memset(fir,0,sizeof(fir));
    FILE* f=fopen("datain.txt","r");
    fscanf(f,"%d",&point_tot);
    int cnt=0,tmp_len=0,st=0;
    char str1[] = "本部"; 
    char str2[] = "总景区";
    char str3[] = "分校";
    char str4[] = "分区";
    char str[Len];
    char tmp_str[Len];
    for(int i=0;i<500;++i){

        scene[i].lei=i%6;
        int tmp_y=scene[i].lei*4+i%4;//0~23
        memcpy(scene[i].dish,dish[tmp_y],sizeof(dish[tmp_y]));

        for(int j=0;j<2;++j)
            scene[i].v[j]=(double)(i*i%95+5)*10.0/maxn;
        scene[i].v[2]=(scene[i].v[0]+scene[i].v[1])/2.0;
            //热度、评论
        scene[i].id=i;
        fscanf(f,"%d",&scene[i].x);
        fscanf(f,"%d",&scene[i].y);
        //坐标
        if(i<250)scene[i].type=0;//学校
        else scene[i].type=1;//景区
        if(i<250){
            if(i<68){
                memcpy(scene[i].bel,attribution[9],sizeof(attribution[10]));
                scene[i].is_head=1;
            }
            else{
                memcpy(scene[i].bel,attribution[get_val(100)],sizeof(scene[i].bel));
                memcpy(str,scene[i].bel,sizeof(scene[i].bel));
                if(i<100){
                    scene[i].is_head=1;
                    strcat(str,str1);
                }
                else strcat(str,str3),scene[i].is_head=0;
                memcpy(scene[i].bel,str,sizeof(str));
            }
            fscanf(f,"%s",tmp_str);
            st=0;
            tmp_len=strlen(tmp_str);
            for(int j=0;j<tmp_len;++j)
                if(tmp_str[j]==':'){
                    st=j;
                    break;
                }
            for(int j=st+1;j<tmp_len;++j){
                scene[i].name[j-st-1]=tmp_str[j];
            }
            //学校
        }
        else if(i<400){
            memcpy(scene[i].bel,attribution[get_val(100)+100],sizeof(scene[i].bel));
            memcpy(str,scene[i].bel,sizeof(scene[i].bel));
            if(i<350){
                strcat(str,str2);
                scene[i].is_head=1;
            }
            else strcat(str,str4),scene[i].is_head=0;
            memcpy(scene[i].bel,str,sizeof(str));
            fscanf(f,"%s",tmp_str);
            tmp_len=strlen(tmp_str);
            for(int j=0;j<tmp_len;++j)
                if(tmp_str[j]==':'){
                    st=j;
                    break;
                }
            for(int j=st+1;j<tmp_len;++j){
                scene[i].name[j-st-1]=tmp_str[j];
            }
            //景区
        }
        else{
            fscanf(f,"%s",tmp_str);
            scene[i].is_head=0;
            if(i<450){
                memcpy(scene[i].bel,attribution[get_val(10)],sizeof(scene[i].bel));
                memcpy(str,scene[i].bel,sizeof(scene[i].bel));
                strcat(str,str3);//分校
                memcpy(scene[i].bel,str,sizeof(str));
            }
            else{
                memcpy(scene[i].bel,attribution[get_val(10)+100],sizeof(scene[i].bel));
                memcpy(str,scene[i].bel,sizeof(scene[i].bel));
                strcat(str,str4);//分区
                memcpy(scene[i].bel,str,sizeof(str));
            }
            memcpy(scene[i].name,places[50],sizeof(places[50]));
        }//400~500
    }

    Edge w;
    fscanf(f,"%d",&edge_tot);
    for(int i=0;i<m;++i){
        int u,v,m_id;
        fscanf(f,"%d",&u);
        fscanf(f,"%d",&v);
        fscanf(f,"%d",&w.length);
        if(w.length<10)w.length=15+i%85;
        fscanf(f,"%d",&m_id);
        // if(i<166)
        //     printf("%d\n",w.length);
        w.exist[m_id]=1;
        if(m_id==1){
            w.exist[0]=1;
        }
        if(m_id==2){
            w.exist[0]=1;
        }
        cnt = add(fir,cnt,e,u,v,w);
        cnt = add(fir,cnt,e,v,u,w);
        // printf("%d %d %d %d\n",u,v,v,u);
    }

    fclose(f);
}//读取地图
//建图
void hash_insert(char *a,int *head,int *nxt,int *to_id,int id){
    int tmp_val=0;
    for(int i=0; a[i]!='\n'&&a[i]!='\0';++i){
        tmp_val=(1LL*tmp_val*P+a[i])%Q;
    }
    nxt[++hash_tot]=head[tmp_val];
    head[tmp_val]=hash_tot;
    to_id[hash_tot]=id;
}//废用函数，但不敢删了
int hash_build(int *head,int *nxt,int *to_id,char *dictionary,char * base_name){
    FILE *f;
    char a[Len];
    for(int i=0; i<10; ++i) {
        f = read_file_by_number(dictionary,base_name,i);
        for(int k=0; k<3; ++k)
            fgets(a, sizeof(a), f);
        omit_line(a);//忽略行末换行
        hash_insert(a, head, nxt, to_id, i);
        fclose(f);
    }
    return hash_tot;
}//废用函数，但不敢删了
int hash_find(char *a,int *head,int *nxt,int *to_id){
    int tmp_val=0;
    for(int i=0; a[i]!='\n'&&a[i]!='\0';++i){
        tmp_val=(1LL*tmp_val*P+a[i])%Q;
    }
    int now=head[tmp_val];
    for(int i=head[tmp_val]; i; i=nxt[i]){
        if(to_id[i]==-1){
            printf("不存在这个编号的用户\n");
        }
        else return to_id[i];
    }
}//废用函数，但不敢删了
void sort_for_all(int *tmp_id,Point *scene,int lim,int type){
//    int tmp_pool[N];
    for(int i=0;i<lim;++i)
        tmp_id[i]=i;
    for(int i=0;i<lim;++i){
        for(int j=lim-1;j>i;--j)
        if(type!=3){
            if(scene[tmp_id[j]].v[type]>scene[tmp_id[j-1]].v[type]){
                swap(&tmp_id[j],&tmp_id[j-1]);
            }
        }
        else
            if(scene[tmp_id[j]].d<scene[tmp_id[j-1]].d)
                swap(&tmp_id[j],&tmp_id[j-1]);
    }
}
int w_type_check(Edge w,int type){
    // return w.length;
    if(type==3){
        int tim;
        if(w.exist[1]==1)tim=w.length*3;
        else if(w.exist[2]==1)tim=w.length*2;
        else tim=w.length*6;
        return tim*2/3;
    }
    if(type==0){
        for(int j=2;j>=0;--j) {
            if(w.exist[j])return w.length*6;
        }
    }
    if(type==1){
        if(w.exist[type])return w.length*3;
    }
    if(type==2){
        if(w.exist[type])return w.length*2;
    }
    return 0x3f3f3f3f;
}
int dijkstra(int lim,Point *scene,int *fir,Node *e,int from,int to,int type,int *pre,int *pre_edge){
    int vis[N],dis[N],inf=0x3f3f3f3f;
    memset(vis,0,sizeof(vis));
    memset(dis,0x3f,sizeof(dis));
    memset(pre,0,sizeof(pre));
    memset(pre_edge,0x3f,sizeof(pre_edge));
    dis[from]=0;
    for(int i=0;i<point_tot;++i){
        int min_len=inf,now=-1,nxt;
        for(int j=0;j<point_tot;++j)
            if(vis[j]==0&&dis[j]<min_len){
                min_len=dis[j];
                now=j;
            }
        if(now==-1)break;
//        printf("%d\n",now);
        vis[now]=1;
        for(int j=fir[now];j;j=e[j].nxt){
            nxt=e[j].to;
            if(vis[nxt]==0&&dis[nxt]>dis[now]+w_type_check(e[j].w,type)){
//                printf("%d %d %d %d %d\n",from,to,e[j].w.length,type,w_type_check(e[j].w,type));
                dis[nxt]=dis[now]+w_type_check(e[j].w,type);
                pre[nxt]=now;
                pre_edge[nxt]=w_type_check(e[j].w,type);
                // printf("%d->%d:%d\n",now,nxt,pre_edge[nxt]);
            }
        }
    }
    return dis[to];
}

void check_type(int type){
    printf("%d\n",type);
}

int catch_event(char *str){
    int len=strlen(str);
    if(len>=3){
        if(str[0]=='1')return 10;//登录
        else if(str[0]=='2'){
            if(str[2]=='1')return 21;
            if(str[2]=='2')return 22;
            if(str[2]=='3')return 23;
            if(str[2]=='4')return 24;
        }//游学推荐
        else if(str[0]=='3'){
            if(str[2]=='1')return 31;
            if(str[2]=='2')return 32;
            if(str[2]=='3')return 33;
            if(str[2]=='4')return 34;
        }//场所查询
        else if(str[0]=='4'){
            if(str[2]=='1')return 41;
            if(str[2]=='2')return 42;
            if(str[2]=='3')return 43;
            if(str[2]=='4')return 44;
        }//游学日记管理
        else if(str[0]=='5'){
            if(str[2]=='1')return 51;
            if(str[2]=='2')return 52;
            if(str[2]=='3')return 53;
            if(str[2]=='4')return 54;
        }//游学路线规划
        else if(str[0]=='6'){
            if(str[2]=='1')return 61;
            if(str[2]=='2')return 62;
            if(str[2]=='3')return 63;
            if(str[2]=='4')return 64;
        }//选做
    }
    return 0;
}//前端事件捕捉
int new_read_user(){
    FILE *f = fopen("now_use.txt","r");
    int x;
    fscanf(f,"%d",&x);
    fclose(f);
    return x;
}
void new_user_login(char *account,char *password,User *person){
    printf("%s %s\n",account,password);
    for(int i=0;i<10;++i){
        printf("%s %s %s %s\n",person[i].account,person[i].password,account,password);
        if(strcmp(person[i].account,account)==0&&
            strcmp(person[i].password,password)==0){
                FILE *f=fopen("now_user.txt","w");
                fprintf(f,"%d",i);
                fclose(f);
                printf("%d登录成功",i);
                return;
            }
    }
    printf("登录失败");
}
int new_recommendations_string(int Lim,Point* scene,Point*tmp_scene,char *str,int type){
    int lim=0,len=strlen(str);
    // printf("%s\n",str);
    char tmp_str[Len];
    memset(tmp_str,0,sizeof(tmp_str));
    for(int i=0;i<Lim;++i){
        if(strlen(scene[i].bel)>=len){
            memset(tmp_str,0,sizeof(tmp_str));
            for(int j=0;j<len;++j)
                tmp_str[j]=scene[i].bel[j];
            if(strcmp(str,tmp_str)==0){
                if(type==0){
                    tmp_scene[lim]=scene[i];
                    ++lim;
                }
                else if(type==1&&scene[i].is_head==1){
                    tmp_scene[lim]=scene[i];
                    ++lim;
                }
                else if(type==2&&scene[i].is_head==0){
                    tmp_scene[lim]=scene[i];
                    ++lim;
                }
            }
        }
        if(strlen(scene[i].name)>=len){
            memset(tmp_str,0,sizeof(tmp_str));
            for(int j=0;j<len;++j)
                tmp_str[j]=scene[i].name[j];
            // printf("%s %s %s\n",str,scene[i].bel,scene[i].name);
            if(strcmp(str,tmp_str)==0){
                if(type==0){
                    tmp_scene[lim]=scene[i];
                    ++lim;
                }
                else if(type==1&&scene[i].is_head==1){
                    tmp_scene[lim]=scene[i];
                    ++lim;
                }
                else if(type==2&&scene[i].is_head==0){
                    tmp_scene[lim]=scene[i];
                    ++lim;
                }
            }
        }
        if(strlen(scene[i].dish)>=len){
            for(int j=0;j<len;++j)
                tmp_str[j]=scene[i].dish[j];
            if(strcmp(str,tmp_str)==0){
                if(type==0){
                    tmp_scene[lim]=scene[i];
                    ++lim;
                }
            }
        }
    }
    if(lim==0){
        char tmp_bel[] = "没有找到相应地点";
        memcpy(tmp_scene[0].bel,tmp_bel,sizeof(tmp_bel));
        ++lim;
    }
    return lim;
}
double new_get_dis(Point aa,Point bb){
    return sqrt((double)(aa.x-bb.x)*(aa.x-bb.x)+(double)(aa.y-bb.y)*(aa.y-bb.y));
}
Diary da[T];
void sort_for_all2(int *tmp_id,Diary *scene,int lim,int type){
//    int tmp_pool[N];
    for(int i=0;i<lim;++i)
        tmp_id[i]=i;
    for(int i=0;i<lim;++i){
        for(int j=lim-1;j>i;--j){
            if(type==0){
                if(scene[tmp_id[j]].num1>scene[tmp_id[j-1]].num1){
                    swap(&tmp_id[j],&tmp_id[j-1]);
                }
            }
            else if(type==1){
                if(scene[tmp_id[j]].num2>scene[tmp_id[j-1]].num2)
                    swap(&tmp_id[j],&tmp_id[j-1]);
            }
            else{
                double x=(double)scene[tmp_id[j]].num1;
                double y=(double)scene[tmp_id[j-1]].num1;
                x*=alpha;
                y*=alpha;
                x+=scene[tmp_id[j]].num2*(1-alpha);
                y+=scene[tmp_id[j-1]].num2*(1-alpha);
                if(x>y){
                    swap(&tmp_id[j],&tmp_id[j-1]);
                }
            }
        }
    }
}
void new_sort_diary(char *dictionary,char*base_name,int type){
    FILE* f=fopen("number.txt","r");
    int diary_tot;
    fscanf(f,"%d",&diary_tot);
    fclose(f);
    int tmp_id[T],tmp_len;
    for(int i=0;i<diary_tot;++i){
        f=read_file_by_number(dictionary,base_name,i);
        fscanf(f,"%d",&da[i].id);
        fscanf(f,"%d%lf",&da[i].num1,&da[i].num2);
        fscanf(f,"%s",da[i].destination);
        fscanf(f,"%s",da[i].t);//标题
        fscanf(f,"%s",da[i].a);//文章
        fclose(f);
    }
    
    sort_for_all2(tmp_id,da,diary_tot,type);//type=0 热度 type=1 评论 //type==2 个人兴趣
    for(int i=0;i<diary_tot;++i){
        printf("id为%d的作者，有一篇日记名称为%s",da[tmp_id[i]].id,da[tmp_id[i]].t);
        if(type==0)printf("热度为%d",da[tmp_id[i]].num1);
        else if(type==1)printf("评分为%.1lf",da[tmp_id[i]].num2);
        else printf("个人兴趣为%.1lf",((double)da[tmp_id[i]].num1)*alpha+da[tmp_id[i]].num2*(1-alpha));
        printf(":");
        tmp_len=strlen(da[tmp_id[i]].a);
        for(int j=0;j<tmp_len;++j){
            printf("%c",da[tmp_id[i]].a[j]);
        }
        if(i!=diary_tot-1)printf("\n");
    }
}
void new_read_diary(char *dictionary,char*base_name,char *str,int type,int type2){
    //type=0 名称 type=1游学目的地
    FILE* f=fopen("number.txt","r");
    int diary_tot;
    fscanf(f,"%d",&diary_tot);
    fclose(f);
    for(int i=0;i<diary_tot;++i){
        f=read_file_by_number(dictionary,base_name,i);
        fscanf(f,"%d",&da[i].id);
        fscanf(f,"%d%lf",&da[i].num1,&da[i].num2);
        fscanf(f,"%s",da[i].destination);
        fscanf(f,"%s",da[i].t);//标题
        fscanf(f,"%s",da[i].a);//文章
        fclose(f);
    }
    int tmp_pos=0,tmp_len=strlen(str),lim=0;
    int tmp_id[T];
    char tmp_str[T];
    Diary tmp_da[T];
    for(int i=0;i<diary_tot;++i){
        if(type==0){
            if(strlen(da[i].t)>=tmp_len){
                for(int j=0;j<tmp_len;++j)
                    tmp_str[j]=da[i].t[j];
                if(strcmp(tmp_str,str)==0){
                tmp_da[lim]=da[i];
                ++lim;
                }
            }
        }
        else{
            // printf("%d %d\n",strlen(da[i].destination),tmp_len);
            if(strlen(da[i].destination)>=tmp_len){
                // memset(tmp_str,0,sizeof(tmp_str));
                // for(int j=0;j<tmp_len;++j)
                //     tmp_str[j]=da[i].destination[j];
                // printf("%s %s %s\n",da[i].destination,tmp_str,str);
                if(strcmp(da[i].destination,str)==0){
                    tmp_da[lim]=da[i];
                    ++lim;
                }
            }
        }
    }
    if(lim==0){
        char tmp_used_str[] = "没有符合要求的日记";
        memcpy(tmp_da[0].t,tmp_used_str,sizeof(tmp_used_str));
        lim++;
    }
    sort_for_all2(tmp_id,tmp_da,lim,type2);
    for(int i=0;i<lim;++i){
        printf("id为%d的作者，有一篇日记名称为%s",tmp_da[tmp_id[i]].id,tmp_da[tmp_id[i]].t);
        printf("目的地在%s,",tmp_da[tmp_id[i]].destination);
        if(type2==0)printf("其热度为%d",tmp_da[tmp_id[i]].num1);
        else printf("其评分为%.1lf",tmp_da[tmp_id[i]].num2);
        printf("\n");
    }
}
char tmp_long_str[A],tmp_short_str[A];
void new_check_diary(char *dictionary,char*base_name,char *article,double num2){
    FILE* f=fopen("number.txt","r");
    int diary_tot;
    fscanf(f,"%d",&diary_tot);
    fclose(f);
    for(int i=0;i<diary_tot;++i){
        f=read_file_by_number(dictionary,base_name,i);
        fscanf(f,"%d",&da[i].id);
        fscanf(f,"%d%lf",&da[i].num1,&da[i].num2);
        fscanf(f,"%s",da[i].destination);
        fscanf(f,"%s",da[i].t);//标题
        fscanf(f,"%s",da[i].a);//文章
        fclose(f);
    }
    memset(tmp_long_str,0,sizeof(tmp_long_str));
    memset(tmp_short_str,0,sizeof(tmp_short_str));
    int tmp_len=strlen(article),st=-1;
    for(int i=0;i<tmp_len;++i)
        if(article[i]==':')st=i;
    for(int i=st+1;i<tmp_len;++i){
        tmp_short_str[i-st-1]=article[i];
    }//正文部分
    tmp_len=strlen(tmp_short_str);
    // printf("%s\n",article);
    // printf("%s\n%.1lf\n",tmp_short_str,num2);
    for(int i=0;i<diary_tot;++i){
        if(strlen(da[i].a)>=tmp_len){
            for(int j=0;j<tmp_len;++j)
                tmp_long_str[j]=da[i].a[j];
            if(strcmp(tmp_long_str,tmp_short_str)==0){
                if(num2<0){
                    tmp_len=strlen(da[i].a);
                    printf("id为%d的作者，有一篇日记为：",da[i].id);
                    for(int j=0;j<tmp_len;++j){
                        printf("%c",da[i].a[j]);
                    }
                }
                else{
                    printf("%s %s\n",da[i].t,da[i].a);
                    f=write_file_by_number(dictionary,base_name,i);
                    fprintf(f,"%d\n",da[i].id);
                    fprintf(f,"%d %lf\n",da[i].num1+1,(double)(da[i].num2*da[i].num1+num2)/(da[i].num1+1));
                    fprintf(f,"%s\n",da[i].destination);
                    fprintf(f,"%s\n",da[i].t);//标题
                    fprintf(f,"%s",da[i].a);//文章
                    fclose(f);
                }
                break;
            }
        }
    }
}//只检索一篇文章
void new_write_diary(char *dictionary,char*base_name,char *destination,char *title,char *article){

    // printf("%shh\n%shh\n%shh\n",destination,title,article);
    FILE* f=fopen("number.txt","r");
    int diary_tot,now_id=0;
    fscanf(f,"%d",&diary_tot);
    fclose(f);

    f=fopen("now_user.txt","r");
    fscanf(f,"%d",&now_id);
    fclose(f);

    f=write_file_by_number(dictionary,base_name,diary_tot);
    diary_tot++;
    printf("%d\n",diary_tot);
    fprintf(f,"%d\n",now_id);
    fprintf(f,"1 10.0\n");
    fprintf(f,"%s\n",destination);
    fprintf(f,"%s\n",title);
    fprintf(f,"%s\n",article);
    fclose(f);

    f=fopen("number.txt","w");
    fprintf(f,"%d",diary_tot);
    fclose(f);
}

void sort_for_10(int *tmp_id,Point *scene,int lim,int type){//0热度 1评论 2距离 3以此类推
//    int tmp_pool[N];
    for(int i=0;i<lim;++i)
        tmp_id[i]=i;
    for(int i=0;i<10;++i){
        for(int j=lim-1;j>i;--j){
            if(type!=2){
                if(scene[tmp_id[j]].v[type]>scene[tmp_id[j-1]].v[type]){
                    swap(&tmp_id[j],&tmp_id[j-1]);
                }
            }
            else{
                    if(scene[tmp_id[j]].d<scene[tmp_id[j-1]].d){
                    swap(&tmp_id[j],&tmp_id[j-1]);
                }
            }
        }
    }
}

int vis[15],stp[15],pre[1205],pre_edge[1205],ans,ans_a[1205],ans_b[1205],ans_cnt,ans_w[1205];
int tmp_a[1205],tmp_b[1205],tmp_cnt=0,tmp_w[1205],cur,tmp_c[1205],tmp_d[1205],tmp_v[1205];
void dfs(int lim,Point *tmp_scene,int *fir,Node *e,int *pass,int cnt,int l,int r,int type){
    if(cnt==r+1){
        // for(int i=l;i<=r;++i)
        //     printf("%d ",stp[i]);
        // printf("\n");
        //(r-l)!种走法
        cur=0;
        tmp_cnt=0;
        for(int i=l;i<r;++i){
            cur+=dijkstra(lim,tmp_scene,fir,e,stp[i],stp[i+1],type,pre,pre_edge);
            int now=stp[i+1],tmp_cnt_tot=0;
            while(now!=stp[i]){
                tmp_c[tmp_cnt_tot]=pre[now];
                tmp_v[tmp_cnt_tot]=pre_edge[now];
                tmp_d[tmp_cnt_tot]=now;
                now=pre[now];
                tmp_cnt_tot++;
            }
            for(int i=0;i<tmp_cnt_tot;++i){
                tmp_a[tmp_cnt]=tmp_c[tmp_cnt_tot-i-1];
                tmp_b[tmp_cnt]=tmp_d[tmp_cnt_tot-i-1];
                tmp_w[tmp_cnt]=tmp_v[tmp_cnt_tot-i-1];
                tmp_cnt++;
            }
        }
        if(ans>cur){
            ans=cur;
            ans_cnt=tmp_cnt;
            for(int i=0;i<tmp_cnt;++i){
                ans_a[i]=tmp_a[i];
                ans_b[i]=tmp_b[i];
                ans_w[i]=tmp_w[i];
            }
        }
        return;
    }
    for(int i=l+1;i<=r;++i)
        if(vis[i]==0){
            vis[i]=1;
            stp[cnt]=pass[i];
            dfs(lim,tmp_scene,fir,e,pass,cnt+1,l,r,type);
            vis[i]=0;
        }
}
void planning_solve(int lim,Point *scene,Point *tmp_scene,int *fir,Node *e,char *argv[],int l,int r,int type){
    //2归属
    //3起点
    //4途径点
    //5途径点
    //6途径点
    //7途径点
    //8途径点
    //9type
    // printf("%d %d\n",l,r);
    int pass[15];
    memset(pass,0,sizeof(pass));
    for(int i=0;i<lim;++i){
        for(int j=l;j<=r;++j){
            if(strcmp(argv[j],tmp_scene[i].name)==0){
                pass[j]=tmp_scene[i].id;
                // printf("%d %s\n",pass[j],argv[j]);
            }
        }
    }
    memset(vis,0,sizeof(vis));
    ans=0x3f3f3f3f;
    ans_cnt=0;
    stp[l]=pass[l];
    dfs(lim,tmp_scene,fir,e,pass,l+1,l,r,type);
    for(int i=0;i<ans_cnt;++i){
        printf("%s\n%s\n%d",scene[ans_a[i]].name,scene[ans_b[i]].name,ans_w[i]);
        if(type==3)printf("s\n");
        else printf("m\n");
    }
}


User person[Len];//用户
Node e[M];//边
int fir[N];

Point scene[N],tmp_scene[N],tmp2_scene[N];//点
char places[N][Len];
char attribution[N][Len];
int tmp_id[N];//排序临时数组

int head[1336],tot,nxt[1336],to_id[1336];
    //hash用
int diary_a[N],diary_b[N],diary_cnt1=0,diary_cnt2=0;


int main(int argc,char *argv[]) {
    if(argc<=1){
        printf("执行失败");
        return 0;
    }//避免编译器直接运行main.c程序运行错误
    srand((unsigned)time(NULL));
    //简单的随机数生成
    char dictionary_user[] = "INIT/user",dictionary_diary[] = "INIT/diary";//路径
    char dictionary_places[] = "INIT/places", dictionary_attribution[] = "INIT/attribution";
    char base_name_user[] = "user",base_name_diary[] = "diary";//文件名
    char base_name_places[] = "places", base_name_attribution[] = "attribution";
    
    int len1=0,len2=0,len3=0,now_id=-1;
    //用户数据访问用

    int n=0,m=0,cnt=0,L,R,num=0;//图论用
    
    
    
    memset(fir,0,sizeof(fir));
    
    FILE *f = read_file_by_number(dictionary_places,base_name_places,0);
    for(int i=0; i<=50; ++i){
        fgets(places[i], sizeof(places[i]), f);
        omit_line(places[i]);
    }//第50是卫生间
    fclose(f);
    f = read_file_by_number(dictionary_attribution,base_name_attribution,0);
    for(int i=0; i<200; ++i){
        fgets(attribution[i], sizeof(attribution[i]), f);
        omit_line(attribution[i]);
    }
    fclose(f);
//    for(int i=0; i<Place; ++i){
//        printf("%d %s\n",i,places[i]);
//    }


    memset(head,0,sizeof(head));
    memset(nxt,0,sizeof(nxt));
    memset(to_id,-1,sizeof(to_id));
    user_build(dictionary_user,base_name_user,person);//用户信息预处理
    // tot=hash_build(head,nxt,to_id,dictionary_user,base_name_user);//哈希建立（重新写）
    int opt =0;
    // printf("%s\n",argv[1]);
    opt = catch_event(argv[1]);

    // printf("hh\n");

    read_rand(fir,e,scene,500,40000,places,attribution);//读图

    // for(int i=0;i<point_tot;++i){
    //     printf("%d %d %d %s %s %s %s\n",i,scene[i].x,scene[i].y,scene[i].bel,scene[i].name,Lei[scene[i].lei],scene[i].dish);
    // }
    int type=0,type2=0,lim=0,lim2=0,tmp_len=0,tmp_pos=0;
    char str1[] = "所有";
    char tmp_str[Len];
    double dd=0.0,num2=0.0;
    char tmp_exist[] = "没有这个途径点";
    int l,r,tmp_lei=0,tmp_un;
    switch(opt){
        case 10:
            new_user_login(argv[2],argv[3],person);
            break;
        case 21:
            for(int i=0;i<point_tot-1;++i){
                printf("%s\n",scene[i].bel);
            }
            printf("%s",scene[point_tot-1].bel);
            break;
        case 22:
            if(argv[2][6]=='1'){
                type=0;
            }//热度
            else if(argv[2][6]=='2'){
                type=1;
            }//评论
            else{ 
                type=2;
            }//个人兴趣
            // check_type(type);
            sort_for_all(tmp_id,scene,point_tot,type);
            for(int i=0;i<point_tot-1;++i){
                printf("%s:%.1lf\n",scene[tmp_id[i]].bel,scene[tmp_id[i]].v[type]);
            }
            printf("%s:%.1lf",scene[tmp_id[point_tot-1]].bel,scene[tmp_id[point_tot-1]].type);
            break;
        case 23:
            if(argv[3][6]=='1'){
                type=0;
            }//名称前缀
            else if(argv[3][6]=='2'){
                type=1;
            }//本部/总景区
            else{
                type=2;
            }//分校/分区
            // check_type(type);
            lim=new_recommendations_string(point_tot,scene,tmp_scene,argv[2],type);
            sort_for_all(tmp_id,tmp_scene,lim,2);//默认是综合排序
            for(int i=0;i<lim-1;++i){
                printf("%s%s:%.1lf\n",tmp_scene[tmp_id[i]].bel,tmp_scene[tmp_id[i]].name,tmp_scene[tmp_id[i]].v[2]);
            }
            printf("%s%s:%.1lf\n",tmp_scene[tmp_id[lim-1]].bel,tmp_scene[tmp_id[lim-1]].name,tmp_scene[tmp_id[lim-1]].v[2]);
            break;
        case 31:
            // check_type(type);
            type=0;//查前缀就行
            lim=new_recommendations_string(point_tot,scene,tmp_scene,argv[2],type);
            // printf("%s\n",argv[2]);
            printf("内部设施总数量%d\n",lim);
            for(int i=0;i<lim-1;++i){
                printf("%s %s\n",tmp_scene[i].bel,tmp_scene[i].name);
            }
            printf("%s %s\n",tmp_scene[lim-1].bel,tmp_scene[lim-1].name);
            break;
        case 32:
            if(argv[5][6]=='1')
                dd=50.0;
            else if(argv[5][6]=='2')
                dd=100.0;
            else
                dd=150.0;
            type=0;
            lim=new_recommendations_string(point_tot,scene,tmp_scene,argv[2],type);
            // argv[2]//place1
            // argv[3]//place2
            // argv[4]//place3
            tmp_pos=-1;
            tmp_len=strlen(argv[3]);
            for(int i=0;i<lim;++i){
                if(strlen(tmp_scene[i].name)>=tmp_len){
                    for(int j=0;j<tmp_len;++j)
                        tmp_str[j]=tmp_scene[i].name[j];
                    if(strcmp(tmp_str,argv[3])==0){
                        tmp_pos=i;
                        break;
                    }
                }
            }
            if(tmp_pos==-1){
                printf("所在地点不存在\n");
            }
            else{
                for(int i=0;i<lim;++i){
                    tmp_scene[i].d=new_get_dis(tmp_scene[i],tmp_scene[tmp_pos]);
                }
                memset(tmp_str,0,sizeof(tmp_str));//清空
                tmp_len=strlen(str1);
                for(int i=0;i<tmp_len;++i)
                    tmp_str[i]=argv[4][i];
                // printf("%s\n",tmp_str);
                if(strcmp(tmp_str,str1)==0){
                    // printf("hh\n");
                    sort_for_all(tmp_id,tmp_scene,lim,3);//按距离从小到大排
                    for(int i=0;i<lim-1;++i)
                        if(tmp_scene[tmp_id[i]].d<dd)
                            printf("%s:%.1lfm\n",tmp_scene[tmp_id[i]].name,tmp_scene[tmp_id[i]].d);
                    if(tmp_scene[tmp_id[lim-1]].d<dd){
                        printf("%s:%.1lfm\n",tmp_scene[tmp_id[lim-1]].name,tmp_scene[tmp_id[lim-1]].d);
                    }
                }
                //所有服务设施
                else{
                    // for(int i=0;i<lim;++i)
                    //     printf("%s %s\n",tmp_scene[i].bel,tmp_scene[i].name);
                    lim=new_recommendations_string(lim,tmp_scene,tmp2_scene,argv[4],0);
                    sort_for_all(tmp_id,tmp2_scene,lim,3);
                    for(int i=0;i<lim-1;++i)
                        if(tmp2_scene[tmp_id[i]].d<dd)
                            printf("%s:%.1lfm\n",tmp2_scene[tmp_id[i]].name,tmp2_scene[tmp_id[i]].d);
                    if(tmp2_scene[tmp_id[lim-1]].d<dd){
                        printf("%s:%.1lfm\n",tmp2_scene[tmp_id[lim-1]].name,tmp2_scene[tmp_id[lim-1]].d);
                    }
                }
                //指定服务设施
            }
            break;
        case 41:
            if(argv[2][6]=='1'){
                type=0;
            }
            else if(argv[2][6]=='2'){
                type=1;
            }
            else type=2;//个人兴趣
            // check_type(type);
            new_sort_diary(dictionary_diary,base_name_diary,type);
            //日记总数，用户可以根据关键字排序
            break;
        case 42:
            if(argv[3][6]=='1')type=0;
            else type=1;
            if(argv[4][6]=='1')type2=0;
            else type2=1;
            new_read_diary(dictionary_diary,base_name_diary,argv[2],type,type2);
            //名称查，目的地查
            //2输入框
            //3名称还是目的地
            //4热度还是评论
            break;
        case 43:
            num2 = strtod(argv[3],NULL);
            new_check_diary(dictionary_diary,base_name_diary,argv[2],num2);
            //全文检索
            break;
        case 44:
            // printf("%s\n",argv[1]);
            new_write_diary(dictionary_diary,base_name_diary,argv[2],argv[3],argv[4]);
            //destination title article
            //写日记
            break;
        case 51:
            // for(int i=2;i<=9;++i)
            //     printf("%s",argv[i]);
            //2归属
            //3起点
            //4途径点
            //5途径点
            //6途径点
            //7途径点
            //8途径点
            //9type
            //路径
            if(argv[9][6]=='1'){
                type=0;
            }//步行
            else if(argv[9][6]=='2'){
                type=1;
            }//自行车
            else if(argv[9][6]=='3'){
                type=2;
            }//摩托车
            else if(argv[9][6]=='4'){
                type=3;
            }//时间
            l=3,r=4;
            for(int i=5;i<=9;++i){
                if(strcmp(argv[i],tmp_exist)==0){
                    break;
                }
                else ++r;
            }//有效地名区间
            lim=new_recommendations_string(point_tot,scene,tmp_scene,argv[2],0);
            planning_solve(lim,scene,tmp_scene,fir,e,argv,l,r,type);
            break;
            
        case 61:
            //2地点字符串
            //3筛选字符串
            //4菜系/名称
            //5排序关键字
            if(argv[4][6]=='1')type=0;//菜系
            else if(argv[4][6]=='2')type=1;//名称

            if(argv[5][6]=='1')type2=0;
            else if(argv[5][6]=='2')type2=1;
            else if(argv[5][6]=='3')type2=2;//和指定地点距离
            lim=new_recommendations_string(point_tot,scene,tmp_scene,argv[2],0);
            // for(int i=0;i<point_tot;++i){
            //     printf("%s%s %d:%s\n",scene[i].bel,scene[i].name,scene[i].lei,scene[i].dish);
            // }
            tmp_un=0;
            for(int i=0;i<lim;++i)
                if(tmp_scene[i].is_head==1)
                    tmp_un=i;
            for(int i=0;i<lim;++i){
                tmp_scene[i].d=new_get_dis(tmp_scene[i],tmp_scene[tmp_un]);
                // printf("%s%s %d:%s\n",tmp_scene[i].bel,tmp_scene[i].name,tmp_scene[i].lei,tmp_scene[i].dish);
            }
            printf("%s\n",argv[3]);
            if(type==0){
                lim2=0;
                tmp_lei=0;
                for(int i=0;i<6;++i)
                    if(strcmp(argv[3],Lei[i])==0){
                        tmp_lei=i;
                        break;
                    }//找到菜系
                // printf("%s\n",Lei[tmp_lei]);
                for(int i=0;i<lim;++i)
                    if(tmp_scene[i].lei==tmp_lei){
                        tmp2_scene[lim2]=tmp_scene[i];
                        ++lim2;
                    }
            }
            else lim2=new_recommendations_string(lim,tmp_scene,tmp2_scene,argv[3],0);
            sort_for_10(tmp_id,tmp2_scene,lim2,type2);
            for(int i=0;i<lim2&&i<10;++i){
                printf("%s距离%.1lfm",tmp2_scene[tmp_id[i]].dish,tmp2_scene[tmp_id[i]].d*10.0);
                if(type2==0)printf(",其热度为%.1lf",tmp2_scene[tmp_id[i]].v[0]);
                else if(type2==1)printf(",其评分为%.1lf",tmp2_scene[tmp_id[i]].v[1]);
                printf("\n");
            }
            break;
    }
    // printf("程序执行完毕了哟");
    return 0;
}
