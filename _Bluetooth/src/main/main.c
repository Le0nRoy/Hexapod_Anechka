#include "mcu_support_package/inc/stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include <string.h>
//PA2 - TX_output
//PA3 - RX_input

#define com_size 10

#define SERVO_180 18000
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

char Command(char command);

void Ports(void);
void Ini_USART(void);
void Timer(void);
void Receive_Debugging(void);
void Feedback(void);

void USART1_IRQHandler(void)
{
  char command; // Буфер для команды
  //uint16_t i = 0;
      
  if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
  {
    command = USART_ReceiveData(USART1);
    Command(command); // Запись команды для передачи в другую функцию
  }
  
  //Feedback(); //Можно добавить обратную связь
  
}
  
int main()
{  
  __disable_irq();
  
  Ports();
  Ini_USART();
  Timer();
  
  __enable_irq();
  while(1)
  {
    switch(Command('r'))
    {
      case 'F':
      {
        // Forward
        
        break;
      }
      
      case 'B':
      {
        //Back
        
        break;
      }
      
      case 'L':
      {
        //Left
        
        break;
      }
      
      case 'R':
      {
        //Right
        
        break;
      }
      
      case 'S':
      {
        //Stop
        
        break;
      }
      
    }
  }

  return 0;
}

void Ports(void)
{
  ////Включем порт А////
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
  
  GPIO_InitTypeDef PORT;
  GPIO_StructInit(&PORT);
  // Настроим ногу (PA1) к которой подключен сервопривод
  PORT.GPIO_Pin = GPIO_Pin_1;
  PORT.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &PORT);
  // Диод на Nucleo (PA5)
  PORT.GPIO_Pin = GPIO_Pin_5;
  PORT.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &PORT);
  
  //А2(9) на выход (USART2(1)_TX)
  PORT.GPIO_Pin = GPIO_Pin_9; 
  PORT.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &PORT);
  //А3(10) на вход (USART2(1)_RX) (CN9_D2)
  PORT.GPIO_Pin = GPIO_Pin_10; 
  PORT.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &PORT);
}

void Ini_USART(void)
{
  //Для PA9_10 USART1
  //Для PA2_3 USART2
  ////Настройка USART////
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
  
  USART_InitTypeDef USART_In;
  
  //USART_StructInit(&Usart);
  USART_In.USART_BaudRate = 115200;//бодрейт
  USART_In.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//управление потоком
	USART_In.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//включение приема и передачи
	USART_In.USART_Parity = USART_Parity_No;//четность
	USART_In.USART_StopBits = USART_StopBits_1;//количество стоп-битов
	USART_In.USART_WordLength = USART_WordLength_8b; //количество бит данных
  USART_Init(USART1, &USART_In);
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);
  
  USART_Cmd(USART1, ENABLE);
}

void Timer(void)
{
  //Изменить на инициализацию через SPL
  
  //Разрешаем таймеру использовать ногу PA1 для ШИМа
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  
  TIM2->CCER |= (TIM_CCER_CC2E);
  TIM2->CCMR1|= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
  TIM2->ARR = 0xFFFF; // период таймера
  //Настраиваем предделитель чтоб частота ШИМа была в районе 50 Гц
  TIM2->PSC = 6;
  //Запускаем таймер
  TIM2->CR1 |= TIM_CR1_CEN;
}

char Command(char command)
{
  static char exe_command;
  char com = 'S';
  
  if(command=='r')//if(strncmp(command, "return", 6)==0)
  {
    
    if(exe_command=='F'||exe_command=='L'||exe_command=='R'||exe_command=='B')
    {
      //Если входим в функцию не из хэндлера, то возвращается команда на исполнение
      com = exe_command;
      //exe_command = 'S';
      return com;
    }
    else
    {
      // Если вход в функцию произошел до первого считывания команды, вместо случайного значения
      // из памяти (переменная не инициализирована) будет возвращена команда на стоп
      return com; //return 'S';
    }
  }
  
//  if(strncmp(command, "ForwardA", 8)==0||strncmp(command, "BackA", 5)==0||strncmp(command, "LeftA", 5)==0||strncmp(command, "RightA", 6)==0||strncmp(command, "StopA", 5)==0)
//  {
//    // При входе из хэндлера запоминается команда для последующего исполнения
//    // на выходе будет только первая буква команды (заглавная)
//    // F - вперед
//    // B - назад
//    // L - влево
//    // R - вправо
//    // S - стоп
//    exe_command = command[0];
//  }
  
  if(command=='F'||command=='B'||command=='L'||command=='R'||command=='S')
  {
    exe_command = command;
  }
  
  return 'S'; // В случае неправильного входа в функцию возвращает команду на стоп.
}

void Receive_Debugging(void)
{

  uint32_t i = 0;
  GPIO_WriteBit(GPIOA, GPIO_Pin_5, 1);
  while(i<0xFFFFFFFF)
  {
    i++;
  }
  i = 0;
  GPIO_WriteBit(GPIOA, GPIO_Pin_5, 0);
}
void Feedback(void)
{
  if(USART_GetFlagStatus(USART1, USART_FLAG_TXE)) // Check if ready to send message
  {
    USART_SendData(USART1, 'F');
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != RESET) // Wait until message sent
    {
    }
    //USART_ClearFlag(USART1, USART_FLAG_TXE);
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
