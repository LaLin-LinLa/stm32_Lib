/* u8g2_d_setup.c */
/* generated code, codebuild, u8g2 project */

#include "u8g2.h"
#include "system.h"

/* ssd1306 f */
void u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_fast_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}

static uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
            __NOP();
            break;
        case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
            for (uint16_t n = 0; n < 320; n++)
                __NOP();
            break;
        case U8X8_MSG_DELAY_MILLI:   // delay arg_int * 1 milli second
            delay_ms_nos(1);
            break;
        case U8X8_MSG_DELAY_I2C:     // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
            delay_us_nos(5);
            break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
        case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
//            arg_int ? GPIO_SetBits(GPIO_B, GPIO_Pin_6) : GPIO_ResetBits(GPIO_B, GPIO_Pin_6); 
						arg_int ? HAL_GPIO_WritePin(I2C1_SCL_S_GPIO_Port, I2C1_SCL_S_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(I2C1_SCL_S_GPIO_Port, I2C1_SCL_S_Pin, GPIO_PIN_RESET); 
            break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
        case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
//            arg_int ? GPIO_SetBits(GPIO_B, GPIO_Pin_7) : GPIO_ResetBits(GPIO_B, GPIO_Pin_7);  
						arg_int ? HAL_GPIO_WritePin(I2C1_SDA_S_GPIO_Port, I2C1_SDA_S_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(I2C1_SDA_S_GPIO_Port, I2C1_SDA_S_Pin, GPIO_PIN_RESET);  
            break;                    // arg_int=1: Input dir with pullup high for I2C data pin
        case U8X8_MSG_GPIO_MENU_SELECT:
            u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_NEXT:
            u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_PREV:
            u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_HOME:
            u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
            break;
        default:
            u8x8_SetGPIOResult(u8x8, 1); // default return value
            break;
    }
    return 1;
}

void u8g2_Init(u8g2_t *u8g2) {
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay);  // 初始化 u8g2 结构体
    u8g2_InitDisplay(u8g2);      // 根据所选的芯片进行初始化工作，初始化完成后，显示器处于关闭状态
    u8g2_SetPowerSave(u8g2, 0);  // 打开显示器
    u8g2_ClearBuffer(u8g2);
}

