#include<time.h>
#include<iostream>
#include<vector>
#include<fstream>
#include<random>
using namespace std;
#define ld long double 
#define INF 1e200

vector<vector<long double>> universe;
vector<long double> temp;
vector<vector<long double>> codebook;

// this function is used to calculate tokura distance between 2 code vectors 
// tokura weights are expilicitly mentioned
ld calculateTokuraDistance(vector<long double > v,vector<long double> u)
{   
	// these are the values of TOkura Distance as specified in assignment
	ld tokura_weights[12] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};
	ld distance_main=0;
    for(int i=0;i<12;i++)
	    distance_main += (tokura_weights[i]) * (u[i]-v[i]) * (u[i]-v[i]) ;
	return distance_main;				
}

// this function is used to make intial codebook
// here initial codebook contians 8 random vectors picked from universe
void make_random_codebook()
{   int d;
    for(int i=1;i<=8;i++) 
    {  
        d=rand();    //randomly piking a number 
        d%=universe.size();   
        codebook.push_back(universe[d]);   // pushing a random vector into codebook
    }
}

//this function is used to read data from universe file 
// it stores entires universe data into a vector of vector
// now entire data looks like this 
// < <vetor1> <vector2> <vector3> ................. <vectorn> > 
// here each vector contins 12 value representing 12 ci's value 
void read_data_from_file()
{   
    long double data;
    string filename = "Universe.txt";
	ifstream recordfile;
	recordfile.open(filename, ios::in);
    if (!recordfile) 
        {
            cout << "\nFile is not Opened\n";    // printing error message if universe file not opened
            recordfile.clear();
        }
    else
        { 
            ld i=0;
            while(!recordfile.eof() && recordfile >> data)   // reading universe file till EOF occurs
                {
                temp.push_back(data);
                i++;
                if(i==12)
                    {
                    universe.push_back(temp);
                    i=0;  
                    temp.clear();
                }
                 }
        }
}

//this function is implementation of kmeans algorithm
long double  kmeans_iteration()
{   
    int count=0;
    vector<vector<long double>> tempcodebook;
    vector<long double> position_vector;
    vector<long double> count_vector;
    vector<vector<long double>> dividevector;
    vector<ld> distances;
    
    
    for(int j=0;j<12;j++)
        temp.push_back(0); 
    
    for(int i=0;i<8;i++)   //intilly making blank temporary codebook
        tempcodebook.push_back(temp);
        
    count_vector=temp;   // intally assigning count vector as all values zero
    temp.clear();


    ld min=INF;
    ld distortion = 0;   // set initial distortion value as zero

    for(int i=0;i<universe.size();i++)  //iterating through all vetors in universe 
    {   
        min=INF;
        int pos=-1;
        for(int j=0;j<8;j++)    // for each vector calculate minimum value corresponding to all codebok values
        {
            ld d=calculateTokuraDistance(universe[i],codebook[j]);
            if(d<min) 
               { 
                 pos=j;
                 min=d;
               }
        } 
        distances.push_back(min);    // feed minimum value to distances vector 

        for(int k=0;k<12;k++)
            tempcodebook[pos][k]+=universe[i][k];

        count_vector[pos] = count_vector[pos] + 1;   // incrementing cluster count for particular codevetor
    }

    for(int i=0;i<distances.size();i++)    // sum up all distance value  and divide by universe size to get distortion value
        distortion+=distances[i];
    distortion/=(universe.size());


   for(int i=0;i<8;i++)
        for(int j=0;j<12;j++)
            tempcodebook[i][j] =  tempcodebook[i][j] / count_vector[i];

    for(int i=0;i<8;i++)
        for(int j=0;j<12;j++)
            codebook[i][j] = ( codebook[i][j] + tempcodebook[i][j] ) / 2 ;

    return distortion;
}
int main()
{    
    int i=2;
    ld res,res_old,diff;

    srand(time(0));
	cout<<"\nReading data from universe file ";
    read_data_from_file();   // reading data from universe file
    cout<<"\n\nImplementing K means algorithm ";
	cout<<"\nIt may take some time to complete all iterations \n";
    make_random_codebook();   // making a random codebook of size 8 

    res_old = kmeans_iteration();   // updating current codebook

    cout<<"\nDistortion after iteraton "<<1<<" is : "<<res_old;
    diff=res_old;
    while(diff>1e-5)  //keep on updating codebook until distortion value is greater than thresold
    {
        res = kmeans_iteration();
        cout<<"\nDistortion after iteraton "<<i<<" is : "<<res;  i++;
        diff=res_old-res;
        res_old=res;
    }  


    cout<<"\n\n\nfinal Code book like this : \n";  
	cout<<"\n-------------------------------------------------------------------------------------------------------------------------------------------";
    for(int i=0;i<8;i++)    // printing final codebook after all procedure
    {   cout<<endl;
        for(int j=0;j<12;j++)
            cout<<codebook[i][j]<<"  ";
    } 
   cout<<"\n---------------------------------------------------------------------------------------------------------------------------------------------";
   cout<<"\n\n";
   system("pause");
   return 0; 
}

