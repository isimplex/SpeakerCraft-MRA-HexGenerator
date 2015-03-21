#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

unsigned char * buildMessage(int * iMsgLen, unsigned char ucCmd,  unsigned short ushPayloadLen,  unsigned char * pPayload)
{
	unsigned char *pMsg = 0;
	unsigned short ushCheckSum = 0;
	int iCount = 0;
	if (iMsgLen == 0)
	{
		return 0;
	}

	*iMsgLen = ushPayloadLen + 6;
	pMsg = (unsigned char *) malloc(*iMsgLen);
	if ((pPayload == 0) && (ushPayloadLen))
	{
		*iMsgLen = 0;
		return 0;
	}

	memset(pMsg, 0x00, *iMsgLen);

	pMsg[0] = 0xFF; // First sync byte
	pMsg[1] = 0x55; // Second sync byte
	pMsg[2] = (ushPayloadLen + 1) >> 8;
	pMsg[3] = (ushPayloadLen + 1) & 0x00FF;
	pMsg[4] = ucCmd;

	ushCheckSum = (unsigned short) pMsg[2] + (unsigned short) pMsg[3] + (unsigned short) pMsg[4];
	for (iCount = 0; iCount < ushPayloadLen; iCount++)
	{
		pMsg[iCount + 5] = pPayload[iCount];
		ushCheckSum += (unsigned short) pMsg[iCount + 5];
	}
	pMsg[*iMsgLen - 1] = (unsigned char) (0x100 - (ushCheckSum & 0x00FF));
	return(pMsg);
}

int main(int argc, char *argv [])
{
	int iCount = 0;
	int iParamCount = 0;
	int iBufferLength = 0;
	unsigned char * pMyBuffer = 0;
	unsigned char ucCommand = 0;
	unsigned char ucParams[64];
	
	if (argc < 2)
	{
		printf("Usage:\n<cmd> <data 1> ... <data n> (DECIMAL VALUES ONLY)\n");
		exit(1);
	}

	iParamCount = argc - 2;
	ucCommand = atoi(argv[1]);

	if ((ucCommand == 1) || // Do not use these command codes
		(ucCommand == 2) ||
		(ucCommand == 16) ||
		(ucCommand == 17) ||
		(ucCommand == 18) ||
		(ucCommand == 19) ||
		(ucCommand == 20)
		)
	{
		printf("Command %d not supported\n", ucCommand);
		exit(1);
	}

	for (iCount = 0; iCount < iParamCount; iCount++)
	{
		ucParams[iCount] = atoi(argv[2 + iCount]);;
	}

	pMyBuffer = buildMessage(&iBufferLength, ucCommand, iParamCount, ucParams);
	printf("\n");
	printf("message bytes: 0x");
	for (iCount = 0; iCount < iBufferLength; iCount++) {
		printf("%02X", *(pMyBuffer + iCount));
	}
	printf("\n");

	return 0;
}