#include "cpp_random.h"
#include <time.h>

auto rnd = std::bind(std::uniform_int_distribution<int>{0, 255}, std::default_random_engine((uint32_t)time(0)));
uint8_t random_number_cpp()
{
    //std::
    return rnd();
}