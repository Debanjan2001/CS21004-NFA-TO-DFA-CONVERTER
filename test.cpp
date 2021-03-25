#include<bits/stdc++.h>
using namespace std;

int main()
{
    srand(time(0));
    
    int lo=3,hi=24;
    int range=hi-lo;
    int n=lo+rand()%range;
    int m=lo+rand()%1;
    cout<<n<<endl<<m<<endl;

    vector<bool> isStartOrFinal(n+5,false);

    int cntStart=0;
    for(int i=1;i<=min(8,1+rand()%(n/2));i++)
    {
        int x=rand()%n;
        while(isStartOrFinal[x])
            x=rand()%n;
        isStartOrFinal[x]=true;
        cout<<x<<" ";
        cntStart++;
    }
    cout<<-1<<endl;

    for(int i=1;i<=min(min(6,n-cntStart),1+rand()%(n/2));i++)
    {
        int x=rand()%n;
        while(isStartOrFinal[x])
            x=rand()%n;
        isStartOrFinal[x]=true;
        cout<<x<<" ";
    }
    cout<<-1<<endl;


    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            cout<<i<<" "<<j<<" ";
            int x=rand()%n;
            while(x==i)
                x=rand()%n;
                
            cout<<x<<endl;
        }
    }

    cout<<-1<<endl;
    return 0;
}