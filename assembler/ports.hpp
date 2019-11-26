

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

template<int I>
constexpr
int not0() { return I; }
template<>
constexpr
int not0<0>() { return 1; }

template<int wA, int wB=0, int wC=0, int wD=0, int wE=0, int wF=0>
union ports {
  std::bitset<wA+wB+wC+wD+wE+wF> ADDR = 0;
  struct {
    unsigned A : wA;
    unsigned B : not0<wB>();
    unsigned C : not0<wC>();
    unsigned D : not0<wD>();
    unsigned E : not0<wE>();
    unsigned F : not0<wF>();
  } PORT;
};


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

#endif // PORTS_HPP
