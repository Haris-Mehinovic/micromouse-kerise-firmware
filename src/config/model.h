/**
 * @file model.h
 * @author Ryotaro Onuki (kerikun11+github@gmail.com)
 * @brief マイクロマウスのモデル
 * @date 2020-04-20
 */
#pragma once

#include "feedback_controller.h"
#include "trajectory_tracker.h"

#ifndef M_PI
static constexpr float M_PI = 3.14159265358979323846f;
#endif

namespace field {

/* Field Size Parameter */
static constexpr float SegWidthFull = 90.0f;
static constexpr float SegWidthDiag = 127.2792206135786f;
static constexpr float WallThickness = 6.0f;

}; // namespace field

namespace model {

#define KERISE_SELECT 5

#if KERISE_SELECT == 5
/* KERISE v5 */
/* Machine Size Parameter */
static constexpr float RotationRadius = 29.0f / 2;
static constexpr float GearRatio = 1.0f;
static constexpr float WheelDiameter = 12.60f;
static constexpr float CenterShift = 0.0f;
static constexpr float TailLength = 13.0f + CenterShift;
/* ToF */
static constexpr float tof_dist_offset = 12;
/* Reflector */
const float wall_attach_gain_Kp = 12.0f;
const float wall_attach_gain_Ki = 0.05f;
const float wall_attach_end = 0.01f;
const float wall_avoid_gain = 0.001f;
/* Model */
static constexpr struct ctrl::FeedbackController<ctrl::Polar>::Model
    SpeedControllerModel = {
  .K1 = ctrl::Polar(5463, 137), .T1 = ctrl::Polar(0.1998 / 1.6, 0.1354),
};
static constexpr struct ctrl::FeedbackController<ctrl::Polar>::Gain
    SpeedControllerGain = {
  .Kp = ctrl::Polar(0.0008, 0.06), .Ki = ctrl::Polar(0.02, 9.0),
  .Kd = ctrl::Polar(0, 0),
};
/* Estimated Velocity IIR Filter gain */
static constexpr struct ctrl::Polar alpha = ctrl::Polar(0.9f, 0.0f);
/* Trajectory Tracking Gain */
static constexpr struct ctrl::TrajectoryTracker::Gain TrajectoryTrackerGain = {
    .zeta = 1.0f,
    .omega_n = 1.0f,
    .low_zeta = 0.5f,
    .low_b = 0.001f,
};

#elif KERISE_SELECT == 4
/* Original KERISE v4 */
/* Machine Size Parameter */
static constexpr float RotationRadius = 15.0f;
static constexpr float GearRatio = (12.0f / 38.0f);
static constexpr float WheelDiameter = 12.67f;
static constexpr float CenterShift = 6.0f;
static constexpr float TailLength = 16.4f;
/* ToF */
static constexpr float tof_dist_offset = 21; //< 大きいほど壁に近く
/* Reflector */
const float wall_attach_gain_Kp = 240.0f;
const float wall_attach_gain_Ki = 1.0f;
const float wall_attach_end = 0.1f;
const float wall_avoid_gain = 0.003f;
/* Model */
static constexpr struct ctrl::FeedbackController<ctrl::Polar>::Model
    SpeedControllerModel = {
  // .K1 = ctrl::Polar(5789, 49.74f), .T1 = ctrl::Polar(0.12f, 0.08f), /*< 東 */
  // .K1 = ctrl::Polar(5789, 500.0f), .T1 = ctrl::Polar(0.12f, 0.48f), /*< 守 */
  .K1 = ctrl::Polar(5789, 1000.0f), .T1 = ctrl::Polar(0.12f, 0.48), /*< 4 */
};
static constexpr struct ctrl::FeedbackController<ctrl::Polar>::Gain
    SpeedControllerGain = {
  // .Kp = ctrl::Polar(0.0006f, 0.1f), .Ki = ctrl::Polar(0.1f, 3.0f), /*< 東 */
  // .Kp = ctrl::Polar(0.0012f, 0.12f), .Ki = ctrl::Polar(0.3f, 6.0f), /*< 攻 */
  // .Kp = ctrl::Polar(0.0006f, 0.08f), .Ki = ctrl::Polar(0.1f, 3.0f), /*< 守 */
  .Kp = ctrl::Polar(0.0008f, 0.15f), .Ki = ctrl::Polar(0.1f, 6.0f), /*< 4 */
      .Kd = ctrl::Polar(0, 0),
};
/* Estimated Velocity IIR Filter gain */
static constexpr ctrl::Polar alpha = ctrl::Polar(0.8f, 0.0f);
/* Trajectory Tracking Gain */
static constexpr struct ctrl::TrajectoryTracker::Gain TrajectoryTrackerGain = {
    .zeta = 0.8f,
    .omega_n = 18.0f,
    .low_zeta = 1.0f,
    .low_b = 0.001f,
};

#elif KERISE_SELECT == 3
/* Copy KERISE v4 */
/* Machine Size Parameter */
static constexpr float RotationRadius = 15.0f;
static constexpr float GearRatio = (12.0f / 38.0f);
static constexpr float WheelDiameter = 12.98f;
static constexpr float CenterShift = 6.0f;
static constexpr float TailLength = 16.4f;
/* ToF */
static constexpr float tof_dist_offset = 18; //< 大きいほど壁に近く
/* Reflector */
const float wall_attach_gain_Kp = 240.0f;
const float wall_attach_gain_Ki = 1.0f;
const float wall_attach_end = 0.1f;
const float wall_avoid_gain = 0.003f;
/* Model */
static constexpr struct ctrl::FeedbackController<ctrl::Polar>::Model
    SpeedControllerModel = {
  // .K1 = ctrl::Polar(5789, 500.0f), .T1 = ctrl::Polar(0.12f, 0.48f), /*< 守 */
  // .K1 = ctrl::Polar(5789, 49.74f), .T1 = ctrl::Polar(0.12f, 0.08f), /*< 東 */
  .K1 = ctrl::Polar(5789, 1000.0f), .T1 = ctrl::Polar(0.12f, 0.48), /*< 4 */
};
static constexpr struct ctrl::FeedbackController<ctrl::Polar>::Gain
    SpeedControllerGain = {
  // .Kp = ctrl::Polar(0.0012f, 0.12f), .Ki = ctrl::Polar(0.3f, 6.0f), /*< 攻 */
  // .Kp = ctrl::Polar(0.0006f, 0.08f), .Ki = ctrl::Polar(0.1f, 3.0f), /*< 守 */
  // .Kp = ctrl::Polar(0.0008f, 0.15f), .Ki = ctrl::Polar(0.1f, 6.0f), /*< 4 */
  .Kp = ctrl::Polar(0.0006f, 0.1f), .Ki = ctrl::Polar(0.1f, 3.0f), /*< 3 */
      .Kd = ctrl::Polar(0, 0),
};
/* Estimated Velocity IIR Filter gain */
static constexpr ctrl::Polar alpha = ctrl::Polar(0.8f, 0.0f);
/* Trajectory Tracking Gain */
static constexpr struct ctrl::TrajectoryTracker::Gain TrajectoryTrackerGain = {
    .zeta = 0.6f,
    .omega_n = 18.0f,
    .low_zeta = 1.0f,
    .low_b = 0.001f,
};

#endif

}; // namespace model
