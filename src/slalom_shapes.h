#pragma once

#include "slalom.h"

namespace ctrl {

enum ShapeIndex {
  S90,
  F45,
  F90,
  F135,
  F180,
  FV90,
  FK90,
  FS90,
  ShapeIndexMax,
};

static const std::array<ctrl::slalom::Shape, ShapeIndexMax> shapes = {{
/* SS_S90  T:0.287526*/
ctrl::slalom::Shape(ctrl::Pose(      45,       45,    1.5708), ctrl::Pose(      44,       44,    1.5708),  1.00004, 0.999981,  265.749, 3769.91, 113.097, 9.42478),
/* SS_F45  T:0.257131*/
ctrl::slalom::Shape(ctrl::Pose(      90,       45,  0.785398), ctrl::Pose( 72.4265,       30,  0.785398),  2.57355,  21.2132,  411.637, 3769.91, 113.097, 9.42478),
/* SS_F90  T:0.374612*/
ctrl::slalom::Shape(ctrl::Pose(      90,       90,    1.5708), ctrl::Pose( 69.9999,       70,    1.5708),  20.0001,       20,  422.782, 3769.91, 113.097, 9.42478),
/* SS_F135 T:0.465157*/
ctrl::slalom::Shape(ctrl::Pose(      45,       90,   2.35619), ctrl::Pose( 33.1366,       80,   2.35619),  21.8634,  14.1421,  353.607, 3769.91, 113.097, 9.42478),
/* SS_F180 T:0.563108*/
ctrl::slalom::Shape(ctrl::Pose(       0,       90,   3.14159), ctrl::Pose(       0,       90,   3.14159),       24,       24,  412.225, 3769.91, 113.097, 9.42478),
/* SS_FV90 T:0.387894*/
ctrl::slalom::Shape(ctrl::Pose( 63.6396,  63.6396,    1.5708), ctrl::Pose( 47.9999,       48,    1.5708),  15.6397,  15.6396,  289.908, 3769.91, 113.097, 9.42478),
/* SS_FK90 T:0.286038*/
ctrl::slalom::Shape(ctrl::Pose( 127.279,  127.279,    1.5708), ctrl::Pose(     125,      125,    1.5708),   2.2794,  2.27923,  754.969, 3769.91, 113.097, 9.42478),
/* SS_FS90 T:0.287526*/
ctrl::slalom::Shape(ctrl::Pose(      45,       45,    1.5708), ctrl::Pose(      44,       44,    1.5708),  1.00004, 0.999981,  265.749, 3769.91, 113.097, 9.42478),
}};

} // namespace ctrl