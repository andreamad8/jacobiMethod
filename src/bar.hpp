#ifndef JAC_BAR
#define JAC_BAR

#include <algorithm>
#include <cassert>
#include <chrono>
#include <climits>
#include <cmath>
#include <condition_variable> // std::condition_variable
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <math.h>
#include <mutex>
#include <queue>
#include <thread>
#include <tuple>
#include <unistd.h>
#include <vector>

using namespace std;

class barrier {
private:
  int N_THREADS;
  int counts[2];
  int current;
  mutex lock;
  condition_variable condition;

public:
  barrier(int n);
  bool await(function<void()> cb);
};

barrier::barrier(int n) {
  N_THREADS = n;
  counts[0] = 0;
  counts[1] = 0;
  current = 0;
}

bool barrier::await(function<void()> cb) {
  unique_lock<mutex> _lock(lock);
  int my = current;
  counts[my]++;

  if (counts[my] < N_THREADS) {
    condition.wait(_lock, [&] { return counts[my] == N_THREADS; });
    return false;
  } else {
    current ^= 1;
    counts[current] = 0;
    cb();
    condition.notify_all();
    return true;
  }
}

#endif
