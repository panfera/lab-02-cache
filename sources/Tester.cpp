// Copyright 2020 hacker
#include "Tester.hpp"
#include <cmath>
#include <random>
#include <ctime>

Tester::Tester(int Lmin, int Lmax):
    series(0), buf(NULL), size_buf(0), m(0), size(0), testing_buf(0),
    L_min(Lmin), L_max(Lmax), L1_element(L_min*1024/sizeof(int)),
    time_direct(0), time_reverse(0), time_random(0){
}
Tester::~Tester() {
  for (size_t i = 0; i < size_buf.size(); ++i)
  {
    delete[] buf[i];//delete a - освобождение памяти из-под указателя
  }//delete[] a -освобождение памяти из-под массива
  delete[] buf;
}

void Tester::define_series() {
  double begin = L_min * 0.5;
  double end = 1.5 * L_max;
  double element = begin;
  double val = 2; //K
  int x = round(log2(L_min));

  while (element < end){
    series.push_back(element);
    element = pow(val, x);
    ++x;
  }
  series.push_back(end);
}

void Tester::show_series(std::ostream& out) {
  for (size_t i = 0; i < series.size(); ++i)
  {
    out << series[i] << " ";
  }
  out << std::endl;
}

void Tester::create_buffer() {
  std::mt19937 gen; // создаем объект
  gen.seed(time(0)); // инициализируем его начальным значением

  buf = new int* [series.size()];
  int* d_buf;
  for (size_t i = 0; i < series.size(); ++i) {
    size_buf.push_back(series[i] * 1024 / sizeof(int));
    d_buf = new int [size_buf[i]];

    for (int k = 0; k < size_buf[i]; ++k) {
      d_buf[k] = gen();
    }
    buf[i] = d_buf;
  }
}

void Tester::warming_up(int l){
  size = size_buf[l];
  int count;
  m = 0;
  if (size < L1_element){
    count = L1_element/size;
    for (int k = 0; k < count; ++k)
      for (int i = 0; i < size; i += 16)
        m = testing_buf[i];
  } else{
    for (int i = 0; i < L1_element; i += 16)
      m = testing_buf[i];
  }
}

void Tester::direct() {
  time_direct.clear();

  for (size_t l = 0; l < size_buf.size(); ++l){
    m = 0;
    testing_buf = buf[l];
    size = size_buf[l];
    warming_up(l);

    auto start = std::chrono::steady_clock::now();
    for (int k = 0; k < iter; ++k) {
      for (int i = 0; i < size; i+=16)
        m = testing_buf[i];
    }
    auto end = std::chrono::steady_clock::now();
    auto  elapsed_seconds_dur =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    double elapsed_seconds = (elapsed_seconds_dur).count();
    elapsed_seconds/=iter;
    elapsed_seconds/=(1+size/16);
    time_direct.push_back(round(elapsed_seconds));
  }
}

void Tester::reverse(){
  time_reverse.clear();

  for (size_t l = 0; l < size_buf.size(); ++l){
    m = 0;
    testing_buf = buf[l];
    size = size_buf[l];
    warming_up(l);

    auto start = std::chrono::steady_clock::now();
    for (int k = 0; k < iter; ++k) {
      for (int i = size_buf[l] - size_buf[l] % 16 - 1; i >= 0; i -= 16)
        m = testing_buf[i];
    }
    auto end = std::chrono::steady_clock::now();
    auto  elapsed_seconds_dur =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    double elapsed_seconds = (elapsed_seconds_dur).count();
    elapsed_seconds/=iter;
    elapsed_seconds/=(1+size/16);
    time_reverse.push_back(round(elapsed_seconds));
  }
}
void Tester::random(){
  std::mt19937 gen2;
  gen2.seed(time(0));
  time_random.clear();

  for (size_t l = 0; l < size_buf.size(); ++l){
    std::vector<int> index;
    int count = (size_buf[l] == 0) ? 0 : (1 + (size_buf[l] - 1) / 16);
    for (int i = 0; i < count; ++i) {
      m = gen2() % size_buf[l];
      index.push_back(m);
    }

    m = 0;
    size = size_buf[l];
    testing_buf = buf[l];

    warming_up(l);

    auto start = std::chrono::steady_clock::now();

    for (int k = 0; k < iter; ++k) {
      for (const auto& number : index){
        m = testing_buf[number];
      }
    }
    auto end = std::chrono::steady_clock::now();
    auto  elapsed_seconds_dur =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    double elapsed_seconds = elapsed_seconds_dur.count();

    elapsed_seconds/=iter;
    elapsed_seconds/=(1+size/16);
    time_random.push_back(round(elapsed_seconds));
  }
}

void Tester::print_direct(std::ostream& out) const{
  out << "investigation:\n\ttravel_order: \"direction\"\n\texperiments:\n";
  for (size_t l = 0; l < size_buf.size(); ++l){
    out << "\t- experiment:\n\t\tnumber: " << l + 1
        << "\n\t\tinput_data:\n\t\t\t  buffer_size: \"" << series[l]
        << "kb\"\n\t\tresults:\n\t\t duration: \""<< time_direct[l]
        << "ns\"" << std::endl;
  }
  out << std::endl;
}

void Tester::print_reverse(std::ostream& out) const{
  out << "investigation:\n\ttravel_order: \"reverse\"\n\texperiments:\n";
  for (size_t l = 0; l < size_buf.size(); ++l){
    out << "\t- experiment:\n\t\tnumber: " << l + 1
        << "\n\t\tinput_data:\n\t\t\t  buffer_size: \"" << series[l]
        << "kb\"\n\t\tresults:\n\t\t duration: \""<< time_reverse[l]
        << "ns\"" << std::endl;
  }
  out << std::endl;
}

void Tester::print_random(std::ostream& out) const{
  out << "investigation:\n\ttravel_order: \"random\"\n\texperiments:\n";
  for (size_t l = 0; l < size_buf.size(); ++l){
    out << "\t- experiment:\n\t\tnumber: " << l + 1
        << "\n\t\tinput_data:\n\t\t\t  buffer_size: \"" << series[l]
        << "kb\"\n\t\tresults:\n\t\t duration: \""<< time_random[l]
        << "ns\"" << std::endl;
  }
  out << std::endl;
}

void Tester::show_Experiment(std::ostream& out) {
  define_series();
  create_buffer();
  direct();
  reverse();
  random();
  out << (*this);
}
std::ostream& operator<< (std::ostream& out, const Tester& tester){
  tester.print_direct(out);
  tester.print_reverse(out);
  tester.print_random(out);
  return out;
}
