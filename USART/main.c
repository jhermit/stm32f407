#include <stm32f4xx.h>
#include <misc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_usart.h>

#define MAX_STRLEN 128 
volatile char received_string[MAX_STRLEN+1];



void Delay( __IO uint32_t nCount )
{
    while ( nCount-- ) {
        ;
    }
}



void init_USART() 
{

	/* Enable APB2 peripheral clock for USART2
     * and PA2->TX, PA3->RX
     */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	
	/* Enable APB2 peripheral clock for USART2 */ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* GPIOA Configuration for USART2 */
	GPIO_InitTypeDef GPIO_InitStructure;  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;  ///< PA2(TX) and PA3(RX) are used 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            ///< Configured pins as alternate function 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       ///< IO speed 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          ///< Output type as push-pull mode
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;           ///< Activates pullup resistor
	GPIO_Init(GPIOA, &GPIO_InitStructure);                  ///< Initial GPIOA

	/* Connect GPIO pins to AF */ 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
    /* Configuration for USART2 */
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;                      ///< Baudrate is set to 9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;     ///< Standard word length = 8 bits
	USART_InitStructure.USART_StopBits = USART_StopBits_1;          ///< Standard stop bit = 1 bit
	USART_InitStructure.USART_Parity = USART_Parity_No;             ///< Standard parity bit = NONE
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; ///< No flow control
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; ///< To enable receiver and transmitter
	USART_Init(USART2, &USART_InitStructure);                       ///< Initial USART2

    /* Enable receiver interrupt for USART2 and 
     * Configuration for NVIC 
     * */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                  ///< Enable USART2 receiver interrupt 
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		        ///< Configure USART2 interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;       ///< Set the priority group of USART2 interrupt
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		        ///< Set the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			        ///< Globally enable USART2 interrupt
	NVIC_Init(&NVIC_InitStructure);							        ///< Initial NVIC 	

    /* Enable USART2 */
	USART_Cmd(USART2, ENABLE);
}



void init_led(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable AHB1 peripheral clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* Configure the LED pin as push-pull output. */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}



/* This function is used to transmit a string of characters via 
 * the USART specified in USARTx.
 * 
 * It takes two arguments: USARTx --> can be any of the USARTs e.g. USART1, USART2 etc.
 * 						   (volatile) char *s is the string you want to send
 * 
 * Note: The string has to be passed to the function as a pointer because
 * 		 the compiler doesn't know the 'string' data type. In standard
 * 		 C a string is just an array of characters
 * 
 * Note 2: At the moment it takes a volatile char because the received_string variable
 * 		   declared as volatile char --> otherwise the compiler will spit out warnings
 * */
void USART_puts(USART_TypeDef* USARTx, volatile char *s){

	while(*s){
		// wait until data register is empty
		while( !(USARTx->SR & 0x00000040) ); 

        USART_SendData(USARTx, *s);
		*s++;
	}
}



int main()
{

    init_led();
    init_USART();

    USART_puts( USART2, "Init complete! Hello World!\r\n" );

    while ( 1 ) {  
        ;
    }
}



void USART2_IRQHandler(void){
	
	// check if the USART2 receive interrupt flag was set
	if( USART_GetITStatus(USART2, USART_IT_RXNE) ){

        USART_SendData( USART2, USART2->DR );

	}
}
