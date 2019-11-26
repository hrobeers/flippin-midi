#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>

#include "chash.hpp"
#include "ports.hpp"

void fatal(const char* msg) {
  std::cerr << msg << std::endl;
  exit(EXIT_FAILURE);
}

uint_fast64_t parse(const char* str) {
  char *end;
  switch (str[0]) {
  case '$':
    return std::strtol(&str[1], &end, 16);
  case 'b':
    return std::strtol(&str[1], &end, 2);
  default:
    return std::strtol(str, &end, 10);
  }
}


// Input ports
#define WORD_PORT_IN  0b000000000111
#define SIGN_PORT_IN  0b000000111000
#define TRIG_PORT_IN  0b000011000000
#define SCALE_PORT_IN 0b111100000000
#define WORD(PORTS)  PORTS.PORT.A
#define SIGN(PORTS)  PORTS.PORT.B
#define TRIG(PORTS)  PORTS.PORT.C
#define SCALE(PORTS) PORTS.PORT.D

typedef ports<
  WIDTH<WORD_PORT_IN>(),
  WIDTH<SIGN_PORT_IN>(),
  WIDTH<TRIG_PORT_IN>(),
  WIDTH<SCALE_PORT_IN>()> IN;

// Output ports
#define MIDI_PORT_OUT  0b00001111
#define SIGN_PORT_OUT  0b01110000
#define SCLCK_PORT_OUT 0b10000000
#define MIDI(PORTS)  PORTS.PORT.A
//#define SIGN(PORTS)  PORTS.PORT.B
#define SCLCK(PORTS)  PORTS.PORT.C

typedef ports<
  WIDTH<MIDI_PORT_OUT>(),
  WIDTH<SIGN_PORT_OUT>(),
  WIDTH<SCLCK_PORT_OUT>()> OUT;

// TIGGERS
#define HI 0b01
#define LO 0b10

int main(int argc, char* argv[]) {
  // TODO parse rom size
  size_t rom_size = std::pow(2,16);
  std::vector<OUT> ROM(rom_size);
  std::string word;

  IN input;

  while (std::cin >> word) {
    switch (chash(word.c_str())) {

    case chash("scale"):
      std::cin >> word;
      SCALE(input) = parse(word.c_str());
      if (SCALE(input) & ~SHIFT<SCALE_PORT_IN>())
        fatal("invalid scale argument");
      break;

    case chash("sign"):
      std::cin >> word;
      SIGN(input) = parse(word.c_str());
      if (SIGN(input) & ~SHIFT<SIGN_PORT_IN>())
        fatal("invalid sign argument");
      break;

    case chash("hi"):
      TRIG(input) = HI;
      break;

    case chash("lo"):
      TRIG(input) = LO;
      break;

    case chash("noteon"):
      std::cout << input.ADDR << std::endl;
      SIGN(ROM[input.ADDR.to_ulong()]) = SIGN(input);
      std::cout << ROM[input.ADDR.to_ulong()].ADDR << std::endl;
      break;

    case chash("noteoff"):
      std::cout << input.ADDR << std::endl;
      SIGN(ROM[input.ADDR.to_ulong()]) = SIGN(input);
      std::cout << ROM[input.ADDR.to_ulong()].ADDR << std::endl;
      break;

    default:
      //std::cout << word << std::endl;
      break;
    }
  }

  return EXIT_SUCCESS;
}
