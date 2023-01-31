#include <iostream>
using namespace std;
int main(){
    string s="maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?";
    for(int i=0;i<s.size();i++){
        for(auto c:"flyers"){
            if(c==s[i]){
                cout<<i<<' '<<c<<endl;
                continue;
            }
        }
    }
}