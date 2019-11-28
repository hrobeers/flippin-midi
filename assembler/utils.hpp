
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

#ifndef UTILS_HPP
#define UTILS_HPP

namespace {
  constexpr
  unsigned int chash(const char* str, char delim = 0, int h = 0)
  {
    // space is first printable char
    return str[h]<' ' || str[h]==delim ? 5381 : (chash(str, delim, h+1)*33) ^ str[h];
  }

  void fatal(const char* msg) {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
  }

  size_t parse(const char* str) {
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
}

#endif // UTILS_HPP
