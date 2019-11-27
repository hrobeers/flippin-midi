

// Copyright (C) 2019 hrobeers (https://github.com/hrobeers)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef PORTS_HPP
#define PORTS_HPP

#include <bitset>


// Compile time calculation of trailing zeros in binary
template <uint64_t I>
constexpr
uint8_t LSHIFT_CNT() {
  return LSHIFT_CNT<I<<1>()-1;
}
template <>
constexpr
uint8_t LSHIFT_CNT<0>() {
  return 64;
}
template<uint64_t I>
constexpr
uint8_t USHIFT_CNT() {
  return USHIFT_CNT<(I>>1)>()+1;
}
template <>
constexpr
uint8_t USHIFT_CNT<0>() {
  return 0;
}
template<uint64_t I>
constexpr
uint8_t WIDTH() {
  return USHIFT_CNT<I>()-LSHIFT_CNT<I>();
}
template <>
constexpr
uint8_t WIDTH<0>() {
  return 1;
}

template<uint64_t I, typename T = int>
constexpr
T SHIFT() { return I >> LSHIFT_CNT<I>(); }

template<int I>
constexpr
int not0() { return I; }
template<>
constexpr
int not0<0>() { return 1; }

template<int P>
constexpr int pow(int base) { return pow<P-1>(base)*base; }
template<>
constexpr int pow<0>(int base) { return 1; }

template<int wA, int wB, int wC, int wD, int wE, int wF>
struct limits_t {
  constexpr static unsigned A = pow<wA>(2)-1;
  constexpr static unsigned B = pow<wB>(2)-1;
  constexpr static unsigned C = pow<wC>(2)-1;
  constexpr static unsigned D = pow<wD>(2)-1;
  constexpr static unsigned E = pow<wE>(2)-1;
  constexpr static unsigned F = pow<wF>(2)-1;
};

template<typename L, int wA, int wB, int wC, int wD, int wE, int wF>
struct masks_t {
  constexpr static unsigned A = L::A;
  constexpr static unsigned B = L::B << USHIFT_CNT<A>();
  constexpr static unsigned C = L::C << USHIFT_CNT<B>();
  constexpr static unsigned D = L::D << USHIFT_CNT<C>();
  constexpr static unsigned E = L::E << USHIFT_CNT<D>();
  constexpr static unsigned F = L::F << USHIFT_CNT<E>();
};

template<int wA, int wB=0, int wC=0, int wD=0, int wE=0, int wF=0>
union ports_t {
  std::bitset<wA+wB+wC+wD+wE+wF> addr = 0;
  struct ports {
    unsigned A : wA;
    unsigned B : not0<wB>();
    unsigned C : not0<wC>();
    unsigned D : not0<wD>();
    unsigned E : not0<wE>();
    unsigned F : not0<wF>();
  } port;

  typedef limits_t<wA,wB,wC,wD,wE,wF> limits;
  typedef masks_t<limits,wA,wB,wC,wD,wE,wF> masks;

  const limits limit;
  const masks mask;
};

#define PORT(name,port) \
  template<typename T, typename Tval> \
  void name(T& d, const Tval& val) { d.port = val; } \
  template<typename T> \
  unsigned name(const T& d) { return d.port; }

#endif // PORTS_HPP
