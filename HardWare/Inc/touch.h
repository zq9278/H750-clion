
#ifndef __FT5206_H
#define __FT5206_H

#include "main.h" // 确保这里引入的是您项目的HAL库主头文件

// 定义FT5206 I2C通信使用的GPIO端口和引脚
//#define FT5206_I2C_SCL_GPIO_Port     GPIOB // 示例SCL端口
//#define FT5206_I2C_SCL_Pin           GPIO_PIN_10 // 示例SCL引脚
//#define FT5206_I2C_SDA_GPIO_Port     GPIOB // 示例SDA端口
//#define FT5206_I2C_SDA_Pin           GPIO_PIN_11 // 示例SDA引脚
#define SDA_IN()  {GPIOD->MODER&=~(GPIO_MODER_MODE0<<(4*2));GPIOD->MODER|=0<<4*2;} // PD4输入模式
#define SDA_OUT() {GPIOD->MODER&=~(GPIO_MODER_MODE0<<(4*2));GPIOD->MODER|=1<<4*2;} // PD4输出模式

//#define SDA_IN()  {GPIOD->MODER &= ~(3UL << (4 * 2)); GPIOD->MODER |= (0 << (4 * 2));} // PD4输入模式
//#define SDA_OUT() {GPIOD->MODER &= ~(3UL << (4 * 2)); GPIOD->MODER |= (1 << (4 * 2));} // PD4输出模式
#define FT_RST(n)  (n?HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET))//FT5206复位引脚


// FT5206 I2C地址及命令
#define FT_CMD_WR 0x70 // 写地址
#define FT_CMD_RD 0x71 // 读地址

// FT5206 常用寄存器定义
#define FT_DEVIDE_MODE 0x00
#define FT_REG_NUM_FINGER 0x02
#define FT_TP1_REG 				0X03	  	//第一个触摸点数据地址
#define FT_TP2_REG 				0X09		//第二个触摸点数据地址
#define FT_TP3_REG 				0X0F		//第三个触摸点数据地址
#define FT_TP4_REG 				0X15		//第四个触摸点数据地址
#define FT_TP5_REG 				0X1B		//第五个触摸点数据地址
#define FT_ID_G_THGROUP 0x80
#define FT_ID_G_MODE 0xA4
#define FT_ID_G_LIB_VERSION 0xA1
#define FT_ID_G_PERIODACTIVE 0x88

#define CT_MAX_TOUCH  5    //电容屏支持的点数,固定为5点

#define TP_PRES_DOWN 0x80  //触屏被按下
#define TP_CATH_PRES 0x40  //有按键按下了


typedef struct
{
    uint16_t x;
    uint16_t y;
}tp_dev;
//触摸屏结构体


// 函数声明
uint8_t FT5206_Init_SoftI2C(void);
uint8_t FT5206_WR_Reg_SoftI2C(uint16_t reg, uint8_t *buf, uint8_t len);
void FT5206_RD_Reg_SoftI2C(uint16_t reg, uint8_t *buf, uint8_t len);
uint8_t FT5206_Scan_SoftI2C(uint8_t mode);
void FT5206_INT_SoftI2C(void );

        void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_Wait_Ack(void);
void I2C_Send_Byte(uint8_t byte);

uint8_t I2C_Read_Byte(uint8_t ack);
void I2C_Ack(void);
void I2C_NAck(void);

#endif
