#include <LPC210X.H>
#include "uart.h"
#include "string.h"

#define TERMINATOR '\r'
#define NULL '\0'


/************ UART ************/
// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

// PINSEL Config
#define mP0_1_UART_RX0_PIN (1<<2)
#define mP0_0_UART_TX0_PIN (1<<0)

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

////////////// Zmienne globalne ////////////

//char cOdebranyZnak;
struct RecieverBuffer sRxBuffer;
struct TransmiterBuffer sTxBuffer;

///////////////////////////////////////////
enum eTransmiterStatus eTransmiter_GetStatus(void)
{
	return sTxBuffer.eStatus;
}
///////////////////////////////////////////
void Transmiter_SendString(char cString[])
{
	CopyString(cString, sTxBuffer.cData);
	sTxBuffer.cCharCtr++;
	sTxBuffer.eStatus = BUSY;
	U0THR = sTxBuffer.cData[0];
}

///////////////////////////////////////////
char cTransmiter_GetCharacterFromBuffer()
{
	char cCurrentChar = sTxBuffer.cData[sTxBuffer.cCharCtr];

	
	if(1 == sTxBuffer.fLastCharacter)
	{
		sTxBuffer.fLastCharacter = 0;
		sTxBuffer.cCharCtr = 0;
		sTxBuffer.eStatus = FREE;
		return NULL;
	}
	else if((NULL == cCurrentChar))
	{
		sTxBuffer.fLastCharacter = 1;
		return TERMINATOR;
	}
	else
	{
		sTxBuffer.cCharCtr++;
		return cCurrentChar;
	}
	
}

///////////////////////////////////////////
void Reciever_PutCharacterToBuffer(char cCharacter)
{
	if(sRxBuffer.ucCharCtr >= RECIEVER_SIZE)
	{
		sRxBuffer.eStatus = OVERFLOW;
	}
	else if(cCharacter == TERMINATOR)
	{
		sRxBuffer.cData[sRxBuffer.ucCharCtr] = NULL;
		sRxBuffer.ucCharCtr = 0;
		sRxBuffer.eStatus = READY;
	}
	else
	{
		sRxBuffer.cData[sRxBuffer.ucCharCtr] = cCharacter;
		sRxBuffer.ucCharCtr++;
		sRxBuffer.eStatus = EMPTY;
	}
	
}

///////////////////////////////////////////
enum eRecieverStatus eReciever_GetStatus(void)
{
	return sRxBuffer.eStatus;
}

///////////////////////////////////////////
void Reciever_GetStringCopy(char * DestinationBuffer)
{
	sRxBuffer.eStatus = EMPTY;
	CopyString(sRxBuffer.cData, DestinationBuffer);
}

///////////////////////////////////////////
__irq void UART0_Interrupt (void) {
	
	unsigned int uiCopyOfU0IIR = U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii

	if((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
	{
		//cOdebranyZnak = U0RBR;
		Reciever_PutCharacterToBuffer(U0RBR);
	} 
   
	if((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
	{
		unsigned char ucCharacterToSend = cTransmiter_GetCharacterFromBuffer();
		if(NULL != ucCharacterToSend)
		{
			U0THR = ucCharacterToSend;
		}
	}

	VICVectAddr = 0; // Acknowledge Interrupt
}

/*///////////////////////////////////////////
void UART_InitWithInt(unsigned int uiBaudRate){

	// UART0
	PINSEL0 = PINSEL0 | mP0_1_UART_RX0_PIN | mP0_0_UART_TX0_PIN; // ustawic pina na odbiornik uart0
	U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // dlugosc slowa, DLAB = 1
	U0DLL   = (((15000000)/16)/uiBaudRate);                      // predkosc transmisji
	U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
	U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE ;    // ??? co tu robinmy -> wlaczenie przerwania wystepujacego gdy w buforze odbiorczym pojawia sie nowe dane i gdy zostanie wyslany znak

	// INT
	VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
	VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
} */

void UART_InitWithInt(unsigned int uiBaudRate){
	
	unsigned long ulDivisor, ulWantedClock;
	ulWantedClock=uiBaudRate*16;
	ulDivisor=15000000/ulWantedClock;
	// UART
	PINSEL0 = PINSEL0 | 0x55;                                     // ustawic piny uar0 odbiornik nadajnik
	U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // d³ugosc s³owa, DLAB = 1
	U0DLL = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff );
	ulDivisor >>= 8;
	U0DLM = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff );
	U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
	U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE ;               

	// INT
	VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
	VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel

}
