/**
 * 1 初始化可指定引脚为 -1 ,串口将通过 iomux 输出
 * 2 读取数据时等待时间建议为 20ms
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_UART

#include "driver/uart.h"

#define EXUART_QUEUE_SIZE (10)
#define EXUART_RX_BUFFER_SIZE (1024 * 16)
#define EXUART_TX_BUFFER_SIZE (1024 * 16)

void exuart_init(uart_port_t uart_num, int baud_rate, int tx_io_num,
                 int rx_io_num);
void exuart_deinit(uart_port_t uart_num);

#endif

#ifdef __cplusplus
}
#endif