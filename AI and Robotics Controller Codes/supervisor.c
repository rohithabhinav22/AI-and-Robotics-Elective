#include <webots/supervisor.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define TIME_STEP 32
#define WHITE_LINE_X -125.0
#define TOLERANCE 0.5

const char *car1_name = "Rohith Car";
const char *car2_name = "Radha Krishna Car";

int main() {
  wb_robot_init();

  // Get each car node by DEF name
  WbNodeRef car1 = wb_supervisor_node_get_from_def(car1_name);
  WbNodeRef car2 = wb_supervisor_node_get_from_def(car2_name);

  bool car1_started = false;
  double car1_start_time = 0, car1_finish_time = 0;
  bool car1_finished = false, car1_crossed_once = false;

  bool car2_started = false;
  double car2_start_time = 0, car2_finish_time = 0;
  bool car2_finished = false, car2_crossed_once = false;

  while (wb_robot_step(TIME_STEP) != -1) {
    car1 = wb_supervisor_node_get_from_def(car1_name);
    car2 = wb_supervisor_node_get_from_def(car2_name);

    if (car1 == NULL || car2 == NULL) {
      printf("One or both car DEF names are incorrect or missing!\n");
      break;
    }

    // Get each car's position (World Coordinates)
    const double *car1_pos = wb_supervisor_node_get_position(car1);
    const double *car2_pos = wb_supervisor_node_get_position(car2);

    // Car 1 logic
    if (!car1_started && fabs(car1_pos[0] - WHITE_LINE_X) < TOLERANCE && !car1_crossed_once) {
      car1_started = true;
      car1_start_time = wb_robot_get_time();
      car1_crossed_once = true;
      printf("Rohith Car started timer!\n");
    } else if (car1_started && !car1_finished && fabs(car1_pos[0] - WHITE_LINE_X) < TOLERANCE && car1_crossed_once) {
      car1_finished = true;
      car1_finish_time = wb_robot_get_time();
      printf("Rohith Car finished! Time: %.2fs\n", car1_finish_time - car1_start_time);
    }

    // Car 2 logic
    if (!car2_started && fabs(car2_pos[0] - WHITE_LINE_X) < TOLERANCE && !car2_crossed_once) {
      car2_started = true;
      car2_start_time = wb_robot_get_time();
      car2_crossed_once = true;
      printf("Radha Krishna Car started timer!\n");
    } else if (car2_started && !car2_finished && fabs(car2_pos[0] - WHITE_LINE_X) < TOLERANCE && car2_crossed_once) {
      car2_finished = true;
      car2_finish_time = wb_robot_get_time();
      printf("Radha Krishna Car finished! Time: %.2fs\n", car2_finish_time - car2_start_time);
    }

    // If both finished, declare winner and break
    if (car1_finished && car2_finished) {
      printf("\nRace Results:\n");
      printf("Rohith Car: %.2fs\n", car1_finish_time - car1_start_time);
      printf("Radha Krishna Car: %.2fs\n", car2_finish_time - car2_start_time);
      const char *winner = (car1_finish_time - car1_start_time) < (car2_finish_time - car2_start_time) ? car1_name : car2_name;
      printf("Winner: %s!\n", winner);
      break;
    }
  }

  wb_robot_cleanup();
  return 0;
}