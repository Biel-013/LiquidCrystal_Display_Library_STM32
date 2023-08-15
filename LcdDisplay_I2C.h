/*
 *      Armazenamento Externo EEPROM - EEPROM.h
 *
 *      Data:
 *      Autor: Gabriel Luiz
 *      Contato: (31) 97136-4334 || gabrielluiz.eletro@gmail.com
 */
/*
 * 		- Links Úteis -
 *
 */

#ifndef INC_LCDDISPLAY_I2C_H_
#define INC_LCDDISPLAY_I2C_H_

#include "stm32f1xx_hal.h"

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup

#define LCD_STATUS_ON (1 << 0U)
#define LCD_STATUS_OFF (0 << 0U)

#define LCD_CMD_CLEAR (1 << 0U)
#define LCD_CMD_RETURN_CURSOR (1 << 1U)

#define LCD_ENTRY_MODE_MOVING_LEFT ((1 << 2U) | (1 << 1U))
#define LCD_ENTRY_MODE_MOVING_RIGHT ((1 << 2U) | (0 << 1U))
#define LCD_ENTRY_MODE_SHIFTING_ON ((1 << 2U) | (1 << 0U))
#define LCD_ENTRY_MODE_SHIFTING_OFF ((1 << 2U) | (0 << 0U))

#define LCD_SCREEN_SHIFT_LEFT ((1 << 4U) | (1 << 3U) | (0 << 2U))
#define LCD_SCREEN_SHIFT_RIGHT ((1 << 4U) | (1 << 3U) | (1 << 2U))
#define LCD_CURSOR_SHIFT_LEFT ((1 << 4U) | (0 << 3U) | (0 << 2U))
#define LCD_CURSOR_SHIFT_RIGHT ((1 << 4U) | (0 << 3U) | (1 << 2U))

#define LCD_BIT_DISPLAY_ON ((1 << 3U) | (1 << 2U))
#define LCD_BIT_DISPLAY_OFF ((1 << 3U) | (0 << 2U))
#define LCD_BIT_CURSOR_ON ((1 << 3U) | (1 << 1U))
#define LCD_BIT_CURSOR_OFF ((1 << 3U) | (0 << 1U))
#define LCD_BIT_CURSOR_BLINK_ON ((1 << 3U) | (1 << 0U))
#define LCD_BIT_CURSOR_BLINK_OFF ((1 << 3U) | (0 << 0U))

#define LCD_BIT_FUNCTION_8_BIT_MODE ((1 << 5U) | (1 << 4U))
#define LCD_BIT_FUNCTION_4_BIT_MODE ((1 << 5U) | (0 << 4U))
#define LCD_BIT_FUNCTION_2_LINE_MODE ((1 << 5U) | (1 << 3U))
#define LCD_BIT_FUNCTION_1_LINE_MODE ((1 << 5U) | (0 << 3U))
#define LCD_BIT_FUNCTION_5_X_11_FONT ((1 << 5U) | (1 << 2U))
#define LCD_BIT_FUNCTION_5_X_8_FONT ((1 << 5U) | (0 << 2U))

#define LCD_BIT_SET_CGRAM_ADRESS (1 << 6U)
#define LCD_BIT_SET_DDRAM_ADRESS (1 << 7U)

#define LCD_BIT_WRITE_DATA_RAM ((1 << 9U) | (0 << 8U))
#define LCD_BIT_READ_DATA_RAM ((1 << 9U) | (1 << 8U))

/* USER CODE END PD */

/*----------------------- BYTES PARA LER OU ESCREVER ----------------------*/

/**
 * @brief Função usada para determinar a quantidade de bytes a escrever ou ler
 * @param Tamanho: Quantidade de bytes
 * @param Posicao: Posição de escrita na página
 * @param tamanho_pagina: Tamanho de uma unica página
 * @retval Quantidade de bytes para se escrever na página
 */

HAL_StatusTypeDef LCD_Init();
HAL_StatusTypeDef LCD_SendCmd(char cmd);
HAL_StatusTypeDef LCD_SendData(char data);
HAL_StatusTypeDef LCD_SendBuffer(char *str);
HAL_StatusTypeDef LCD_Clear();
HAL_StatusTypeDef LCD_Cursor(uint8_t CURSOR_status, uint8_t BLINK_status);
HAL_StatusTypeDef LCD_SetAdress_DDRAM(uint8_t ADRESS);
HAL_StatusTypeDef LCD_SetPosition(uint8_t LINHA, uint8_t COLUNA);
void StoreInBuffer_f(char c, char *buffer, uint8_t *index);
HAL_StatusTypeDef LCD_printf(const char *BUFFER_IN, ...);

#endif /* INC_LCDDISPLAY_I2C_H_ */
