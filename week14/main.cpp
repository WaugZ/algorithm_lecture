//
//  main.cpp
//  GA
//
//  Created by 王资 on 2018/12/6.
//  Copyright © 2018 王资's. All rights reserved.
//

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::sort;

// f(x)=xsin(7/x)+3;

class Genetic {
private:
  int len; // length of bits
  int top; // the top number according to #of bits
  int n;  // max number of iteration
  int m;  // the number of one gener
  double p_c;
  double p_m;
  
  double fitness(int id) {
    double y = id / double(top);
    double epthlon = 1e-20;
    return y * sin(7. / (y + epthlon)) + 3.;
  }
  
  // 保留原排序，只对index排序
  template <typename T>
  vector<size_t> sort_indexes(const vector<T> &v) {
    
    // 初始化索引向量
    vector<size_t> idx(v.size());
    //使用iota对向量赋0~？的连续值
    iota(idx.begin(), idx.end(), 0);
    
    // 通过比较v的值对索引idx进行排序
    sort(idx.begin(), idx.end(),
         [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});
    return idx;
  }
  
  void gen() {
    random_device rd;
    mt19937 gen(rd());
    
    int iter = 0;
    vector<int> group(m);
    vector<int> choose(m);
    vector<double> fs(m);
    vector<double> prob(m);
    //  random initialize
    for(int i = 0; i < m; i++) {
      uniform_int_distribution<> dis(0, top);
      int id = dis(gen);
      group[i] = id;
      choose[i] = 0;
    }
    while (true) {
      double sum_fit = 0.;
      //  fitness for every identity
      for(int i = 0; i < m; i++) {
        fs[i] = fitness(group[i]);
        sum_fit += fs[i];
      }
      if(iter + 1 == n) {
        break;
      }
      //  probobilty and the number of each chromosome to be chosen
      int left = m;
      for(int i = 0; i < m; i++) {
        prob[i] = fs[i] / sum_fit;
        choose[i] = int(prob[i] * m);
        prob[i] = prob[i] * m - choose[i];
        left -= choose[i];
      }
      vector<size_t> top_k = sort_indexes(prob);
      while(left > 0) {
        choose[top_k[0]]++;
        top_k.erase(top_k.begin());
        left--;
      }
      
      //  new general
      vector<int> new_group(m);
      int choose_ind = 0;
      for(int i = 0; i < m; i++) {
        if(choose[choose_ind] > 0) {
          new_group[i] = group[choose_ind];
          choose[choose_ind]--;
        } else {
          choose_ind++;
          i--;
        }
      }
      //  chromosome exchange and variation
      uniform_real_distribution<> dis(0, 1);
      uniform_int_distribution<> dice(0, len);
      for(int i = 0; i < m; i += 2) {
        int child_0 = new_group[i];
        int child_1 = new_group[i+1];
        //  exchange
        if(dis(gen) < p_c) {
          int shift = dice(gen);
          int high_0 = child_0 >> shift << shift;
          int low_0 = child_0 - high_0;
          int high_1 = child_1 >> shift << shift;
          int low_1 = child_1 - high_1;
          child_0 = high_0 + low_1;
          child_1 = high_1 + low_0;
        }
        // variation
        if(dis(gen) < p_m) {
          int pos = dice(gen);
          child_0 ^= 1 << pos;
        }
        if(dis(gen) < p_m) {
          int pos = dice(gen);
          child_1 ^= 1 << pos;
        }
        group[i] = child_0;
        group[i + 1] = child_1;
      }
      
      // debug
//      cout << "Iter " << iter << ' ';
//      for(auto i : group) {
//        cout << i << ' ';
//      }
//      cout << endl;
      iter++;
    }
    
    //  the max fitness
    auto pos = std::max_element(fs.begin(), fs.end());
    double best = *pos;
    long offset = pos - fs.begin();
    cout << "The best pos is " << group[offset] << " and the point " <<  best << endl;

  }
  
public:
  Genetic() {
    len = 10;
    top = 1023;
    n = 0xffff;
//    int id = 0b1111111111;
//    cout << id << ' ' << fitness(id) << endl;
    m = 252;
    p_c = 1.;
    p_m = .01;
    gen();
  }
  
  
};

int main(int argc, const char * argv[]) {
  Genetic g;
  return 0;
}
