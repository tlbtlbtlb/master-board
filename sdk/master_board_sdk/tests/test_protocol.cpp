#include "catch2/catch_all.hpp"
#include "master_board_sdk/protocol.h"
#include <limits>

TEST_CASE("FLOAT_TO_D32QN and friends work", "[protocol]") {

  // check normal values
  CHECK(FLOAT_TO_D32QN(0.0, 24) == int32_t(0x00000000));
  CHECK(FLOAT_TO_D32QN(1.0, 24) == int32_t(0x01000000));
  CHECK(FLOAT_TO_D32QN(-1.0, 24) == int32_t(0xff000000));

  // check clamping
  CHECK(FLOAT_TO_D32QN(150.0, 24) == int32_t(0x7fffffff));  
  CHECK(FLOAT_TO_D32QN(-150.0, 24) == int32_t(0x80000001));

  // check that crazy values don't overflow
  CHECK(FLOAT_TO_D32QN(-1.0e99, 24) == int32_t(0x80000001));
  CHECK(FLOAT_TO_D32QN(1.0e99, 24) == int32_t(0x7fffffff));  
  CHECK(FLOAT_TO_D32QN(std::numeric_limits<double>::infinity(), 24) == 
        int32_t(0x7fffffff));  
  CHECK(FLOAT_TO_D32QN(-std::numeric_limits<double>::infinity(), 24) ==
        int32_t(0x80000001));

#if 0
  // Note: I'm not sure this behavior is guaranteed by every floating point system.
  // 0 is a relatively benign outcome for sending a NaN, but it might be better to check for NaN
  // and throw an exception in, say, Motor::SetPositionReference
  CHECK(FLOAT_TO_D32QN(std::numeric_limits<double>::quiet_NaN(), 24) ==
        int32_t(0));
#endif


  CHECK(FLOAT_TO_D16QN(1.0, 10) == int16_t(0x0400));
  CHECK(FLOAT_TO_D16QN(-1.0, 10) == int16_t(0xfc00));
  CHECK(FLOAT_TO_D16QN(150.0, 10) == int16_t(0x7fff));  
  CHECK(FLOAT_TO_D16QN(-150.0, 10) == int16_t(0x8001));
  CHECK(FLOAT_TO_D16QN(1.0e99, 10) == int16_t(0x7fff));  
  CHECK(FLOAT_TO_D16QN(-1.0e99, 10) == int16_t(0x8001));

  CHECK(FLOAT_TO_uD16QN(1.0, 10) == uint16_t(0x0400));
  CHECK(FLOAT_TO_uD16QN(-1.0, 10) == uint16_t(0x0000));
  CHECK(FLOAT_TO_uD16QN(150.0, 10) == uint16_t(0xffff));  
  CHECK(FLOAT_TO_uD16QN(-150.0, 10) == uint16_t(0x0000));  
  CHECK(FLOAT_TO_uD16QN(1.0e99, 10) == uint16_t(0xffff));  
  CHECK(FLOAT_TO_uD16QN(-1.0e99, 10) == uint16_t(0x0000));  

  CHECK(FLOAT_TO_uD8QN(1.0, 3) == uint8_t(0x08));
  CHECK(FLOAT_TO_uD8QN(-1.0, 10) == uint8_t(0x00));
  CHECK(FLOAT_TO_uD8QN(150.0, 10) == uint8_t(0xff));  
  CHECK(FLOAT_TO_uD8QN(-150.0, 10) == uint8_t(0x00));  
  CHECK(FLOAT_TO_uD8QN(1.0e99, 10) == uint8_t(0xff));  
  CHECK(FLOAT_TO_uD8QN(-1.0e99, 10) == uint8_t(0x00));  
}
