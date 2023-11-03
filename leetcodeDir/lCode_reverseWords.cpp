#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace lCode {
    string reverseWords(string s) {
        auto begin=s.begin();
        auto str_end=s.begin();
        stack<string>wordsStack;
        string ans="";
        while (begin!=s.end()) {
            while ((*begin)==' ') {
                begin++;
            }
            str_end=begin;
            while ((*str_end)!=' '&&(str_end!=s.end())) {
                str_end++;
            }
            if (!string(begin,str_end).empty()) {
                wordsStack.push(string(begin,str_end));
                begin=str_end;
            }
        }
        while (!wordsStack.empty()){
            ans+=wordsStack.top();
            wordsStack.pop();
            ans+=!wordsStack.empty()?" ":"";
        }
    return ans;
    }
    string reverseWords2(string s){
        string ans="";
        reverse(s.begin(),s.end());




        return ans;
    }
}

int main(){
    string str1 = "a good   example";
    string str2 = "a good   example";
    cout<<"\""<<lCode::reverseWords(str1)<<"\""<<endl;
    reverse(str2.begin(),str2.end());
    cout<<"\""<<str2<<"\""<<endl;
    
    
    
    
}