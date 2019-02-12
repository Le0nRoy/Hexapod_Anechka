#include "mcu_support_package/inc/stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#define SERVO_180 5000
#define SERVO_0 1800
// Функция устанавливает позицию вала (в градусах)
void set_pos(uint8_t pos) 
{
  uint32_t tmp=(SERVO_180 - SERVO_0) /180 ;
  TIM2->CCR2 = SERVO_0 + tmp * pos;
}

// Функция задержки
void delay(void) 
{
  volatile uint32_t i;
  for (i=1; i != 0xFFFF; i++)
    ;
}
int main(void)
{
	//Включем порт А
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
  //Включаем Таймер 2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  GPIO_InitTypeDef PORT;
  // Настроим ногу (PA1) к которой подключен сервопривод
  PORT.GPIO_Pin = (GPIO_Pin_1);
  //Будем использовать альтернативный режим а не обычный GPIO
  PORT.GPIO_Mode = GPIO_Mode_AF_PP;
  PORT.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &PORT);
  //Разрешаем таймеру использовать ногу PA1 для ШИМа
  TIM2->CCER |= (TIM_CCER_CC2E);
  TIM2->CCMR1|= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
  TIM2->ARR = 0xFFFF; // период таймера
  //Настраиваем предделитель чтоб частота ШИМа была в районе 50 Гц
  TIM2->PSC = 6;
  //Запускаем таймер
  TIM2->CR1 |= TIM_CR1_CEN;
  uint8_t i;
  //Начинаем крутить сервой от 0 до 180 градусов. 
  while(1)
	{
//    for (i=0;i<=180;i++)
//		{
//      delay();
//      set_pos(i);
//    }
    
    for (i=180;i>=0;i--)
		{
      delay();
      set_pos(i);
    }
  }
   
}


// классический ассерт для STM32
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t * file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
     
    (void)file;
    (void)line;

    __disable_irq();
    while(1)
    {
        // это ассемблерная инструкция "отладчик, стой тут"
        // если вы попали сюда, значит вы ошиблись в параметрах. Смотрите в call stack
        __BKPT(0xAB);
    }
}
#endif
