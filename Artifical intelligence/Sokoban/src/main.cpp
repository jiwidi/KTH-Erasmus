#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include <tuple>
#include "math.h"

using namespace std;

int heuristic(vector<vector<char>> map,int f,int s,int f2,int s2){
    int au1=0;
    int a=1000;
    int f2;
    int s2;
    for (vector<char>& v: map){
        int au2=0;
        for (char& c: v){
            if(c=='.' && ( distance(f,s,au1,au2)<a ){
                f2=au1;
                s2=au2;
            }
            au2++;
        }
        au1++;

    }
    int value=distance(f,s,f2,s2)
    return value;

} 

double distance(int f,int s,int f2,int s2){
    return sqrt( ( (f2-f)*(f2-f) ) + ( (s2-s)*(s2-s) ) )

}

tuple<int,int,char> choose_movement(vector<vector<char>> map,int f,int s,int f2, int s2,vector<vector<int>> visited,string solution)
{
    tuple<int,int,char> mov=make_tuple(f,s,' ');
    int toph=10000;

    if( heuristic(map,f,s-1,f2,s2)<toph && (map[f][s-1]==' ' || map[f][s-1]=='.') )
    {
        mov=make_tuple(f,s-1,'R');
        toph=heuristic(map,f,s-1,f2,s2);
        cout << "heuristic R" << toph;

    }

    if( heuristic(map,f+1,s,f2,s2)<toph && (map[f+1][s]==' ' || map[f+1][s]=='.') )
    {
        mov=make_tuple(f+1,s,'D');
        toph=heuristic(map,f+1,s,f2,s2);
        cout << "heuristic D" << toph;

    }

    if( heuristic(map,f,s+1,f2,s2)<toph && (map[f][s+1]==' ' || map[f][s+1]=='.') )
    {
        mov=make_tuple(f,s+1,'L');
        toph=heuristic(map,f,s+1,f2,s2);
        cout << "heuristic L" << toph;

    }

    if( heuristic(map,f-1,s,f2,s2)<toph && (map[f-1][s]==' ' || map[f-1][s]=='.') )
    {
        mov=make_tuple(f-1,s,'U');
        toph=heuristic(map,f-1,s,f2,s2);
        cout << "heuristic U" << toph;

    }



    return mov;

}
int main(int argc, char* argv[]) {
	

	if (argc < 3) { 
        cout << "Usage is -in <infile> -out <outdir>\n"; // Inform the user of how to use the program
        cin.get();
        exit(0);
    } else { // if we got enough parameters...
        char* myFile, myPath, myOutPath;
        for (int i = 0; i < argc; i++) { 
        	if(i + 1 != argc){
            	if (strcmp(argv[i], "-f")) {
              		cout << "Hi \n";
              		myFile = argv[i+2];
              		cout << myFile << '\n';
            	}
            //std::cout << argv[i] << " ";
            }
        }

        //... some more code
        ifstream infile(myFile);
        ifstream infile2(myFile);
        string line;
        int length =0;
        while (getline(infile2, line))
        {
            length++;
        }

        //string line;
        char output[500];
        vector< vector<char> > map(length);
        int aux=0;
        int aux2=0;
        int f,s,f2,s2;
        bool found=false;
        while (getline(infile, line))
        {

            cout << line << '\n';

            aux2=0;
            for(char& c : line) {
              map[aux].push_back(c);
              
              if(c=='@'){
                f=aux;
                s=aux2;
                found=true;
              }
              if(c=='.'){
                f2=aux;
                s2=aux2;
              }
              aux2++;

            }
            aux++;  
        }
        
        for(vector<char>& v : map)
        {
            for(char& ve : v)
            {
                cout << ve;
            } 
            cout << '\n';
        }
        if(found)
        {   
            bool goal=false;
            string solution;
            vector<vector<int>> visited(length);
            int aux3=0;
            for (vector<int>& vv: visited){
                for (int i=0;i<length;i++){
                    visited[aux3].push_back(0);
                }
                aux3++;
            }
            while (!goal && aux3!=5)
            {
                aux3++;
                tuple<int,int,char> mov = choose_movement(map,f,s,f2,s2,visited,solution);
                visited[get<0>(mov)][get<1>(mov)]++;
                if (map[get<0>(mov)][get<1>(mov)]=='.')
                {
                    goal=true;
                }
                cout << "Movement \n ";
                cout << get<0>(mov); //<<get<1>(mov);
                cout << ' ';
                cout << get<1>(mov);
                cout << '\n';
                cout << get<2>(mov) << '\n';

                if (get<2>(mov)==' '){
                    goal=true;
                }
                else{
                    map[get<0>(mov)][get<1>(mov)]='@';
                    map[f][s]=' ';
                }

                f=get<0>(mov);
                s=get<1>(mov);
                solution+=get<2>(mov);
                for(vector<char>& v : map)
                {
                    for(char& ve : v)
                    {
                        cout << ve;
                    } 
                    cout << '\n';
                }
                getchar();


            }
            cout << solution;
        }
        
        // cout << '\n';
        
        
        // cout << get<0>(mov); //<<get<1>(mov);
        // cout << '\n';
        // cout << get<1>(mov);
        // cout << '\n';
        return 0;
    }



}

// class Game{
//     public:
//         vector<vector<char>> map;
//         tuple <int,int> coor;
//         // game(vector<vector<char>> map,int f, int s);
//         // setCoor(int f, int s);
//         Game(vector<vector<char>> mapp,int f, int s)
//         {
//             map=mapp;
//             coor=make_tuple(f,s);
//         }
//         void setCoor(int f,int s ) {
//             coor=make_tuple(f,s);
//         }

// };

