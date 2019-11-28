#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>

#include "utils.hpp"
#include "ports.hpp"


// Port widths
const int WORD_WIDTH  = 3;
const int SIGN_WIDTH  = 3;
const int TRIG_WIDTH  = 2;
const int SCALE_WIDTH = 4;
const int MIDI_WIDTH  = 4;
const int SCLCK_WIDTH = 1;

// Input ports
PORT(WORD,A)
PORT(SIGN,B)
PORT(TRIG,C)
PORT(SCALE,D)

typedef ports_t<WORD_WIDTH, SIGN_WIDTH, TRIG_WIDTH, SCALE_WIDTH> IN;

// Output ports
PORT(MIDI,A)
//PORT(SIGN,B) already declared
PORT(SCLCK,C)

typedef ports_t<MIDI_WIDTH, SIGN_WIDTH, SCLCK_WIDTH> OUT;

// TIGGERS
const unsigned HI = 0b01;
const unsigned LO = 0b10;

int main(int argc, char* argv[]) {
  // TODO parse rom size
  size_t rom_size = pow<14>(2);
  std::vector<OUT> ROM(rom_size);
  std::string word;
  size_t value = 0;

  IN input;

  while (std::cin >> word) {
    switch (chash(word.c_str())) {

    case chash("scale"):
      std::cin >> word;
      value = parse(word.c_str());
      if (value > SCALE(input.limit))
        fatal("invalid scale argument");
      SCALE(input.port, value);
      break;

    case chash("sign"):
      std::cin >> word;
      value = parse(word.c_str());
      if (value > SIGN(input.limit))
        fatal("invalid sign argument");
      SIGN(input.port, value);
      break;

    case chash("hi"):
      TRIG(input.port, HI);
      break;

    case chash("lo"):
      TRIG(input.port, LO);
      break;

    case chash("noteon"):
      SIGN(ROM[input.addr.to_ulong()].port, SIGN(input.port));
      std::cout << input.addr << " -> "<< ROM[input.addr.to_ulong()].addr << std::endl;
      break;

    case chash("noteoff"):
      SIGN(ROM[input.addr.to_ulong()].port, SIGN(input.port));
      std::cout << input.addr << " -> "<< ROM[input.addr.to_ulong()].addr << std::endl;
      break;

    default:
      //std::cout << word << std::endl;
      break;
    }
  }

  return EXIT_SUCCESS;
}
