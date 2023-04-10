#include <iostream>
#include <sys/time.h>
#include "image.hpp"
#include <arm_neon.h>

using namespace std;


class CumulativeSum {
 public:
  static constexpr int32x4_t zero = {0, 0, 0, 0};
  CumulativeSum() { sum = zero; }

  int32x4_t extend(int32x4_t data) {
    data = vaddq_s32(data, vextq_s32(zero, data, 3));
    data = vaddq_s32(data, vextq_s32(zero, data, 2));
    data = vaddq_s32(data, sum);
    sum = vdupq_laneq_s32(data, 3);
    return data;
  }

 private:
  int32x4_t sum;
};

void IntegralImage(Image<int>& img) {
  int32x4_t zero = { 0, 0, 0, 0 };
  for (size_t i = 1; i < img.height(); i++) {
    int* prev_row = img[i-1];
    int* curr_row = img[i];
    CumulativeSum csum;
    for (size_t j = 0; j < img.width(); j+=4) {
      int32x4_t prev = vld1q_s32(prev_row+j);
      int32x4_t curr = vld1q_s32(curr_row+j);
      vst1q_s32(curr_row+j, vaddq_s32(prev, curr));
      vst1q_s32(prev_row+j, csum.extend(prev));
    }
  }
  int* curr_row = img[img.height()-1];
  CumulativeSum csum;
  for (size_t j = 0; j < img.width(); j+=4) {
    vst1q_s32(curr_row+j, csum.extend(vld1q_s32(curr_row+j)));
  }
}


int main() {
  Image<int> img(7680,4320); //8K image size

  for (size_t i = 0; i < img.height(); i++) {
    for (size_t j = 0; j < img.width(); j++) {
      img[i][j] = i*img.width() + j;
    }
  }

  struct timeval start, end;

  gettimeofday(&start, NULL);

  IntegralImage(img);

  gettimeofday(&end, NULL);

  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
    double(end.tv_usec - start.tv_usec)/1000000.0;
  cout << "Time = " << elapsedtime_sec << std::endl;

  long int checksum = 0;
  for (size_t i = 0; i < img.height(); i++) {
    for (size_t j = 0; j < img.width(); j++) {
      checksum += img[i][j];
    }
  }

  if (checksum == 51113437679616LL)
    cout << "checksum pass" << endl;
  else {
    cout << "CHECKSUM FAIL" << endl;
    cout << "{" << endl;
    for (size_t i = 0; i < 3; i++) {
      cout << "\t" << endl;
      for (size_t j = 0; j < 8; j++) {
        cout << img[i][j] << ",\t";
      }
      cout << "..." << endl;
    }
    cout << "..." << endl << '}' << endl;
  }

  return 0;
}
