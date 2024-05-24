#include "string.h"

#define NULL '\0'

//---------------------------- LANCUCHY ZNAKOWE - OPERACJE PROSTE ----------------------------
void CopyString(char pcSource[], char pcDestination[])
{
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter = 0; pcSource[ucCharacterCounter] != NULL; ucCharacterCounter++)
	{
		pcDestination[ucCharacterCounter] = pcSource[ucCharacterCounter];
	}
	pcDestination[ucCharacterCounter] = NULL;
}


enum CompResult eCompareString(char pcStr1[], char pcStr2[])
{
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter = 0; pcStr1[ucCharacterCounter] != NULL; ucCharacterCounter++)
	{
		if(pcStr1[ucCharacterCounter] != pcStr2[ucCharacterCounter])
		{
			return DIFFERENT;
		}
	}
	
	if(NULL == pcStr2[ucCharacterCounter])
	{
		return EQUAL;
	}
	return DIFFERENT;
}


void AppendString(char pcSourceStr[], char pcDestinationStr[])
{
	unsigned char ucLastCharacter;
	
	for(ucLastCharacter = 0; pcDestinationStr[ucLastCharacter] != NULL; ucLastCharacter++){}
	CopyString(pcSourceStr, &pcDestinationStr[ucLastCharacter]);
}


void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar)
{
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter = 0; pcString[ucCharacterCounter] != NULL; ucCharacterCounter++)
	{
		if(pcString[ucCharacterCounter] == cOldChar)
		{
			pcString[ucCharacterCounter] = cNewChar;
		}
	}
}



//---------------------------- LANCUCHY ZNAKOWE - KONWERSJE ----------------------------
void UIntToString(unsigned int uiValue, char pcStr[])
{
	signed char ucTetradeIndex;
	unsigned char ucTetradeValue;
	
	pcStr[0] = '0';
	pcStr[1] = 'x';
	
	for(ucTetradeIndex = 0; ucTetradeIndex < 4; ucTetradeIndex++)
	{
		ucTetradeValue = (uiValue >> (4 * ucTetradeIndex)) & 0x0F;
		if(ucTetradeValue < 10)
		{
			pcStr[5 - ucTetradeIndex] = '0' + ucTetradeValue;
		}
		else
		{
			pcStr[5 - ucTetradeIndex] = 'A' + ucTetradeValue - 10;
		}
	}
	
	pcStr[6] = NULL;
}


enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue)
{
	unsigned char ucStringIndex;
	unsigned char ucTetradeCharacter;
	
	*puiValue = 0;
	
	if((pcStr[0] != '0') || (pcStr[1] != 'x') || (NULL == pcStr[2]))
	{
		return ERROR;
	}
	
	for(ucStringIndex = 2; ucStringIndex < 7; ucStringIndex++)
	{
		ucTetradeCharacter = pcStr[ucStringIndex];
		
		if(NULL == ucTetradeCharacter)
		{
			return OK;
		}
		else if(6 == ucStringIndex)
		{
			return ERROR;
		}
		
		*puiValue = (*puiValue) << 4;
		
		if((ucTetradeCharacter >= 'A') && (ucTetradeCharacter <= 'F'))
		{
			*puiValue = (*puiValue) | (ucTetradeCharacter - 'A' + 10);
		}
		else if((ucTetradeCharacter >= '0') && (ucTetradeCharacter <= '9'))
		{
			*puiValue = (*puiValue) | (ucTetradeCharacter - '0');
		}
		else
		{
			return ERROR;
		}
	}
	return OK;
}


void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[])
{
	unsigned char ucLastCharacter;
	
	for(ucLastCharacter = 0; pcDestinationStr[ucLastCharacter] != NULL; ucLastCharacter++){}
	UIntToString(uiValue, &pcDestinationStr[ucLastCharacter]);
}

