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
            cout << "\nFile is not Opened\n";      // printing error message if universe file not opened
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

// this function is used to make intial codebook
// here initial codebook contians only one vector which is centroid of universe
void make_initial_codebook()
{
    for(int i=0;i<12;i++)
        temp.push_back(0);
    codebook.push_back(temp);   // intializing codebook with all values as zero
    temp.clear();
    for(int i=0;i<universe.size();i++)   // reading data from universe vector 
        for(int j=0;j<12;j++)
            codebook[0][j]+=universe[i][j];   // adding ci's value to respective ci value in codebook
  
    for(int i=0;i<12;i++)
        codebook[0][i] /= universe.size();    //taking centorid value of all respective ci's
}


//This function is used to split current codebook
// here spliting parameter is 0.003
// it takes each vector from current codebook and split it into 2 vectors using spliting parameter 
void spilt_current_codebook()
{  
    vector<vector<long double>> newcodebook;
    for(int i=0;i<codebook.size();i++)   
    {   
        vector<ld> temp1;
        vector<ld> temp2;
        for(int j=0;j<12;j++)
        {
            temp1.push_back(codebook[i][j]-0.003);
            temp2.push_back(codebook[i][j]+0.003);
        }
        newcodebook.push_back(temp1);
        newcodebook.push_back(temp2);
    }
    codebook.clear();
    for(int i=0;i<newcodebook.size();i++)
        codebook.push_back(newcodebook[i]);
}


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


// this function is kmeans algorithm 
// update codebook based on kmeans algorithm
long double updatecodebook_using_kmeans()
{   
    int count=0;
    vector<vector<long double>> tempcodebook;
    vector<long double> position_vector;
    vector<long double> count_vector;
    vector<vector<long double>> dividevector;
    vector<ld> distances;
    
    for(int j=0;j<12;j++)
        temp.push_back(0); 
    
    for(int i=0;i<codebook.size();i++)
        tempcodebook.push_back(temp);   
    count_vector=temp;
    temp.clear();
    ld distortion = 0;   // set initial distortion value as zero
    

    ld min;
    for(int i=0;i<universe.size();i++)  //iterating through all vetors in universe 
    {   
        min=INF;
        int pos=-1;
        for(int j=0;j<codebook.size();j++)    // for each vector calculate minimum value corresponding to all codebok values
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


   for(int i=0;i<codebook.size();i++)
        for(int j=0;j<12;j++)
            tempcodebook[i][j] =  tempcodebook[i][j] / count_vector[i];

    for(int i=0;i<codebook.size();i++)     // updating codebook
        for(int j=0;j<12;j++)
            codebook[i][j] = ( codebook[i][j] + tempcodebook[i][j] ) / 2 ;

    return distortion;
}

//this function is used to print codebook
void print_codebook()
{
    cout<<"\n-------------------------------------------------------------------------------------------------------------------------------------------";
    for(int i=0;i<codebook.size();i++)
    {   cout<<endl;
        for(int j=0;j<12;j++)
            cout<<codebook[i][j]<<"  ";
    } 
    cout<<"\n-------------------------------------------------------------------------------------------------------------------------------------------";
}

int main()
{   cout<<"\nReading data from universe file  ";
    read_data_from_file();     // step 1 : read data
    make_initial_codebook();   // step 2 : make intial codebook
    ld res,res_old,diff;

    cout<<"\nInitial Code book like this : ";
    
    print_codebook();         // print intial codebook
    cout<<"\nImplementing LBG Algorithm ";
    while(codebook.size()<8)     //step 3: split and update codebook until codebook size !=  8
    {   cout<<"\n";
		cout<<"\nthis may take some time to complete all iterations\n";
        spilt_current_codebook();
        res_old = updatecodebook_using_kmeans();    // calling kmeans algo to update current codebook
        diff=res_old;
        while(diff>1e-5)     //keep on updating codebook until distortion value is greater than thresold
        {
            res = updatecodebook_using_kmeans();   //calling kmeans algo to update current codebook
            diff=res_old-res;
            res_old=res;
        }  
        cout<<"\nDistortion after this split is "<<res;
        cout<<"\nfinal Code book after having codebook of size  : "<<codebook.size();
        print_codebook();
    }
cout<<"\n";
system("pause");
return 0;
}

