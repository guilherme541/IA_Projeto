#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/supervisor.h>

#define TIME_STEP 64
#define QtddCaixa 20
#define LIMIAR_MOVIMENTO 0.0001

int main(int argc, char **argv) {
  wb_robot_init();

  // Motores
  WbDeviceTag MotorEsquerdo = wb_robot_get_device("left wheel motor");
  WbDeviceTag MotorDireito = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(MotorEsquerdo, INFINITY);
  wb_motor_set_position(MotorDireito, INFINITY);

  // Sensores de proximidade
  WbDeviceTag SensorProx[8];
  char nomeSensor[8];
  for (int i = 0; i < 8; i++) {
    sprintf(nomeSensor, "ps%d", i);
    SensorProx[i] = wb_robot_get_device(nomeSensor);
    wb_distance_sensor_enable(SensorProx[i], TIME_STEP);
  }

  WbNodeRef caixas[QtddCaixa];
  char nomeCaixa[20];
  for (int c = 0; c < QtddCaixa; c++) {
    sprintf(nomeCaixa, "CAIXA%d", c + 1);
    caixas[c] = wb_supervisor_node_get_from_def(nomeCaixa);
  }

  bool encontrouCaixaLeve = false;
  double posicaoAnterior[QtddCaixa][3] = {0};


  for (int c = 0; c < QtddCaixa; c++) {
    if (caixas[c] != NULL) {
      const double *pos = wb_supervisor_node_get_position(caixas[c]);
      posicaoAnterior[c][0] = pos[0];
      posicaoAnterior[c][1] = pos[1];
      posicaoAnterior[c][2] = pos[2];
    }
  }

  while (wb_robot_step(TIME_STEP) != -1) {
    double prox_frente = wb_distance_sensor_get_value(SensorProx[0]) + 
                        wb_distance_sensor_get_value(SensorProx[7]);
    double prox_direita = wb_distance_sensor_get_value(SensorProx[1]) + 
                         wb_distance_sensor_get_value(SensorProx[2]);
    double prox_esquerda = wb_distance_sensor_get_value(SensorProx[5]) + 
                          wb_distance_sensor_get_value(SensorProx[6]);

    double vel_esq = 2.0, vel_dir = 2.0;

    if (!encontrouCaixaLeve) {
      if (prox_frente > 80) {
        if (prox_esquerda > prox_direita) {
          vel_esq = 2.0;
          vel_dir = -1.0;
        } else {
          vel_esq = -1.0;
          vel_dir = 2.0;
        }
      } else if (prox_direita > 80) {
        vel_esq = 2.0;
        vel_dir = 0.5;
      } else if (prox_esquerda > 80) {
        vel_esq = 0.5;
        vel_dir = 2.0;
      }

      for (int c = 0; c < QtddCaixa; c++) {
        if (caixas[c] != NULL) {
          const double *pos_atual = wb_supervisor_node_get_position(caixas[c]);
          double deltaX = fabs(pos_atual[0] - posicaoAnterior[c][0]);
          double deltaY = fabs(pos_atual[1] - posicaoAnterior[c][1]);
          double deltaZ = fabs(pos_atual[2] - posicaoAnterior[c][2]);
          // Print para depuração
          printf("Caixa %d: DeltaX = %.6f, DeltaY = %.6f, DeltaZ = %.6f\n", c + 1, deltaX, deltaY, deltaZ);

          if (deltaX > LIMIAR_MOVIMENTO || deltaY > LIMIAR_MOVIMENTO || deltaZ > LIMIAR_MOVIMENTO) {
            encontrouCaixaLeve = true;
            printf("Encontrou caixa leve! (CAIXA%d)\n", c + 1);
            break;
          }
        }
      }
    } else {
      vel_esq = 2.0;
      vel_dir = -2.0;
    }

    wb_motor_set_velocity(MotorEsquerdo, vel_esq);
    wb_motor_set_velocity(MotorDireito, vel_dir);

    for (int c = 0; c < QtddCaixa; c++) {
      if (caixas[c] != NULL) {
        const double *pos = wb_supervisor_node_get_position(caixas[c]);
        posicaoAnterior[c][0] = pos[0];
        posicaoAnterior[c][1] = pos[1];
        posicaoAnterior[c][2] = pos[2];
      }
    }
  }

  wb_robot_cleanup();
  return 0;
}