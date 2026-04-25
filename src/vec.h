#ifndef MING_VEC_H
#define MING_VEC_H

#include "defs.h"
#include <cassert>

template <usize N, typename T = float> struct Vector {
  inline T &get(usize i) {
    assert(i < N);
    return data[i];
  }
  inline T &operator[](usize i) { return get(i); }
  inline T &x() { return get(0); }
  inline T &y() { return get(1); }
  inline T &z() { return get(2); }
  inline T &w() { return get(3); }
  inline T &r() { return get(0); }
  inline T &g() { return get(1); }
  inline T &b() { return get(2); }
  inline T &a() { return get(3); }

  // Aritmetic operator overloads
  Vector operator*(Vector v) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * v[i];
    }
    return result;
  }
  void operator*=(Vector v) {
    for (usize i = 0; i < N; i++) {
      data[i] *= v[i];
    }
  }
  Vector operator/(Vector v) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * v[i];
    }
    return result;
  }
  void operator/=(Vector v) {
    for (usize i = 0; i < N; i++) {
      data[i] /= v[i];
    }
  }
  Vector operator+(Vector v) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * v[i];
    }
    return result;
  }
  void operator+=(Vector v) {
    for (usize i = 0; i < N; i++) {
      data[i] += v[i];
    }
  }
  Vector operator-(Vector v) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * v[i];
    }
    return result;
  }
  void operator-=(Vector v) {
    for (usize i = 0; i < N; i++) {
      data[i] -= v[i];
    }
  }
  Vector operator*(T val) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * val;
    }
    return result;
  }
  void operator*=(T val) {
    for (usize i = 0; i < N; i++) {
      data[i] *= val;
    }
  }
  void operator/=(T val) {
    for (usize i = 0; i < N; i++) {
      data[i] /= val;
    }
  }
  Vector operator+(T val) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * val;
    }
    return result;
  }
  void operator+=(T val) {
    for (usize i = 0; i < N; i++) {
      data[i] += val;
    }
  }
  Vector operator-(T val) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * val;
    }
    return result;
  }
  void operator-=(T val) {
    for (usize i = 0; i < N; i++) {
      data[i] -= val;
    }
  }

  T data[N];
};

template <usize N, typename T>
Vector<N, T> operator*(T factor, Vector<N, T> v) {
  return v * factor;
}

template <usize N, typename T> T dot(Vector<N, T> v1, Vector<N, T> v2) {
  T sum = 0;
  for (usize i = 0; i < N; i++) {
    sum += v1[i] * v2[i];
  }
  return sum;
}

template <typename T> Vector<2, T> perpendicular_clockwise(Vector<2, T> v) {
  return Vector<2, T>{v.y(), -v.x()};
}

typedef Vector<2, float64> v2f;
typedef Vector<3, float64> v3f;

typedef Vector<2, int> v2i;
typedef Vector<3, int> v3i;
typedef Vector<4, uint8> rgba;

#endif
