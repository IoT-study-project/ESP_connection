#include "mgos.h"
#include "mgos_i2c.h"
#include "mgos_mqtt.h"

#define L3G4200D_I2C_ADDR 0x69  // I2C address of L3G4200D

struct mgos_i2c *gyro_i2c;

// L3G4200D registers
#define L3G4200D_REG_CTRL_REG1 0x20
#define L3G4200D_REG_CTRL_REG4 0x23
#define L3G4200D_REG_OUT_X_L    0x28
#define L3G4200D_REG_OUT_X_H    0x29
#define L3G4200D_REG_OUT_Y_L    0x2A
#define L3G4200D_REG_OUT_Y_H    0x2B
#define L3G4200D_REG_OUT_Z_L    0x2C
#define L3G4200D_REG_OUT_Z_H    0x2D

bool init_gyroscope() {
  // Initialize the I2C interface
  gyro_i2c = mgos_i2c_get_global();
  if (gyro_i2c == NULL) {
    LOG(LL_ERROR, ("Failed to initialize I2C. Check your wiring."));
    return false;
  }

  // Configure the gyroscope
  uint8_t ctrl_reg1_data = 0x0F;  // Set data rate and enable all axes
  uint8_t ctrl_reg4_data = 0x20;  // Full-scale range selection (2000 dps)

  // Write configuration to gyroscope
  if (!mgos_i2c_write_reg_b(gyro_i2c, L3G4200D_I2C_ADDR, L3G4200D_REG_CTRL_REG1, ctrl_reg1_data)) {
    LOG(LL_ERROR, ("Failed to write to gyroscope (CTRL_REG1)."));
    return false;
  }

  if (!mgos_i2c_write_reg_b(gyro_i2c, L3G4200D_I2C_ADDR, L3G4200D_REG_CTRL_REG4, ctrl_reg4_data)) {
    LOG(LL_ERROR, ("Failed to write to gyroscope (CTRL_REG4)."));
    return false;
  }

  LOG(LL_INFO, ("Gyroscope initialized successfully."));
  return true;
}

const char* get_gyroscope_data_string(int gyro_x, int gyro_y, int gyro_z) {
    // Determine the maximum length of the formatted string
    int max_length = snprintf(NULL, 0, "{ x: %d, y: %d, z: %d }", gyro_x, gyro_y, gyro_z) + 1;

    // Allocate memory for the formatted string
    char* result = malloc(max_length);

    // Format the string
    snprintf(result, max_length, "{ x: %d, y: %d, z: %d }", gyro_x, gyro_y, gyro_z);

    return result;
}


void read_gyroscope_data() {
  // Read gyroscope data
  int16_t gyro_x = (mgos_i2c_read_reg_b(gyro_i2c, L3G4200D_I2C_ADDR, L3G4200D_REG_OUT_X_H) << 8) |
                   mgos_i2c_read_reg_b(gyro_i2c, L3G4200D_I2C_ADDR, L3G4200D_REG_OUT_X_L);
  int16_t gyro_y = (mgos_i2c_read_reg_b(gyro_i2c, L3G4200D_I2C_ADDR, L3G4200D_REG_OUT_Y_H) << 8) |
                   mgos_i2c_read_reg_b(gyro_i2c, L3G4200D_I2C_ADDR, L3G4200D_REG_OUT_Y_L);
  int16_t gyro_z = (mgos_i2c_read_reg_b(gyro_i2c, L3G4200D_I2C_ADDR, L3G4200D_REG_OUT_Z_H) << 8) |
                   mgos_i2c_read_reg_b(gyro_i2c, L3G4200D_I2C_ADDR, L3G4200D_REG_OUT_Z_L);

  LOG(LL_INFO, ("Gyroscope Data: X=%d, Y=%d, Z=%d", gyro_x, gyro_y, gyro_z));
  const char* message = get_gyroscope_data_string(gyro_x, gyro_y, gyro_z);
  
  uint16_t returned_value = mgos_mqtt_pubf("iot/0/data", 1, false, message);
  LOG(LL_INFO, ("Published: %d", returned_value));
  LOG(LL_INFO, ("Connection mqtt: %d", mgos_mqtt_global_connect()));
}


enum mgos_app_init_result mgos_app_init(void) {
  if (!init_gyroscope()) {
    return MGOS_APP_INIT_ERROR;
  }

  // Set up a timer to read gyroscope data every second
  mgos_set_timer(1000, MGOS_TIMER_REPEAT, read_gyroscope_data, NULL);

  return MGOS_APP_INIT_SUCCESS;
}
