#define RECIEVER_SIZE 64
#define TRANSMITER_SIZE 64
//extern char cOdebranyZnak;

enum eRecieverStatus {EMPTY, READY, OVERFLOW};
struct RecieverBuffer
{ 
	char cData[RECIEVER_SIZE];
	unsigned char ucCharCtr; 
	enum eRecieverStatus eStatus;
};

enum eTransmiterStatus {FREE, BUSY};
struct TransmiterBuffer
{
	char cData[TRANSMITER_SIZE]; 
	enum eTransmiterStatus eStatus; 
	unsigned char fLastCharacter; 
	unsigned char cCharCtr;
};



void UART_InitWithInt(unsigned int uiBaudRate);

void Reciever_GetStringCopy(char * DestinationBuffer);
enum eRecieverStatus eReciever_GetStatus(void);

void Transmiter_SendString(char cString[]);
enum eTransmiterStatus eTransmiter_GetStatus(void);
