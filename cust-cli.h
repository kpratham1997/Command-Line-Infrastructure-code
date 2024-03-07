/* Standard includes. */
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <time.h>


/* Base type definitions */
typedef int BaseType_t;
#define pdFALSE ( ( BaseType_t ) 0 )
#define pdPASS ( ( BaseType_t ) 1 )
#define CUSTOM_CLI_MAX_COMMANDS 10 // Maximum number of commands
#define IMPORTANT "admin123" // Define the admin password

//#define ARRAY_BASED_COMMAND_REGISTER 1 
#undef ARRAY_BASED_COMMAND_REGISTER 
#undef DEBUG



/* Command Definition Struct */
typedef struct xCOMMAND_LINE_INPUT
{
    const char * pcCommand;
    const char * pcHelpString;
    BaseType_t (*pxCommandInterpreter)(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
    int8_t cExpectedNumberOfParameters; //-127 to 128
    BaseType_t isSpecialCommand; // Indicate if it's a special command (e.g., user ID or password)
} CLI_Command_Definition_t;

#ifndef ARRAY_BASED_COMMAND_REGISTER
typedef struct xCOMMAND_INPUT_LIST
{
	const CLI_Command_Definition_t *pxCommandLineDefinition;
	struct xCOMMAND_INPUT_LIST *pxNext;
} CLI_Definition_List_Item_t;
#endif

/* Function Prototypes */
BaseType_t CLIRegisterCommand(const CLI_Command_Definition_t * const pxCommandToRegister);
BaseType_t CLIProcessCommand(const char * const pcCommandInput, char * pcWriteBuffer, size_t xWriteBufferLen);
BaseType_t prvGetNumberOfParameters( const char *pcCommandString );
const char *FreeRTOS_CLIGetParameter( const char *pcCommandString, BaseType_t uxWantedParameter, BaseType_t *pxParameterStringLength );


/* Command Registration Array */
#ifdef ARRAY_BASED_COMMAND_REGISTER
CLI_Command_Definition_t commandArray[CUSTOM_CLI_MAX_COMMANDS];
#endif
int commandCount = 0;
char writeBuffer[512];

/* Help Command Function Prototype */
BaseType_t prvHelpCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

/* Set Command Function Prototype */
BaseType_t prvSetCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

/* Get Command Function Prototype */
BaseType_t prvGetCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);


BaseType_t prvGetNumberOfParameters( const char *pcCommandString );