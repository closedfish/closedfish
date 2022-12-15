#include "jsoncpp-master/dist/json/json.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include<time.h>

using namespace std;
using namespace Json;

class Opening{
   public:
       string name;
       string eco;
       //vector<string> moves_black;
       //vector<string> moves_white;
       vector<string> moves;
   Opening(string name1,string eco1,vector<string> moves1){
       name = name1;
       eco = eco1;
       moves = moves1;
       //moves_black = moves_black1;
       //moves_white = moves_white1;
   }

};


string next_move(vector<Opening> openings_vect, int move_number){
    srand(time(0));
    int r = rand()% openings_vect.size();
    Opening o = openings_vect[r];
    cout<<o.name<<endl;
    return o.moves[move_number];
}

vector<Opening> available_openings(vector<Opening> openings_vect, int move_number, string opponents_move)
{
    vector<Opening> available_openings_vect;
    for (int i=0; i<openings_vect.size(); i++)
    {
       Opening o = openings_vect[i];
       if(move_number + 1 < o.moves.size())
       {
        if(o.moves[move_number] == opponents_move)
        available_openings_vect.push_back(o);
       }
    }
    return available_openings_vect;
}


int main(){

    ifstream f("eco.json");
    Value openings;
    Reader reader;
    f >> openings;
    reader.parse(f,openings);
    //vector<string> a = moves_w("1. e4 Nf6 2. e5 Nd5 3. d4 d6 4. c4 Nb6 5. f4 dxe5 6. fxe5 Nc6 7. Nf3 Bg4 8. e6 fxe6 9. c5");
    //for (auto i = a.begin(); i != a.end(); ++i)
        //cout << *i <<endl;
    //string str ("Some string");
    //cout << str.back();
    vector<Opening> openings_vect;
    //vector<string> moves_black1;
    //vector<string> moves_white1;
    vector<string> moves1;
    string moves_str, name1, eco1;
    string s;
    int turn;
    for(int i = 0; i < openings.size(); i++)
    {
         //moves_black1.clear();
         //moves_white1.clear();
         moves1.clear();
         turn = 1;
         moves_str = (openings[i]["moves"]).toStyledString();
         moves_str = moves_str.substr(1,moves_str.size()-3);
         name1 = (openings[i]["name"]).toStyledString();
         name1 = name1.substr(1,name1.size()-3);
         eco1 = (openings[i]["eco"]).toStyledString();
         eco1 = eco1.substr(1,eco1.size()-3);

         istringstream iss(moves_str);
         while (iss >> s)
         {
             string str (s);
             if(str.back()!='.')
             {
                 /*if(turn == 1)
                    moves_white1.push_back(s);
                 else
                    moves_black1.push_back(s);

                 turn = 1 - turn;*/
                 moves1.push_back(s);
             }
         }
        Opening o(name1, eco1, moves1);
        openings_vect.push_back(o);
    }

    cout<<"Type 1 if you're playing with white and 0 if you're playing with black"<<endl;
    cin>>turn;
    string opponents_move;
    string our_move;
    int move_number = 0;
    if(turn == 1)
    {
        cin>>opponents_move;
        openings_vect = available_openings(openings_vect, move_number, opponents_move);
        move_number++;
    }
    while(openings_vect.size() != 0)
    {
        our_move = next_move(openings_vect,move_number);
        cout<<our_move<<endl;
        openings_vect = available_openings(openings_vect, move_number, our_move);
        move_number++;
        cin>>opponents_move;
        openings_vect = available_openings(openings_vect, move_number, opponents_move);
        move_number++;
    }
    cout<<"END"<<endl;

    /*cout<<openings_vect.size()<<endl;
    /*for(int i=0; i<openings_vect.size(); i++)
    {
       Opening o = openings_vect[i];
       if(o.moves[0] != "e4")
        openings_vect.remove(o);
    }*/
    /*remove_unavailable_openings(openings_vect,0,"khm");
    cout<<openings_vect.size()<<endl;
    for(int i=0;i<openings_vect.size();i++)
    {
        Opening o = openings_vect[i];
        cout<<o.name<<endl;
        cout<<o.eco<<endl;
        for(int i=0;i<o.moves.size();i++)
            cout<<o.moves[i]<<" ";
        cout<<endl;
    }*/


return 0;
}

