#ifndef VECT_PREF_DEFAULT_H
#define VECT_PREF_DEFAULT_H

#include "array_wrapper.hpp"
#include <cmath>

template<typename T>
//By default our 'SIMD' version of T is just T.
struct preffered_vector_type{
  constexpr static int width = 1;
  using type = array_wrapper<T>; //By wrapping it with array_wrapper it behaves like a normal vector unit.
};

template<typename T>
class vectorized_type{
  //The vectorized type, internally
  using Simd = typename preffered_vector_type<T>::type;
  Simd mVal;
public:
    //How many elements the SIMD element has
    static constexpr int Width = preffered_vector_type<T>::width;

  inline Simd inner(){return mVal;}

  inline void set_1(T val){
    for(int i = 0; i < Width; i++){
        mVal[i] = val;
    }
  }
  vectorized_type() {}
  vectorized_type(T val) {set_1(val);}
  vectorized_type(Simd val) : mVal(val) {}

  void set(int index, T val){ mVal[index] = val;}
  //array indexing
  const T operator[](std::size_t i) const {return mVal[i];}
  // T& operator[](std::size_t i){return mVal[i];}

  //increment
  vectorized_type& operator++(){
    return ((*this) += vectorized_type(1));
  }

  //decrement
  vectorized_type& operator--(){
    return ((*this) -= vectorized_type(1));
  }

  inline float sum() const{
    float sum = 0;
    for(int i = 0; i < Width; i++){
      sum += mVal[i];
    }
    return sum;
  }

  //inline vectorized_type sqrt() const {return vec::sqrt(mVal);}

  vectorized_type& operator=(const vectorized_type& rhs){
    mVal = rhs.mVal;
    return *this;
  }
  //assign add
  inline vectorized_type& operator+=(const vectorized_type& rhs){
    auto sum = (*this)+rhs;
    return (*this) = sum;
  }
  //assign subtract
  inline vectorized_type& operator-=(const vectorized_type& rhs){
    return (*this) = (*this)-rhs;
  }
  //assign mult
  inline vectorized_type& operator*=(const vectorized_type& rhs){
    return (*this) = (*this)*rhs;
  }
  //assign divide
  inline vectorized_type& operator/=(const vectorized_type& rhs){
    return (*this) = (*this)/rhs;
  }
  //addition
  // friends defined inside class body are inline and are hidden from non-ADL lookup
 friend vectorized_type operator+(const vectorized_type lhs,        // passing lhs by value helps optimize chained a+b+c
                    const vectorized_type rhs) // otherwise, both parameters may be const references
 {
  //  std::cout << "vectorized" << std::endl;
   return lhs.mVal + rhs.mVal;
 }

 //subtraction
friend vectorized_type operator-(const vectorized_type lhs, const vectorized_type rhs)
{
  return lhs.mVal - rhs.mVal;
}

//multiplication
inline friend vectorized_type operator*(const vectorized_type lhs, const vectorized_type rhs)
{
 return lhs.mVal * rhs.mVal;
}

//division
inline friend vectorized_type operator/(const vectorized_type lhs, const vectorized_type rhs)
{
  return lhs.mVal / rhs.mVal;
}

//some basic mathematical operations
//These are overwritten in the preferences files to use custom SIMD
// algorithms or instructions where applicable.
inline vectorized_type sqrt() const{
  vectorized_type ret;
  for(int i = 0; i < Width; i++){
    ret.set(i, std::sqrt(mVal[i]));
  }
  return ret;
}
inline vectorized_type log() const{
  vectorized_type ret;
  for(int i = 0; i < Width; i++){
    ret.set(i,std::log(mVal[i]));
  }
  return ret;
}
inline vectorized_type cos() const{
  vectorized_type ret;
  for(int i = 0; i < Width; i++){
    ret.set(i,std::cos(mVal[i]));
  }
  return ret;
}
inline vectorized_type sin() const{
  vectorized_type ret;
  for(int i = 0; i < Width; i++){
    ret.set(i,std::sin(mVal[i]));
  }
  return ret;
}
inline vectorized_type tan() const{
  vectorized_type ret;
  for(int i = 0; i < Width; i++){
    ret.set(i, std::tan(mVal[i]));
  }
  return ret;
}
inline vectorized_type pow(const vectorized_type& power) const{
  vectorized_type ret;
  for(int i = 0; i < Width; i++){
    ret.set(i,std::pow(mVal[i], power[i]));
  }
  return ret;
}

};

//basecase mathematical operators simply call std::function.
//these can be specialized to support vectorized calls.
using std::sqrt;
template<typename T>
inline auto sqrt(vectorized_type<T> val){
  return val.sqrt();
}

using std::log;
template<typename T>
inline auto log(vectorized_type<T> val){
  return val.log();
}
using std::sin;
template<typename T>
inline auto sin(vectorized_type<T> val){
  return val.sin();
}
using std::cos;
template<typename T>
inline auto cos(vectorized_type<T> val){
  return val.cos();
}
using std::tan;
template<typename T>
inline auto tan(vectorized_type<T> val){
  return val.tan();
}

template<typename T>
inline auto pow(vectorized_type<T> base, vectorized_type<T> power){
  return base.pow(power);
}


#endif