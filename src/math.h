#ifndef MING_MATH_H
#define MING_MATH_H

#include "defs.h"
#include <cassert>
#include <cmath>

template <usize N, typename T = float64>
struct Vector {
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
  Vector operator/(T val) {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = get(i) / val;
    }
    return result;
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

  // invert
  Vector operator-() {
    Vector result;
    for (usize i = 0; i < N; i++) {
      result[i] = -get(i);
    }
    return result;
  }

  T data[N];
};

template <usize N, typename T>
Vector<N, T> operator*(T factor, Vector<N, T> v) {
  return v * factor;
}

template <usize N, typename T>
T dot(Vector<N, T> v1, Vector<N, T> v2) {
  T sum = 0;
  for (usize i = 0; i < N; i++) {
    sum += v1[i] * v2[i];
  }
  return sum;
}

template <typename T>
Vector<3, T> cross(Vector<3, T> v1, Vector<3, T> v2) {
  return {{
    v1.y() * v2.z() - v1.z() * v2.y(),
    v1.z() * v2.x() - v1.x() * v2.z(),
    v1.x() * v2.y() - v1.y() * v2.x(),
  }};
}

template <usize N, typename T>
T length_squared(Vector<N, T> v) {
  T result = 0;
  for (usize i = 0; i < N; i++) {
    result += v[i]*v[i];
  }
  return result;
}

template <usize N, typename T>
T length(Vector<N, T> v) {
  return std::sqrt(length_squared(v));
}

template <usize N, typename T> 
Vector<N, T> normalize(Vector<N, T> v) {
  return v/length(v);
}

typedef Vector<2, float64> v2f;
typedef Vector<3, float64> v3f;
typedef Vector<4, float64> v4f;

typedef Vector<2, int> v2i;
typedef Vector<3, int> v3i;
typedef Vector<4, int> v4i;
typedef Vector<4, uint8> rgba;

template <usize M, usize N, typename T = float64>
struct Matrix {
  inline T &get(usize i, usize j) {
    assert(i < M && j < N);
    return data[i][j];
  }
  inline void set(usize i, usize j, T val) {
    assert(i < M && j < N);
    data[i][j] = val;
  }

  Vector<M,T> operator*(Vector<N,T> vec) {
    Vector<M,T> result;
    for (usize m = 0; m < M; m++) {
      T sum = 0;
      for (usize n = 0; n < N; n++) {
        sum += get(m, n) * vec[n];
      }
      result[m] = sum;
    }
    return result;
  }

  template <usize P>
  Matrix<M,P,T> operator*(Matrix<N,P,T> mat) {
    Matrix<M,P,T> result;
    for (usize m = 0; m < M; m++) {
      for (usize p = 0; p < P; p++) {
        T sum = 0;
        for (usize n = 0; n < N; n++) {
          sum += get(m, n) * mat.get(n, p);
        }
        result.set(m, p, sum);
      }
    }
    return result;
  }


  T data[M][N];
};

// template <typename T = float64>
// T det(Matrix<3, 3, T> mat) {
//   return mat.data[0][0] * mat.data[1][1] * mat.data[2][2]
//     + mat.data[0][1] * mat.data[1][2] * mat.data[2][0]
//     + mat.data[0][2] * mat.data[1][0] * mat.data[2][1]
//
//     - mat.data[2][0] * mat.data[1][1] * mat.data[0][2]
//     - mat.data[2][1] * mat.data[1][2] * mat.data[0][0]
//     - mat.data[2][2] * mat.data[1][0] * mat.data[0][1];
// }

template <typename T = float64>
Matrix<4,4,T> translation(Vector<3,T> pos) {
  return Matrix<4,4>{{
    {1,0,0,pos.x()},
    {0,1,0,pos.y()},
    {0,0,1,pos.z()},
    {0,0,0,1},
  }};
}

#endif
