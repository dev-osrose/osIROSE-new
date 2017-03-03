#pragma once

struct Destination {
  Destination(float x, float y, uint16_t dist) : x_( x ),
                                                 y_( y ),
                                                 dist_( dist ) {}

  float x_;
  float y_;
  uint16_t dist_;
};
