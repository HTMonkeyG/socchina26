#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <driver/uart.h>
#include <driver/gpio.h>
#include <dc2.h>
#include <lwrb/lwrb.h>
#include <lwpkt/lwpkt.h>
#include "shared/binaryStream.h"
#include "frontend/connection/uartPort.h"

#define kUartTxPin   17
#define kUartRxPin   18

static QueueHandle_t hUartQueue = NULL;
static SemaphoreHandle_t hMutex = NULL;
static TaskHandle_t hThread = NULL;
static lwrb_t gLwpktTxRb, gLwpktRxRb;
static u08 gLwpktTxRbData[kUartBufSize]
  , gLwpktRxRbData[kUartBufSize];
static lwpkt_t gLwpkt;

static void s_UartThread(
  void *
) {
  uart_event_t event;
  u08 buffer[kUartBufSize] = {0};

  while (1) {
    if (xQueueReceive(hUartQueue, (void *)&event, (TickType_t)portMAX_DELAY) == pdFAIL)
      continue;

    switch (event.type) {
      case UART_DATA:
        uart_read_bytes(UART_NUM_1, buffer, event.size, portMAX_DELAY);
        if (xSemaphoreTake(hMutex, portMAX_DELAY)) {
          lwrb_write(&gLwpktRxRb, buffer, event.size);
          xSemaphoreGive(hMutex);
        }
        break;
      case UART_FIFO_OVF:
        uart_flush_input(UART_NUM_1);
        printf("uart hw buffer overflow.\n");
        break;
      case UART_BUFFER_FULL:
        uart_flush_input(UART_NUM_1);
        printf("uart sw buffer full.\n");
        break;
      default:
        break;
    }
  }
}

void Uart_Initialize() {
  // Init lwpkt.
  lwrb_init(&gLwpktTxRb, gLwpktTxRbData, kUartBufSize);
  lwrb_init(&gLwpktRxRb, gLwpktRxRbData, kUartBufSize);
  lwpkt_init(&gLwpkt, &gLwpktTxRb, &gLwpktRxRb);

  // Init uart.
  uart_config_t uart1 = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_DEFAULT
  };
  uart_param_config(UART_NUM_1, &uart1);
  gpio_reset_pin(kUartRxPin);
  uart_set_pin(UART_NUM_1, kUartTxPin, kUartRxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(UART_NUM_1, kUartBufSize, kUartBufSize, 20, &hUartQueue, 0);

  // Create mutex.
  hMutex = xSemaphoreCreateMutex();
  assert(hMutex);

  // Create background task.
  xTaskCreate(s_UartThread, "s_UartThread", 2048, NULL, 1, &hThread);
  assert(hThread);
}

void Uart_Update() {
  u08 buf[kUartBufSize];

  // Handle send.
  size_t len = lwrb_get_full(&gLwpktTxRb);
  if (len) {
    lwrb_read(&gLwpktTxRb, buf, len);
    uart_write_bytes(UART_NUM_1, buf, len);
  }
}

void Uart_Terminate() {
  vTaskDelete(hThread);
  uart_driver_delete(UART_NUM_1);
}

void Uart_SendMessage(
  u08 packetId,
  const u08 *data,
  size_t length
) {
  lwpkt_write(&gLwpkt, 191, packetId, data, length);
}

i08 Uart_ReceiveMessage(
  u08 *packetId,
  u08 *data,
  size_t *length
) {
  i08 ret = 0;

  // Handle receive.
  if (xSemaphoreTake(hMutex, portMAX_DELAY)) {
    if (lwpkt_read(&gLwpkt) != lwpktVALID)
      goto Return;

    size_t len = lwpkt_get_data_len(&gLwpkt);
    *packetId = lwpkt_get_cmd(&gLwpkt);
    memcpy(data, lwpkt_get_data(&gLwpkt), len);
    *length = len;
    ret = 1;
  } else
    return ret;

Return:
  xSemaphoreGive(hMutex);
  return ret;
}
