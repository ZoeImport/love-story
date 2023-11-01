#include <iostream>
#include <vector>
using namespace std;

namespace lCode {
auto findDiagonalOrder(vector<vector<int>> &mat) {
  //对角线的坐标值之和相等，方向依据奇偶性判断
  
}
}

int main(){
  //[[1,1,1],[1,0,1],[1,1,1]]
  vector<vector<int>> matrix;
  matrix.push_back({1, 1, 1});
  matrix.push_back({1, 0, 1});
  matrix.push_back({1, 1, 1});
  lCode::findDiagonalOrder(matrix);
  for (auto arr : matrix) {
    cout << "[ ";
    for (auto num : arr) {
      cout << num << " ";
    }
    cout << "]" << endl;
  }
}