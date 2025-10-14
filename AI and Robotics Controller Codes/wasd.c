#include <webots/robot.h>
#include <webots/keyboard.h>
#include <webots/lidar.h>
#include <webots/vehicle/driver.h>
#include <webots/gps.h>
#include <webots/display.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define TIME_STEP 50
#define UNKNOWN 99999.99

// Manual driving state
double speed = 0.0;
double steering_angle = 0.0;

// LIDAR
WbDeviceTag sick;
int sick_width;
double sick_fov;

// GPS
WbDeviceTag gps;
double gps_coords[3] = {0.0, 0.0, 0.0};
double gps_speed = 0.0;

// Display
WbDeviceTag display;
WbImageRef speedometer_image = NULL;

void update_display() {
  const double NEEDLE_LENGTH = 50.0;
  wb_display_image_paste(display, speedometer_image, 0, 0, false);
  double current_speed = wbu_driver_get_current_speed();
  if (isnan(current_speed)) current_speed = 0.0;
  double alpha = current_speed / 260.0 * 3.72 - 0.27;
  int x = -NEEDLE_LENGTH * cos(alpha);
  int y = -NEEDLE_LENGTH * sin(alpha);
  wb_display_draw_line(display, 100, 95, 100 + x, 95 + y);
  char txt[64];
  sprintf(txt, "GPS coords: %.1f %.1f", gps_coords[0], gps_coords[2]);
  wb_display_draw_text(display, txt, 10, 130);
  sprintf(txt, "GPS speed:  %.1f", gps_speed);
  wb_display_draw_text(display, txt, 10, 140);
}

void compute_gps_speed() {
  const double *coords = wb_gps_get_values(gps);
  const double speed_ms = wb_gps_get_speed(gps);
  gps_speed = speed_ms * 3.6;
  memcpy(gps_coords, coords, sizeof(gps_coords));
}

double process_sick_data(const float *sick_data, double *obstacle_dist) {
  const int HALF_AREA = 20;
  int sumx = 0, collision_count = 0;
  *obstacle_dist = 0.0;
  for (int x = sick_width / 2 - HALF_AREA; x < sick_width / 2 + HALF_AREA; x++) {
    float range = sick_data[x];
    if (range < 20.0) {
      sumx += x;
      collision_count++;
      *obstacle_dist += range;
    }
  }
  if (collision_count == 0) return UNKNOWN;
  *obstacle_dist = *obstacle_dist / collision_count;
  return ((double)sumx / collision_count / sick_width - 0.5) * sick_fov;
}

void check_keyboard() {
  int key = wb_keyboard_get_key();
  if (key == 'W') {
    speed += 1.5;
    if (speed > 250.0) speed = 250.0;
  } else if (key == 'S') {
    speed -= 1.5;
    if (speed < 0.0) speed = 0.0;
  } else if (key == 'A') {
    steering_angle -= 0.08;
    if (steering_angle < -0.5) steering_angle = -0.5;
  } else if (key == 'D') {
    steering_angle += 0.08;
    if (steering_angle > 0.5) steering_angle = 0.5;
  } else {
    steering_angle = 0.0;
  }
}


int main() {
  wb_robot_init();
  sick = wb_robot_get_device("Sick LMS 291");
  wb_lidar_enable(sick, TIME_STEP);
  sick_width = wb_lidar_get_horizontal_resolution(sick);
  sick_fov = wb_lidar_get_fov(sick);

  gps = wb_robot_get_device("gps");
  wb_gps_enable(gps, TIME_STEP);

  display = wb_robot_get_device("display");
  speedometer_image = wb_display_image_load(display, "speedometer.png");

  wb_keyboard_enable(TIME_STEP);

  while (wb_robot_step(TIME_STEP) != -1) {
    check_keyboard();

    // Obstacle avoidance
    const float *sick_data = wb_lidar_get_range_image(sick);
    double obstacle_dist, obstacle_angle;
    obstacle_angle = process_sick_data(sick_data, &obstacle_dist);

    // If obstacle ahead, brake and try to steer
    if (obstacle_angle != UNKNOWN) {
      wbu_driver_set_brake_intensity(0.6);
      if (obstacle_angle > 0) steering_angle += 0.2;
      else steering_angle -= 0.2;
    } else {
      wbu_driver_set_brake_intensity(0.0);
    }

    // Set speed and steering
    wbu_driver_set_cruising_speed(speed);
    wbu_driver_set_steering_angle(steering_angle);

    compute_gps_speed();
    update_display();
  }

  wb_robot_cleanup();
  return 0;
}