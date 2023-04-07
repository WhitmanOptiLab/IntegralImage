#include <iostream>
#include <sys/time.h>
#include "image.hpp"

using namespace std;

void IntegralImage(Image<int>& img) {
  for (int i = 0; i < img.height(); i++) {
    for (int j = 1; j < img.width(); j++) {
      img[i][j] = img[i][j] + img[i][j-1];
    }
  }
  for (int i = 1; i < img.height(); i++) {
    for (int j = 0; j < img.width(); j++) {
      img[i][j] = img[i][j] + img[i-1][j];
    }
  }
}


int main() {
  Image<int> img(7680,4320); //8K image size

  for (int i = 0; i < img.height(); i++) {
    for (int j = 0; j < img.width(); j++) {
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
  else
    cout << "CHECKSUM FAIL" << endl;

  return 0;
}
