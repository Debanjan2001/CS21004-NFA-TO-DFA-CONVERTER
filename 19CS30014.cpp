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

        // Free Space upon destruction
        ~NFA()
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

        ~SetOfStates()
        {
            if(arr)
            {
                delete arr;
                cout<<"Success"<<endl;
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

        DFA()
        {
            n = 0,m = 0;
            StartState = 0;
            FinalState = SetOfStates();
            transition = NULL;
        }

        ~DFA()
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
    cout<<"++++ INPUT NFA"<<endl;
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
    int statecount = 0;
    for(int i=0;i<D.FinalState.size;i++)
    {
        for(int j=0;j<32;j++)
        {
            if( (D.FinalState.arr[i] &(1U<<j)) )
                statecount++;
        }
    }
    cout<<"     "<<statecount<<" final states"<<endl;

    //Transition Function
    cout<<"     Trasition function: "<<((D.n>64)?"Skipped":"")<<endl;
    //TO be written.
}


// NFA to DFA Converter
void subsetcons(NFA& N,DFA& D)
{
    //New sizes
    int n =(1<<N.n), m = N.m;
    D.n= n;
    D.m = m;

    // Start State 
    D.StartState = N.StartState;

    //Final States
    int size = (n/32)+1;

    D.FinalState.size = size;
    D.FinalState.arr = new unsigned int[size];

    unsigned int F = N.FinalState;

    for(int i=0;i<n;i++)
    {
        if( (F & i) )
        {
            int index = i/32;
            int pos = i%32;

            D.FinalState.arr[index] |= (1U<<pos); 
        }
    }

    //Transition Function
    D.transition = new unsigned int*[n];
    for(int i=0;i<n;i++)
    {
        D.transition[i] = new unsigned int[m];
        for(int j=0;j<m;j++)
        {
            for(int k=0;k<N.n;k++)
            {
                if( i & k )
                {
                    D.transition[i][j] |= N.transition[k][j];
                }
            }
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

    DFA D;
    subsetcons(N,D);
    printDFA(D);
    

    return 0;
}

