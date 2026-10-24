#include <bits/stdc++.h>
using namespace std;
#if !__has_include("../../competition/models/prefix_grid.hpp")
int main(){cerr<<"RED: model tools not implemented\n";return 1;}
#else
namespace grid_model {
#include "../../competition/models/prefix_grid.hpp"
}
namespace enum_model {
#include "../../competition/models/enumeration.hpp"
}
namespace selection {
#define main original_selection_main
#include "../../competition/models/choose_tasks.cpp"
#undef main
}
namespace ordered_choice {
#define main original_choice_main
#include "../../competition/models/ordered_choice.cpp"
#undef main
}
int main(){
    mt19937 rng(20261024);
    for(int tc=0;tc<300;tc++){
        int n=1+(int)(rng()%8),m=1+(int)(rng()%8);
        grid_model::init(n,m);vector<vector<long long>>v(n+1,vector<long long>(m+1));
        for(int i=1;i<=n;i++)for(int j=1;j<=m;j++)
            grid_model::a[i][j]=v[i][j]=(int)(rng()%21)-10;
        grid_model::build_prefix();
        for(int step=0;step<60;step++){
            int x=1+(int)(rng()%n),y=1+(int)(rng()%m);
            int u=1+(int)(rng()%n),w=1+(int)(rng()%m);
            if(x>u)swap(x,u);
            if(y>w)swap(y,w);
            long long sum=0;
            for(int i=x;i<=u;i++)for(int j=y;j<=w;j++)sum+=v[i][j];
            assert(grid_model::query(x,y,u,w)==sum);
        }
        for(int step=0;step<60;step++){
            int x=1+(int)(rng()%n),y=1+(int)(rng()%m);
            int u=1+(int)(rng()%n),w=1+(int)(rng()%m);
            if(x>u)swap(x,u);
            if(y>w)swap(y,w);
            long long z=(int)(rng()%21)-10;
            grid_model::add_rect(x,y,u,w,z);
            for(int i=x;i<=u;i++)for(int j=y;j<=w;j++)v[i][j]+=z;
        }
        grid_model::restore_difference();
        for(int i=1;i<=n;i++)for(int j=1;j<=m;j++)
            assert(grid_model::a[i][j]==v[i][j]);
    }
    for(int tc=0;tc<160;tc++){
        int n=(int)(rng()%17);vector<long long>a(n+1);long long inv=0;
        for(int i=1;i<=n;i++)enum_model::a[i]=a[i]=(int)(rng()%15)-7;
        for(int i=1;i<=n;i++)for(int j=i+1;j<=n;j++)inv+=a[i]>a[j];
        assert(enum_model::merge_count(1,n)==inv);
        assert(is_sorted(enum_model::a+1,enum_model::a+n+1));
        long long goal=(int)(rng()%31)-15, cnt=0;
        for(int mask=0;mask<(1<<n);mask++){
            long long sum=0;for(int i=1;i<=n;i++)if(mask>>(i-1)&1)sum+=a[i];
            cnt+=sum==goal;
        }
        assert(enum_model::subset_sum_count(a,goal)==cnt);
    }
    for(int tc=0;tc<300;tc++){
        int n=1+(int)(rng()%9);selection::n=n;selection::T=(int)(rng()%40);
        for(int i=1;i<=n;i++)selection::p[i]={1+(int)(rng()%n),i,1+(int)(rng()%10)};
        int expected=0;
        for(int mask=0;mask<(1<<n);mask++){
            int k=__builtin_popcount((unsigned)mask),score=0;long long time=0;
            for(int i=1;i<=n;i++)if(mask>>(i-1)&1){
                time+=selection::p[i].t;score+=selection::p[i].a>=k;
            }
            if(time<=selection::T)expected=max(expected,score);
        }
        sort(selection::p+1,selection::p+n+1,[](auto x,auto y){return x.t<y.t;});
        int got=0;for(int k=1;k<=n;k++)if(selection::check(k))got=k;
        assert(got==expected);
    }
    for(int tc=0;tc<250;tc++){
        int n=1+(int)(rng()%7),m=1+(int)(rng()%4);
        vector<long long>a(n),b(m);ostringstream in;
        in<<n<<' '<<m<<'\n';for(auto&x:a){x=1+rng()%10;in<<x<<' ';}
        in<<'\n';for(auto&x:b){x=1+rng()%10;in<<x<<' ';}in<<'\n';
        function<bool(int,long long)>go=[&](int i,long long pre){
            if(i==n)return true;
            if(a[i]>=pre&&go(i+1,a[i]))return true;
            for(auto x:b)if(x-a[i]>=pre&&go(i+1,x-a[i]))return true;
            return false;
        };
        bool expected=go(0,LLONG_MIN);istringstream input(in.str());ostringstream output;
        auto *cinbuf=cin.rdbuf(input.rdbuf()),*coutbuf=cout.rdbuf(output.rdbuf());
        ordered_choice::solve();cin.rdbuf(cinbuf);cout.rdbuf(coutbuf);
        assert(output.str()==(expected?"YES\n":"NO\n"));
    }
    cout<<"PASS model tools: 2D rectangles, inversion, MITM, two original solutions\n";
}
#endif
