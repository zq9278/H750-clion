//
// Created by zhangqi on 2024/3/22.
//

#include "touch.h"

// 定义I2C使用的GPIO端口和引脚
#define I2C_SCL_GPIO_Port GPIOD  // 示例SCL连接到GPIOB
#define I2C_SCL_Pin GPIO_PIN_5   // 示例SCL连接到Pin 6
#define I2C_SDA_GPIO_Port GPIOD  // 示例SDA连接到GPIOB
#define I2C_SDA_Pin GPIO_PIN_4   // 示例SDA连接到Pin 7

// SCL和SDA的高低电平控制
#define I2C_SCL_H() HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET)
#define I2C_SCL_L() HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET)
#define I2C_SDA_H() HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET)
#define I2C_SDA_L() HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET)

// 读取SDA线状态
#define I2C_SDA_READ() HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin)

const uint16_t FT5206_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};

_m_tp_dev tp_dev=
        {
                FT5206_Init_SoftI2C,
                FT5206_Scan_SoftI2C,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
        };

// I2C起始信号
void I2C_Start(void) {
    // SDA线产生一个下降沿表示开始信号
    SDA_OUT();
    I2C_SDA_H();
    I2C_SCL_H();
    HAL_Delay(1);  // 延时，确保信号稳定
    I2C_SDA_L();
    HAL_Delay(1);
    I2C_SCL_L();
}

// I2C停止信号
void I2C_Stop(void) {
    // SDA线产生一个上升沿表示停止信号
    SDA_OUT();
    I2C_SCL_L();
    I2C_SDA_L();
    HAL_Delay(1);
    I2C_SCL_H();
    I2C_SDA_H();
    HAL_Delay(1);
}

// I2C等待应答
uint8_t I2C_Wait_Ack(void) {
    uint8_t ErrTime = 0;

    I2C_SDA_H();  // 释放SDA线
    SDA_IN();
    HAL_Delay(1);
    I2C_SCL_H();  // 拉高SCL，准备读取应答
    HAL_Delay(1);

    while(I2C_SDA_READ()) {
        ErrTime++;
        if(ErrTime > 250) {
            I2C_Stop();
            return 1;  // 读取应答失败
        }
    }

    I2C_SCL_L();  // 应答结束
    return 0;  // 成功接收到应答
}

// I2C发送一个字节
void I2C_Send_Byte(uint8_t byte) {
    uint8_t i = 8;
    while(i--) {
        I2C_SCL_L();  // 准备数据
        HAL_Delay(1);
        if(byte & 0x80) I2C_SDA_H();
        else I2C_SDA_L();
        byte <<= 1;
        HAL_Delay(1);
        I2C_SCL_H();  // 数据稳定，产生时钟
        HAL_Delay(1);
        I2C_SCL_L();  // 拉低时钟，准备发送下一位
    }
}

// I2C读取一个字节
uint8_t I2C_Read_Byte(uint8_t ack) {
    uint8_t i = 8;
    uint8_t receive = 0;

    I2C_SDA_H();  // 释放SDA线，准备接收数据
    while(i--) {
        receive <<= 1;
        I2C_SCL_L();  // 拉低时钟开始数据接收
        HAL_Delay(1);
        I2C_SCL_H();  // 拉高时钟读取数据
        if(I2C_SDA_READ()) receive |= 0x01;  // 如果SDA线为高，接收位为1
        HAL_Delay(1);
    }
    if (!ack)
        I2C_NAck();  // 发送不应答
    else
        I2C_Ack();   // 发送应答

    return receive;
}

// I2C发送应答
void I2C_Ack(void) {
    I2C_SCL_L();
    SDA_OUT();
    I2C_SDA_L();  // 拉低SDA产生应答信号
    HAL_Delay(1);
    I2C_SCL_H();
    HAL_Delay(1);
    I2C_SCL_L();
}

// I2C发送不应答
void I2C_NAck(void) {
    I2C_SCL_L();
    SDA_OUT();
    I2C_SDA_H();  // 释放SDA产生不应答信号
    HAL_Delay(1);
    I2C_SCL_H();
    HAL_Delay(1);
    I2C_SCL_L();
}


// 使用软件I2C向FT5206写入数据
uint8_t FT5206_WR_Reg_SoftI2C(uint16_t reg, uint8_t *buf, uint8_t len) {
    uint8_t i;
    uint8_t ret = 0;
    I2C_Start();
    I2C_Send_Byte(FT_CMD_WR);    // 发送写命令
    if (I2C_Wait_Ack()) {        // 等待应答
        ret = 1;
    }
    //I2C_Send_Byte(reg & 0xFF);   // 发送寄存器地址
    I2C_Send_Byte( 0x7F);   // 发送寄存器地址
    I2C_Wait_Ack();
    for (i = 0; i < len; i++) {
        I2C_Send_Byte(buf[i]);   // 发送数据
        if (I2C_Wait_Ack()) {    // 等待应答
            ret = 1;
            break;
        }
    }
    I2C_Stop();                  // 生成停止条件
    return ret;
}

// 使用软件I2C从FT5206读取数据
void FT5206_RD_Reg_SoftI2C(uint16_t reg, uint8_t *buf, uint8_t len) {
    I2C_Start();
    I2C_Send_Byte(FT_CMD_WR);   // 发送写命令
    I2C_Wait_Ack();
    I2C_Send_Byte(reg & 0xFF);  // 发送寄存器地址
    I2C_Wait_Ack();
    I2C_Stop();
    I2C_Start();
    I2C_Send_Byte(FT_CMD_RD);   // 发送读命令
    I2C_Wait_Ack();
    while (len) {
        len--;
        *buf = I2C_Read_Byte(len == 0 ? 0 : 1); // 读取数据，最后一个数据发送NACK
        buf++;
    }
    I2C_Stop();                 // 产生停止条件
}
// FT5206初始化函数，使用软件I2C
uint8_t FT5206_Init_SoftI2C(void) {
    uint8_t temp[2];
    FT_RST(0);              // 复位FT5206
    HAL_Delay(21);         // 等待至少5ms
    FT_RST(1);              // 释放复位
    HAL_Delay(50);         // 等待FT5206启动
    temp[0] = 0;
    FT5206_WR_Reg_SoftI2C(FT_DEVIDE_MODE, temp, 1); // 进入正常操作模式
    FT5206_WR_Reg_SoftI2C(FT_ID_G_MODE, temp, 1);  // 设置为查询模式

    temp[0]=22;								//触摸有效值，22，越小越灵敏
    FT5206_WR_Reg_SoftI2C(FT_ID_G_THGROUP,temp,1);	//设置触摸有效值
    temp[0]=12;								//激活周期，不能小于12，最大14
    FT5206_WR_Reg_SoftI2C(FT_ID_G_PERIODACTIVE,temp,1);
    return 0;   // 初始化成功
}

// 扫描触摸屏
// mode: 0, 正常扫描模式
uint8_t FT5206_Scan_SoftI2C(uint8_t mode) {
    uint8_t buf[4];
    uint8_t i = 0;
    uint8_t res = 0;
    uint8_t temp;
    static uint8_t t = 0;

    t++;
    if ((t % 10) == 0 || t < 10) { // 降低CPU占用率
        FT5206_RD_Reg_SoftI2C(FT_REG_NUM_FINGER, &mode, 1); // 读取触摸点的状态
        if ((mode & 0XF) && ((mode & 0XF) < 6)) {
            temp = 0xFF << (mode & 0XF);
            tp_dev.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;
            for (i = 0; i < 5; i++) {
                if (tp_dev.sta & (1 << i)) {
                    FT5206_RD_Reg_SoftI2C(FT5206_TPX_TBL[i], buf, 4); // 读取XY坐标值
                    tp_dev.x[i] = ((uint16_t)(buf[0] & 0X0F) << 8) + buf[1];
                    tp_dev.y[i] = ((uint16_t)(buf[2] & 0X0F) << 8) + buf[3];
                }
            }
            res = 1;
            if (tp_dev.x[0] == 0 && tp_dev.y[0] == 0) mode = 0; // 忽略全零的数据
            t = 0;
        }
    }
    if ((mode & 0X1F) == 0) { // 无触摸点按下
        if (tp_dev.sta & TP_PRES_DOWN) { // 之前是被按下的
            tp_dev.sta &= ~(1 << 7); // 标记按键松开
        } else { // 之前就没有被按下
            tp_dev.x[0] = 0xFFFF;
            tp_dev.y[0] = 0xFFFF;
            tp_dev.sta &= 0XE0; // 清除点有效标记
        }
    }
    if (t > 240) t = 10; // 重新从10开始计数
    return res;
}
