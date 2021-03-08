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

        // S -> Start States ,F -> Final States
        unsigned int S,F;

        // Transition Table
        unsigned int** transition;

        NFA()
        {
            n = 0;
            m = 0;
            transition = NULL;
        }

        // Take n,m as constructor
        NFA(int numStates,int numAlphabets):n(numStates),m(numAlphabets)
        {
            transition = new unsigned int*[n];
            for(int i=0;i<n;i++)
            {
                transition[i] = new unsigned int[m];
                for(int j=0;j<m;j++)
                {
                    transition[i][j] = 0;
                }
            }
        }

        // Assume copy to empty NFA 
        NFA& operator=(const NFA &t)
        {
            n = t.n;
            m = t.m;
            F = t.F;
            S = t.S;
            transition = new unsigned int*[n];
            for(int i=0;i<n;i++)
            {
                transition[i] = new unsigned int[m];
                for(int j=0;j<m;j++)
                {
                    transition[i][j] = t.transition[i][j];
                }
            }
            return *this;
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


// Function to read an NFA from a given file.
void readNFA(string fileName,NFA& N)
{   
    ifstream fin;
    fin.open(fileName.c_str());

    int n,m;
    fin>>n>>m;

    NFA temp(n,m);
    unsigned int F=0,S=0;

    int x=1;
    // Start States reading
    while(x!=-1)
    {
        fin>>x;
        S |= (1U<<x);
    }

    x=1;
    // Final States reading
    while(x!=-1)
    {
        fin>>x;
        F |= (1U<<x);
    }

    x=1;
    // Transition Function reading
    while(x!=-1)
    {
        fin>>x;
        if(x==-1)
            break;
        int y,z;
        fin>>y>>z;
        temp.transition[x][y] |= (1U<<z);
    }

    fin.close();

    temp.F = F;
    temp.S = S;

    N = temp;
} 


// Function to print NFA
void printNFA(const NFA& N)
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
        if( (N.S & (1<<i)) )
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
        if( (N.F & (1<<i)) )
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

// NFA to DFA Converter
void subsetcons(const NFA& N)
{

}

int main()
{
    string file = "input.txt";
    // cout<<">> ENTER input_filename.extension ( example: input.txt ) :";
    // cin>>file;
    
    NFA N;

    readNFA(file,N);

    printNFA(N);

    return 0;
}

