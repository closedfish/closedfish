#include "dist/json/json.h"
#include "dist/json/json-forwards.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;
using namespace Json;

class Otvaranje{
   public:
       string name;
       string eco;
       vector<string> moves;
   Otvaranje(string a,string b,vector<string> c){
       name=a;
       eco=b;
       moves=c;
   }


};

vector<string> moves_str_to_vector(string m){

    vector<string> a;
    istringstream iss(m);
    string s;
    while (iss >> s)
    {
       string str (s);
        if(str.back()!='.')
          a.push_back(s);
    }

    return a;

}

int next_move(vector<Otvaranje> o){
    int r = rand()%(o.size());
    return r;
}


int main(){

    ifstream f("eco.json");
    Value openings;
    Reader reader;
    f >> openings;
    reader.parse(f,openings);
    /*vector<string> a = moves_str_to_vector("1. e4 Nf6 2. e5 Nd5 3. d4 d6 4. c4 Nb6 5. f4 dxe5 6. fxe5 Nc6 7. Nf3 Bg4 8. e6 fxe6 9. c5");
    for (auto i = a.begin(); i != a.end(); ++i)
        cout << *i <<endl;
    string str ("Some string");
    cout << str.back();*/
    for(int i=0;i<openings.size();i++)
        cout<<openings[i]<<endl;

return 0;
}

