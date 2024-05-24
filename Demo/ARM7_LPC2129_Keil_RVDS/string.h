//#define NULL '\0'

enum CompResult{DIFFERENT, EQUAL};
enum Result{OK, ERROR};

//---------------------------- LANCUCHY ZNAKOWE - OPERACJE PROSTE ----------------------------
void CopyString(char pcSource[], char pcDestination[]);
enum CompResult eCompareString(char pcStr1[], char pcStr2[]);
void AppendString(char pcSourceStr[], char pcDestinationStr[]);
void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar);

//------------------------------- LANCUCHY ZNAKOWE - KONWERSJE -------------------------------
void UIntToString(unsigned int uiValue, char pcStr[]);
enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue);
void AppendUIntToString(unsigned int uiValue, char pcDestinationStr[]);
