# KERISE firmware

## Class Design

### Driver

- Buzzer
  - Event-driven Task
- LED
  - Event-driven Task
- Motor
- Fan

### Sensor

- Button
  - Periodic Update
- IMU
  - Periodic Update
- Encoder
  - Periodic Update
- Reflector
  - Periodic Update
- ToF
  - Periodic Update

### Supporter

- UserInterface
  - Blocking Process
  - Button
  - IMU
  - LED
  - Buzzer
- SpeedController
  - Periodic Update
  - Odometry
  - Motor
  - IMU
  - Encoder
- WallDetector
  - Periodic Update
  - Reflector
  - ToF

### Conductor

- SearchRun
  - Task
- FastRun
  - Task
- MazeRobot
  - Task
- Logger
- ExternalController
- Machine

## Controller

1. Sampling
2. Calculation
    1. Integration
    2. Control
3. Conduction

## Main Task

- DriveTask
- PrintTask
- TimeKeepTask

## Settings

### Maze

- Restore
- Backup
- Erase Partially

### Algorithm

- The number of path

### SearchRun

- Force Search
- Force Back
- Timer

- Run Speed
- Max Speed
- Known Diag
- Wall Cut

### FastRun

- Diag or Along
- Curve Gains
- Wall Cut

```cpp
class SensorBase {
public:
  SensorBase() {}
  bool enable() {}
  bool disable() {}

private:
  void task() {
    while (1) {
      /* time sync */
      /* start sampling */
      /* mutex lock */
      /* update */
      /* mutex unlock */
      /* semaphore give */
    }
  }
};

class ActuatorBase {
public:
  ActuatorBase() {}
  enum Event {};
  bool pushEvent() {}

private:
  /* event queue */

  void task() {
    while (1) {
      /* wait for event queue */
    }
  }
};

```