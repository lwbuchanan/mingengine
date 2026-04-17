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

  Vector operator*(Vector v) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * v[i];
    }
    return result;
  }
  Vector operator/(Vector v) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * v[i];
    }
    return result;
  }
  Vector operator+(Vector v) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * v[i];
    }
    return result;
  }
  Vector operator-(Vector v) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * v[i];
    }
    return result;
  }

  Vector operator*(T val) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * val;
    }
    return result;
  }
  Vector operator/(T val) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * val;
    }
    return result;
  }
  Vector operator+(T val) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * val;
    }
    return result;
  }
  Vector operator-(T val) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) * val;
    }
    return result;
  }

  T data[N];
};

template <usize N, typename T>
static Vector<N, T> operator*(T factor, const Vector<N, T> &v) {
  return v * factor;
}

typedef Vector<2, float> v2f;
typedef Vector<3, float> v3f;

typedef Vector<2, int> v2i;
typedef Vector<3, int> v3i;

#endif
