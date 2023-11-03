#include <cstring>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
namespace lCode {
    auto longestPalindrome(string s){
        if (s.length()<2) {
            return s;
        }
        auto isPalindrome=[](string str)->bool{
            if(str.length()<2)return true;
            auto begin=str.begin();
            auto back=str.end()-1;
            while (begin<back) {
                if (*begin!=*back) {
                    return false;
                }
                begin++;
                back--;
            }
        return true;    
        };
        if (isPalindrome(s)) {
            return s;
        }
        auto begin=s.begin();
        auto back=s.end();
        for (auto turns=0;turns<=s.length();turns++) {
            for (auto offset=0;offset<turns;offset++) {
                cout<<string(begin,back)<<endl;
                cout << "==========================" << endl;
                if(isPalindrome(string(begin,back)))
                    return string(begin,back);
                 
                begin--;
                back--;
                // cout<<string(begin,back)<<endl;
                
            }
            begin = s.begin();
            back = s.end();
            begin+=turns;
                // cout<<string(begin,back)<<endl;
        }
       
        return string();
    }

    string longestPalindrome1(string s) {
        int len = s.size();
        string ans = "";
        vector<vector<bool>> dp(len, vector<bool>(len, false));
        for (size_t k = 0; k < len; k++) {
            for (size_t i = 0; i + k < len; i++) {
                int j = i + k;
                if (i == j)
                    dp[i][j] = true;
                else if (k == 1)
                    dp[i][j] = (s[i] == s[j]);
                else {
                    dp[i][j] = dp[i + 1][j - 1] && s[i] == s[j];
                }
                if (dp[i][j] && k + 1 > ans.size()) {
                    ans = s.substr(i, k + 1);
                }
            }
        }
        return ans;
    }
}

int main(){

        string str =
            " qkajbumzdzkiplmbcpnhbzweoevrvbptpozhtrfntszvnwbdahvkykmezrwruhvvslngruvwqebudtfxgpbmwefczwrecpqjegxkqknpobzkemmtruidulnxgntjxcmxtwmlxhzmbqfqylwvzjyojhfawwuupiipvxjiyxkqvsxbhgzzegfkdihizvjoxzrmeorikzsdyphbujaqmykrfblneqmwwxsoonzsgvligqxrrumspylfvquklbanjhkudlprwoycpxdsueokruoofyubirbhbyfuvgllijywuqmkcsfjttbnmelrylivkefllepgxnoeummujbaoyvryukyoumvuxezegpwgmwsupjuaarvbtbfmisrifjadqjypmzipvjysgakvjhfeaqwpsqijvqibshctuabwqqsjwotjopahoaptmxkwerkjkmwiodgblhtnhykzjuaoluoyokroxuvqtkpggfanzabgjejdfsgybhxbscubdpufywbxgutheskuhixasnksoayjngvhfoxxclykfobrwxjwgefarzczvptlfrgrtrjcemaeihpukhbeoezgvrwxgyhpkkfvmfvquwtswkdwqqgrgasopladdnteulqofmjhewpghkibbrewnhdllfppctgkfkoedoiwqocnpvfviochrokrgrzthrvyhqfsrzyyvqwkhuzsrkfaympcdodkwaojnghzytkhf";
            cout<< lCode::longestPalindrome(str) << endl;
        auto isPalindrome = [](string str) -> bool {
          if (str.length() < 2)
            return true;
          auto begin = str.begin();
          auto back = str.end() - 1;
          while (begin < back) {
            if (*begin != *back) {
              return false;
            }
            begin++;
            back--;
          }
          return true;
        };
        // cout<<isPalindrome("s")<<" =======================================";
}