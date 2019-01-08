//
// Created by pilarski on 8-1-19.
//

#ifndef MILLI_LIBRARY_REPEAT_HPP
#define MILLI_LIBRARY_REPEAT_HPP

template <typename T>
auto repeat(unsigned long long times, T callback) -> void {
  while(times--)
    callback();
}

#endif //MILLI_LIBRARY_REPEAT_HPP
