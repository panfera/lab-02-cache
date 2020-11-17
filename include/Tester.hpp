// Copyright 2020 hacker
#ifndef INCLUDE_TESTER_HPP_
#define INCLUDE_TESTER_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>

class Tester {
 public:
  explicit Tester(int Lmin, int Lmax);


  void define_series();
  void show_series(std::ostream& out);
  void create_buffer();
  void warming_up(int l);

  void direct();
  void print_direct(std::ostream& out) const;

  void reverse();
  void print_reverse(std::ostream& out) const;

  void random();
  void print_random(std::ostream& out) const;

  void show_Experiment(std::ostream& out);
  friend std::ostream& operator<<(std::ostream &out, const Tester& tester);
  ~Tester();

 private:
  std::vector<int> series;

  int** buf;
  std::vector<int> size_buf;

  int m;
  int size;
  int* testing_buf;
  int L_min, L_max, L1_element;
  const int iter = 1000;
  const int step = 16;
  const int count_elements = 1024/sizeof(int);

  std::vector<double> time_direct;
  std::vector<double> time_reverse;
  std::vector<double> time_random;
};

#endif  // INCLUDE_TESTER_HPP_
