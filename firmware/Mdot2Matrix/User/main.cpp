#include "debug.h"

uint64_t *uid = (uint64_t*)0x1ffff7E8;

GPIO_TypeDef *colBank[] =     {GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA,
                                GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB,
                                GPIOB, GPIOB, GPIOB, GPIOB};
int colPin[] =                {0, 1, 2, 3, 4, 5, 6, 7,
                                 4, 5, 6, 7, 8, 9, 10, 11,
                                 12, 13, 14, 15};
//const int ticksPerBit = 0x8000;
const int ticksPerBit = 0x80;
const int bitDepth = 8;
const int xres = 20;
const int yres = 12;
uint16_t frame[yres][xres];
uint32_t frameBits[2][bitDepth][xres];

volatile int currentFrame = 0;
volatile int nextFrame = 0;
volatile uint32_t time = 0;
volatile uint32_t currentBit = 0;
volatile uint32_t currentCol = 0;
volatile uint32_t previousCol = 0;
void __attribute__((interrupt("WCH-Interrupt-fast"))) SysTick_Handler(void)

{
    time++;
    colBank[previousCol]->BCR = 1 << colPin[previousCol];
    GPIOC->OUTDR = frameBits[currentFrame][currentBit][currentCol];
    //GPIOC->OUTDR = 0b111111111111 >> currentCol;//frameBits[currentFrame][currentBit][currentCol];
    colBank[currentCol]->BSHR = 1 << colPin[currentCol];
    previousCol = currentCol;
    currentCol++;
    if(currentCol == xres)
    {
        currentCol = 0;
        currentBit++;
        if(currentBit == bitDepth)
        {
            currentBit = 0;
            currentFrame = nextFrame;
        }
    }
    SysTick->CMP = ticksPerBit << currentBit;
    SysTick->SR=0;
    return;
}

bool show()
{
    if(currentFrame != nextFrame) return false;
    int newFrame = currentFrame ^ 1;
    for(int b = 0; b < bitDepth; b++)
    {
        for(int x = 0; x < xres; x++)
        {
            u_int32_t bitMask = 0;
            for(int y = 0; y < yres; y++)
            {
                int bit = (frame[y][x] >> (16 - bitDepth + b)) & 1;
                bitMask |= bit << y;
            }
            //bitMask = 0b111111111111 >> b;
            frameBits[newFrame][b][x] = bitMask ^ 0b111111111111;   //this matrix has inverted bits
        }
    }
    nextFrame = newFrame;
    return true;
}

void USART_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}


int main(void)
{
    //USART_Printf_Init(115200);
    USART_CFG();
/*
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    for(int i = 0; i < 20; i++)
    {
        GPIO_InitStructure.GPIO_Pin = 1 << colPin[i];
        GPIO_Init(colBank[i], &GPIO_InitStructure);
    }

    GPIOC->CFGLR =  ((0b1000| 0b00) <<  0) | ((0b1000| 0b00) <<  4) |
                    ((0b1000| 0b00) <<  8) | ((0b1000| 0b00) << 12) |
                    ((0b1000| 0b00) << 16) | ((0b1000| 0b00) << 20) |
                    ((0b1000| 0b00) << 24) | ((0b1000| 0b00) << 28);
    GPIOC->CFGHR =  ((0b1000| 0b00) <<  0) | ((0b1000| 0b00) <<  4) |
                    ((0b1000| 0b00) <<  8) | ((0b1000| 0b00) << 12);
    GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_SET);
    GPIOC->OUTDR = 0b000000000000;
    GPIO_WriteBit(colBank[0], 1 << colPin[0], Bit_SET);
    for(int y = 0; y < yres; y++)
        for(int x = 0; x < xres; x++)
            frame[y][x] = (x * yres + y) * 256;
    show();
    currentFrame = nextFrame;
    show();

    //Delay_Init();

    SysTick->SR = 0;
    SysTick->CNT = 0;
    SysTick->CMP = ticksPerBit;
    SysTick->CTLR = 1 | 2 | 4 | 8;
    NVIC_EnableIRQ(SysTicK_IRQn);
    SetVTFIRQ((u32)SysTick_Handler, SysTicK_IRQn, 0, ENABLE);
**/
    while(1)
    {
        printf("Juhuuu! It wooorks!\n");
/*
        static int framePos = 0;
        while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
        u_int8_t c = USART_ReceiveData(USART1);
        if(c >= 'A' && c <= 'F') c+= 32;
        if(c == 's')
            framePos = 0;
        else if((c >= '0' && c <= '9') ||
                (c >= 'a' && c <= 'f'))
        {
            //printf("%c", c);
            int g = 0;
            if(c >= '0' && c <= '9')
                g = c - '0';
            else
                g = c - 'a' + 10;
            frame[0][framePos] = g << 12; //framePos will overflow but trust me it works
            framePos++;
            if(framePos == xres * yres)
            {
                framePos = 0;
                show();
            }
        }
        */
    }
}
