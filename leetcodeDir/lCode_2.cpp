#include <iostream>
#include <list>
using namespace std;


struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
 
namespace lCode {
    auto addTwoNumbers(ListNode *l1, ListNode *l2 ) {
        
    }
}

int main(){
    ListNode *l1=new ListNode(3);
    auto node=ListNode(2,l1);    
    ListNode(2,l1);    
}