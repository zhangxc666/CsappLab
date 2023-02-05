#include <iostream>
using namespace std;
int main(){
    string s="59b997fa";
    for(auto c:s){
        cout<<hex<<(int)c<<endl;
    }    
    /*
35
39
62
39
39
37
66
61
    */
    return 0;
}