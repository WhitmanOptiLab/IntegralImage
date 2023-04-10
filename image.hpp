#include <stddef.h>
#include <stdio.h>
#include <jpeglib.h>

#include <iostream>

// Image class

template<class Pix>
class Image {
 public:
  Image(int height = 0, int width = 0) 
      : _width(width), _height(height) {
    allocate_array();
  }

  Image(const Image<Pix>& other) 
      : _width(other._width), _height(other._height) {
    allocate_array();
    for (int col = 0; col < _width; col++) {
      for (int row = 0; row < _height; row++) {
        _array[row*_width + col] = other._array[row*_width + col];
      }
    }
  }

  Image& operator=(const Image& other) {
    dealloc_array();
    _width = other._width;
    _height = other._height;
    allocate_array();
    for (int col = 0; col < _width; col++) {
      for (int row = 0; row < _height; row++) {
        _array[row*_width + col] = other._array[row*_width + col];
      }
    }
    return *this;
  }

  Pix* operator[](const size_t i) {
    return _array+(_width*i);
  }

  ~Image() {
    dealloc_array();
  };

  size_t width() const { return _width; }
  size_t height() const { return _height; }

 protected:
  void dealloc_array() {
    if (_array)
      delete[] _array; 
    _array = NULL;
  }
  void allocate_array() {
    //Check for NULL
    if (_width* _height > 0)
      _array = new Pix[_width*_height];
    else
      _array = NULL;
  }
  size_t _width, _height;
  Pix *_array;
};

