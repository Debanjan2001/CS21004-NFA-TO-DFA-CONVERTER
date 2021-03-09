/*********  Author *************
Name - Debanjan Saha
Roll number - 19CS30014
********************************/

#include<iostream>
#include<fstream>
using namespace std;

// User defined Data structure for storing NFA
class NFA
{
    public:    
        // n -> states, m-> alphabets
        int n,m;

        // StartState -> Start States ,FinalState -> Final States
        unsigned int StartState,FinalState;

        // Transition Table
        unsigned int** transition;

        NFA()
        {
            n = 0,m = 0;
            FinalState = 0, StartState = 0;
            transition = NULL;
        }

        // Free Space as soon as non relevant
        void freeNFA()
        {
            for(int i=0;i<n;i++)
            {
                if(transition[i])
                    delete transition[i];
            }

            if(transition)
                delete transition;
        }
};

class SetOfStates
{
    public:
        unsigned int* arr;
        int size;

        SetOfStates()
        {
            size = 0;
            arr = NULL;
        }

        // Free Space as soon as non relevant
        void freeSetOfStates()
        {
            if(arr)
            {
                delete arr;
            }
        }

};

class DFA
{
    public:
        // n -> states, m-> alphabets
        unsigned int n, m;

        // StartState -> Start State ,FinalState -> Final States  Date type
        unsigned int StartState;
        SetOfStates FinalState;

        // Transition Table
        unsigned int** transition;

        // number of final states. stored to save running time 
        int numFinalStates;

        DFA()
        {
            n = 0,m = 0;
            numFinalStates = 0;
            StartState = 0;
            FinalState = SetOfStates();
            transition = NULL;
        }

        // Free Space as soon as non relevant
        void freeDFA()
        {
            for(int i=0;i<n;i++)
            {
                if(transition[i])
                    delete transition[i];
            }
            if(transition)
            {
                delete transition;
            }
            FinalState.freeSetOfStates();
        }
};



// Function to read an NFA from a given file.
void readNFA(string fileName,NFA& N)
{   
    ifstream fin;
    fin.open(fileName.c_str());

    if(!fin)
    {
        cout<<"ERROR in locating file.Keep it in same directory"<<endl;
        exit(0);
    }
    int n,m;
    fin>>n>>m;

    N.transition = new unsigned int*[n];
    for(int i=0;i<n;i++)
    {
        N.transition[i] = new unsigned int[m];
        for(int j=0;j<m;j++)
        {
            N.transition[i][j] = 0;
        }
    }

    N.n = n;
    N.m = m;

    int x = 1; 
    // Start States reading
    while(true)
    {
        fin>>x;
        if(x==-1)
            break;
        N.StartState |= (1U<<x);
    }

    // Final States reading
    while(true)
    {
        fin>>x;
        if(x==-1)
            break;
        N.FinalState |= (1U<<x);
    }

    // Transition Function reading
    while(true)
    {
        fin>>x;
        if(x==-1)
            break;
        int y,z;
        fin>>y>>z;
        N.transition[x][y] |= (1U<<z);
    }

    fin.close();
} 


// Function to print NFA
void printNFA(NFA& N)
{
    cout<<"++++ INPUT NFA"<<endl;
    cout<<"     Number of States: "<<N.n<<endl;
    
    int printCount = 0;

    cout<<"     Number of Alphabet: {";
    for(int i=0;i<N.m;i++)
    {
        if(printCount)
            cout<<",";
        cout<<i;
        printCount++;
    }
    cout<<"}"<<endl;

    printCount = 0;
    cout<<"     Start States: {";
    for(int i=0;i<N.n;i++)
    {
        if( (N.StartState & (1<<i)) )
        {
            if(printCount)
                cout<<",";
            cout<<i;
            printCount++;
        }
    }
    cout<<"}"<<endl;

    printCount = 0;
    cout<<"     Final States: {";
    for(int i=0;i<N.n;i++)
    {
        if( (N.FinalState & (1<<i)) )
        {
            if(printCount)
                cout<<",";
            cout<<i;
            printCount++;
        }
    }
    cout<<"}"<<endl;

    cout<<"     Transition Function"<<endl;
    for(int i=0;i<N.n;i++)
    {
        for(int j=0;j<N.m;j++)
        {
            printCount = 0;
            cout<<"         Delta("<<i<<","<<j<<") = {";
            for(int k=0;k<N.n;k++)
            {
                if( (N.transition[i][j] & (1<<k)) )
                {
                    if(printCount)
                        cout<<",";
                    cout<<k;
                    printCount++;
                }
            }
            cout<<"}"<<endl;
        }
    }
}

void printDFA(DFA& D)
{
    cout<<"     Number of States: "<<D.n<<endl;
    
    int printCount = 0;

    cout<<"     Number of Alphabet: {";
    for(int i=0;i<D.m;i++)
    {
        if(printCount)
            cout<<",";
        cout<<i;
        printCount++;
    }
    cout<<"}"<<endl;

    cout<<"     Start State: "<<D.StartState<<endl;
    
    if(D.numFinalStates>=64)
        cout<<"     "<<D.numFinalStates<<" final states"<<endl;
    else
    {
        cout<<"     Final States: {";
        printCount = 0;
        for(int i=0;i<D.FinalState.size;i++)
        {
            for(int j=0;j<32;j++)
            {
                if(D.FinalState.arr[i] & (1U<<j) ) 
                {
                    if(printCount)
                        cout<<",";
                    cout<<32*i+j;
                    printCount++;
                }
            }
        }
        cout<<"}"<<endl;
    }

    //Transition Function
    cout<<"     Transition function: ";
    if(D.n>64)
    {
        cout<<"Skipped"<<endl;
        return;
    }
    cout<<endl;
    //TO be written.
    cout<<"     a\\p|";
    for(int i=0;i<D.n;i++)
    {
        printf("%2d ",i);
    }
    cout<<"\n";
    cout<<"     ---+";
    for(int i=0;i<D.n;i++)
    {
        printf("---");
    }

    cout<<"\n";
    for(int i=0;i<D.m;i++)
    {
        cout<<"      "<<i<<" |";
        for(int j=0;j<D.n;j++)
        {
            printf("%2d ",D.transition[j][i]);
        }
        cout<<endl;
    }

}

// NFA to DFA Converter
void subsetcons(NFA& N,DFA& D)
{
    //New sizes
    int n =(1<<N.n), m = N.m;
    D.n = n;
    D.m = m;

    // Start State 
    D.StartState = N.StartState;

    //Final States
    int size = (n/32)+1;
    D.FinalState.size = size;
    D.FinalState.arr = new unsigned int[size];
    for(int i=0;i<size;i++)
    {
        D.FinalState.arr[i] = 0;
    }

    unsigned int F = N.FinalState;

    for(int i=0;i<n;i++)
    {
        if( (F & i) )
        {
            int index = i/32;
            int pos = i%32;

            if( (D.FinalState.arr[index]&(1U<<pos)) == 0 )
            {
                D.numFinalStates++;
                D.FinalState.arr[index] |= (1U<<pos);
            } 
        }
    }

    //Transition Function
    D.transition = new unsigned int*[n];
    for(int i=0;i<n;i++)
    {
        D.transition[i] = new unsigned int[m];
        for(int j=0;j<m;j++)
        {
            D.transition[i][j] = 0;
            for(int k=0;k<N.n;k++)
            {
                if( (i & (1U<<k)) )
                {
                    D.transition[i][j] |= (N.transition[k][j]);
                }
            }
        }
    }
}

void dfs(DFA& D,int currentState,bool vis[])
{
    vis[currentState] = true;
    for(int i=0;i<D.m;i++)
    {
        if(!vis[D.transition[currentState][i]])
        {
            dfs(D,D.transition[currentState][i],vis);
        }
    }
}

void findreachable(DFA& D,SetOfStates& R)
{
    bool *vis = new bool[D.n];
    for(int i=0;i<D.n;i++)
        vis[i] = false;
    
    R.arr = new unsigned int[D.n/32+1];
    R.size = (D.n)/32 + 1 ;

    for(int i=0;i<R.size;i++)
    {
        R.arr[i] = 0;
    }

    dfs(D,D.StartState,vis);
    
    int printCount = 0;
    cout<<"++++ Reachable States: {";
    for(int i=0;i<D.n;i++)
    {
        if(vis[i])
        {   
            if(printCount)
                cout<<",";
            cout<<i;
            printCount++;
            
            R.arr[i/32] |= (1U<<(i%32));
        }
    }
    cout<<"}\n"<<endl;

    delete vis; 
}

void rmunreachable(DFA& D,SetOfStates& R,DFA& D1)
{
    unsigned int* stateMapping = new unsigned int[D.n];

    int newStateCounter = 0;
    for(int i=0;i<R.size;i++)
    {
        for(int j=0;j<32;j++)
        {   
            if( (R.arr[i] & (1U<<j)) )
            {
                stateMapping[32*i+j] = newStateCounter++;
            }
        }
    }

    D1.n = newStateCounter;
    D1.m = D.m;
    D1.StartState = stateMapping[D.StartState];
    
    int finalStateSize = newStateCounter/32 + 1;
    D1.FinalState.size = finalStateSize;
    D1.FinalState.arr = new unsigned int[finalStateSize];
    for(int i=0;i<finalStateSize;i++)
    {
        D1.FinalState.arr[i] = 0;
    }
   
    D1.transition = new unsigned int*[D1.n];
    for(int i=0;i<D1.n;i++)
    {
        D1.transition[i] = new unsigned int[D1.m];
        for(int j=0;j<D1.m;j++)
        {
            D1.transition[i][j] = 0;
        }
    }

    for(int i=0;i<R.size;i++)
    {
        for(int j=0;j<32;j++)
        {   
            if( (R.arr[i] & (1U<<j)) )
            {
                int oldMapping = 32*i+j;
                int newMapping = stateMapping[oldMapping];
                
                int index = newMapping/32;
                int pos = newMapping%32;
                if( (D.FinalState.arr[i] & (1U<<j)) )
                {
                    if( (D1.FinalState.arr[index] & (1U<<pos))== 0 )
                    {
                        D1.numFinalStates++;
                        D1.FinalState.arr[index] |= (1U<<(pos));
                    }    
                }

                for(int a=0;a<D1.m;a++)
                {
                    D1.transition[newMapping][a] = stateMapping[D.transition[oldMapping][a]] ;
                }
            }
        }
    }

    delete stateMapping;
}

void findequiv(DFA& D,bool** M)
{
    for(int p=0;p<D.n;p++)
    {
        for(int q=p+1;q<D.n;q++)
        {
            if(M[p][q] || M[q][p])
                    continue;
            
            int ind1 =p/32 ,ind2 = q/32;
            int pos1 = p%32, pos2 = q%32;

            if( ( (D.FinalState.arr[ind1] & (1U<<pos1)) && !(D.FinalState.arr[ind2] & (1U<<pos2)) ) || ( !(D.FinalState.arr[ind1] & (1U<<pos1)) && (D.FinalState.arr[ind2] & (1U<<pos2)) ) )
            {
                M[p][q] = true;
                M[q][p] = true;
            }
        }
    }

    while(true)
    {
        bool flag = 0;

        for(int p=0;p<D.n;p++)
        {
            for(int q=p+1;q<D.n;q++)
            {
                if(M[p][q] || M[q][p])
                    continue;
                for(int a=0;a<D.m;a++)
                {
                    if(M[D.transition[p][a]][D.transition[q][a]])
                    {
                        flag = 1;
                        M[p][q] = true;
                        M[q][p] = true;
                    }
                }
            }
        }

        if(!flag)
            break;
    }
}

void collapse(DFA& D1,bool** M,DFA& D2)
{
    //Equivalent States
    cout<<"++++ Equivalent states\n";
    bool vis[D1.n];
    int stateMapping[D1.n];
    for(int i=0;i<D1.n;i++)
    {
        vis[i] = false;
        stateMapping[i] = i;
    }
    int stateCnt=0;
    for(int i=0;i<D1.n;i++)
    {
        if(vis[i])
            continue;
        cout<<"     Group "<<stateCnt<<": {"<<i;
        vis[i]=1;
        stateMapping[i] = stateCnt;
        for(int j=i+1;j<D1.n;j++)
        {
            if(vis[j] || M[i][j] || M[j][i]) 
                continue;
            cout<<","<<j;
            vis[j]=1;
            stateMapping[j] = stateCnt;
        }
        stateCnt++;
        cout<<"}\n";
    }
    cout<<"\n";

    //DFA Collapse
    D2.n = stateCnt;
    D2.m = D1.m;

    int finalStateSize = 1 + stateCnt/32;
    D2.FinalState.arr = new unsigned int[finalStateSize];
    D2.FinalState.size = finalStateSize;

    D2.transition = new unsigned int*[stateCnt];
    for(int i=0;i<stateCnt;i++)
    {
        D2.transition[i] = new unsigned int[D2.m];
        for(int j=0;j<D2.m;j++)
        {
            D2.transition[i][j] = 0;
        }
    }

    for(int i=0;i<finalStateSize;i++)
    {
        D2.FinalState.arr[i] = 0;
    }

    D2.StartState = stateMapping[D1.StartState];

    for(int i=0;i<D1.FinalState.size;i++)
    {
        for(int j=0;j<32;j++)
        {
            if( D1.FinalState.arr[i]&(1U<<j) )
            {
                int oldState = 32*i+j;
                int newState = stateMapping[oldState];

                int index = newState/32;
                int pos = newState%32;

                if( (D2.FinalState.arr[index]&(1U<<pos)) == 0 ) 
                {
                    D2.numFinalStates++;
                    D2.FinalState.arr[index] |= (1U<<pos);
                } 
            }
        }
    }

    for(int i=0;i<D1.n;i++)
    {
        for(int j=0;j<D2.m;j++)
        {
            int newState = stateMapping[i];
            D2.transition[newState][j] |= stateMapping[D1.transition[i][j]];
        }
    }


}

int main()
{
    string file = "input.txt";
    // cout<<">> ENTER input_filename.extension ( example: input.txt ) :";
    // cin>>file;
    
    NFA N;
    readNFA(file,N);
    printNFA(N);
    cout<<endl;

    DFA D;
    subsetcons(N,D);
    cout<<"++++ Converted  DFA"<<endl;
    printDFA(D);
    cout<<endl;
    
    SetOfStates R;
    findreachable(D,R);

    DFA D1;
    rmunreachable(D,R,D1);
    D.freeDFA();

    cout<<"++++ Reduced DFA after removing unreachable states"<<endl;
    printDFA(D1);
    cout<<endl;

    bool** M = new bool*[D1.n];
    for(int i=0;i<D1.n;i++)
    {
        M[i] = new bool[D1.n];
        for(int j=0;j<D1.n;j++)
        {
            M[i][j] = false;
        }
    }
    findequiv(D1,M);

    DFA D2;
    collapse(D1,M,D2);

    D1.freeDFA();

    cout<<"++++ Reduced DFA after collapsing equivalent states"<<endl;
    printDFA(D2);

    D2.freeDFA();

    if(M)
    {
        for(int i=0;i<sizeof(M)/sizeof(bool *);i++)
        {
            if(M[i])
                delete M[i];
        }
        delete M;
    }

    return 0;
}

