/*
 *       - .c
 *
 *      Data:
 *      Autor: Gabriel Luiz
 *      Contato: (31) 97136-4334 || gabrielluiz.eletro@gmail.com
 */
/*
 * 		- Links Úteis -
 *
 */

/* Private Includes ----------------------------------------------------------*/
/* USER CODE BEGIN PI */

#include "LcdDisplay_I2C.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
/* USER CODE END PI */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

extern I2C_HandleTypeDef hi2c1;

/* USER CODE END EV */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MAX_SIZE_BUFFER 80
/* USER CODE END PD */

/* External functions ------------------------------------------------------------*/
/* USER CODE BEGIN EF */
/* USER CODE END EF */

/* Private variables --------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private functions ------------------------------------------------------------*/
/* USER CODE BEGIN PF */

/*-------------------------------------------------------------------------*/

/**
 * @brief
 * @param
 * @param
 * @param
 * @retval
 */

HAL_StatusTypeDef LCD_Init() {
	HAL_StatusTypeDef STATUS = HAL_OK;
	HAL_Delay(50);  // wait for >40ms
	if (LCD_SendCmd(LCD_BIT_FUNCTION_8_BIT_MODE) != HAL_OK)
		STATUS = HAL_ERROR;
	HAL_Delay(5);  // wait for >4.1ms
	if (LCD_SendCmd(LCD_BIT_FUNCTION_8_BIT_MODE) != HAL_OK)
		STATUS = HAL_ERROR;
	HAL_Delay(1);  // wait for >100us
	if (LCD_SendCmd(LCD_BIT_FUNCTION_8_BIT_MODE) != HAL_OK)
		STATUS = HAL_ERROR;
	HAL_Delay(10);
	if (LCD_SendCmd(LCD_BIT_FUNCTION_4_BIT_MODE) != HAL_OK)
		STATUS = HAL_ERROR;  // 4bit mode
	HAL_Delay(10);

	// dislay initialisation
	if (LCD_SendCmd(
			LCD_BIT_FUNCTION_4_BIT_MODE | LCD_BIT_FUNCTION_2_LINE_MODE
					| LCD_BIT_FUNCTION_5_X_8_FONT) != HAL_OK)
		STATUS = HAL_ERROR; // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	if (LCD_SendCmd(LCD_BIT_DISPLAY_OFF | LCD_BIT_CURSOR_OFF |
	LCD_BIT_CURSOR_BLINK_OFF) != HAL_OK)
		STATUS = HAL_ERROR; //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	if (LCD_SendCmd(LCD_CMD_CLEAR) != HAL_OK)
		STATUS = HAL_ERROR;  // clear display
	HAL_Delay(1);
	if (LCD_SendCmd(LCD_ENTRY_MODE_MOVING_LEFT | LCD_ENTRY_MODE_SHIFTING_OFF)
			!= HAL_OK)
		STATUS = HAL_ERROR; //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	if (LCD_SendCmd(LCD_BIT_DISPLAY_ON | LCD_BIT_CURSOR_OFF |
	LCD_BIT_CURSOR_BLINK_OFF) != HAL_OK)
		STATUS = HAL_ERROR; //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	return STATUS;
}

HAL_StatusTypeDef LCD_SendCmd(char cmd) {
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd & 0xf0);
	data_l = ((cmd << 4) & 0xf0);
	data_t[0] = data_u | 0x0C;  //en=1, rs=0
	data_t[1] = data_u | 0x08;  //en=0, rs=0
	data_t[2] = data_l | 0x0C;  //en=1, rs=0
	data_t[3] = data_l | 0x08;  //en=0, rs=0
	return HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t*) data_t,
			4, 100);
}

HAL_StatusTypeDef LCD_SendData(char data) {
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data & 0xf0);
	data_l = ((data << 4) & 0xf0);
	data_t[0] = data_u | 0x0D;  //en=1, rs=1
	data_t[1] = data_u | 0x09;  //en=0, rs=1
	data_t[2] = data_l | 0x0D;  //en=1, rs=1
	data_t[3] = data_l | 0x09;  //en=0, rs=1
	return HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t*) data_t,
			4, 100);
}

HAL_StatusTypeDef LCD_SendBuffer(char *buffer) {
	HAL_StatusTypeDef STATUS = HAL_OK;
	while (*buffer)
		if (LCD_SendData(*buffer++) != HAL_OK)
			STATUS = HAL_ERROR;
	return STATUS;
}

HAL_StatusTypeDef LCD_Clear() {
	HAL_StatusTypeDef STATUS = HAL_OK;
	STATUS = LCD_SendCmd(LCD_CMD_CLEAR);
	HAL_Delay(5);
	return STATUS;
}

HAL_StatusTypeDef LCD_Cursor(uint8_t CURSOR_status, uint8_t BLINK_status) {
	return LCD_SendCmd(LCD_BIT_DISPLAY_ON|
			(LCD_BIT_CURSOR_ON * CURSOR_status)
					| (LCD_BIT_CURSOR_BLINK_ON * BLINK_status));
}

HAL_StatusTypeDef LCD_SetAdress_DDRAM(uint8_t ADRESS) {
	return LCD_SendCmd(LCD_BIT_SET_DDRAM_ADRESS | ADRESS);
}

HAL_StatusTypeDef LCD_SetPosition(uint8_t LINHA, uint8_t COLUNA) {
	uint8_t ENDERECO = 0;
	switch(LINHA)
	{
	case 0:
		ENDERECO = 0x00 + COLUNA;
		break;
	case 1:
		ENDERECO = 0x40 + COLUNA;
			break;
	case 2:
		ENDERECO = 0x14 + COLUNA;
			break;
	case 3:
		ENDERECO = 0x54 + COLUNA;
			break;
	default:
			break;
	}
	return LCD_SetAdress_DDRAM(ENDERECO);
}

void StoreInBuffer_f(char c, char buffer[MAX_SIZE_BUFFER], uint8_t *index) {
	if (*index < MAX_SIZE_BUFFER - 1) {
		buffer[*index] = c;
		(*index)++;
	}
}

HAL_StatusTypeDef LCD_printf(const char *BUFFER_IN, ...) {
	va_list args;
	va_start(args, BUFFER_IN);
	char BUFFER_OUT[MAX_SIZE_BUFFER] = { 0 };
	HAL_StatusTypeDef STATUS = HAL_OK;

	// Iterar pelos caracteres do formato
	uint8_t index = 0;  // Índice atual no buffer de saída
	for (const char *c = BUFFER_IN; *c != '\0'; c++) {
		if (*c == '%') {
			// Tratar substituição de formato
			c++;
			if (*c == 'c') {
				// Substituição para um caractere
				char ch = (char) va_arg(args, int);
				StoreInBuffer_f(ch, BUFFER_OUT, &index);
			} else if (*c == 's') {
				// Substituição para uma string
				const char *str = va_arg(args, const char*);
				while (*str != '\0') {
					StoreInBuffer_f(*str, BUFFER_OUT, &index);
					str++;
				}
			} else if (*c == 'd') {
				// Substituição para um número inteiro
				int num = va_arg(args, int);
				char buffer[20]; // Buffer temporário para converter o número para uma string
				sprintf(buffer, "%d", num);
				const char *str = buffer;
				while (*str != '\0') {
					StoreInBuffer_f(*str, BUFFER_OUT, &index);
					str++;
				}
			} else if (*c == 'f') {
				// Substituição para um número de ponto flutuante
				double num = va_arg(args, double);
				char buffer[20]; // Buffer temporário para converter o número para uma string
				sprintf(buffer, "%.2f", num); // @suppress("Float formatting support")
				const char *str = buffer;
				while (*str != '\0') {
					StoreInBuffer_f(*str, BUFFER_OUT, &index);
					str++;
				}
			}
		} else {
			// Enviar caracteres normais para o buffer
			StoreInBuffer_f(*c, BUFFER_OUT, &index);
		}

		// Parar se o índice atingir o tamanho máximo do buffer - 1 para deixar espaço para o caractere nulo
		if (index >= MAX_SIZE_BUFFER - 1) {
			break;
		}
	}

	// Adicionar um caractere nulo ao final do buffer
	BUFFER_OUT[index] = '\0';

	STATUS = LCD_SendBuffer(BUFFER_OUT);

	va_end(args);

	return STATUS;
}
/* USER CODE BEGIN PF */

