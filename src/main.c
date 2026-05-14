/*
 * ch32v307 eide demo
 * version: v1.2
 * Copyright (c) 2022 Taoyukai
 * SPDX-License-Identifier: Apache-2.0
 */

#include "zf_common_headfile.h"


/**
 * @brief
 *
 * @return int
 */
int main(void)
{
    clock_init(SYSTEM_CLOCK_144M);      // 初始化芯片时钟 工作频率为 144MHz
    debug_init();                       // 务必保留，本函数用于初始化MPU 时钟 调试串口

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE,&GPIO_InitStructure);

    GPIO_WriteBit(GPIOE,GPIO_Pin_10,SET);

    while (1) {
        GPIO_WriteBit(GPIOE, GPIO_Pin_10, !GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_10));
        system_delay_ms(500);
    }
}
