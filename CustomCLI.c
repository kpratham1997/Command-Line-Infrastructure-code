#include "cust-cli.h"

/* Initialize Commands */
const CLI_Command_Definition_t xHelpCommand = {
    "help",
    "\r\n1) Lists all the registered commands by typing 'help'.\n2) Can also be used to learn about other 'registered'commands for eg :- help set \r\n",
    prvHelpCommand,
    -1
};

const CLI_Command_Definition_t xSetCommand = {
    "set",
    "\r\nSets the passed value in system\r\n",
    prvSetCommand,
    2    
};

const CLI_Command_Definition_t xGetCommand = {
    "get",
    "\r\nget:\r\nGets the value from system\r\n",
    prvGetCommand,
    1 
};

//extra
/*
const CLI_Command_Definition_t xACommand = {
    "A",
    "\r\nget:\r\nGets the value from system\r\n",
    prvGetCommand,
    1 
};

const CLI_Command_Definition_t xBCommand = {
    "B",
    "\r\nget:\r\nGets the value from system\r\n",
    prvGetCommand,
    1 
};

const CLI_Command_Definition_t xCCommand = {
    "C",
    "\r\nget:\r\nGets the value from system\r\n",
    prvGetCommand,
    1 
};

const CLI_Command_Definition_t xDCommand = {
    "D",
    "\r\nget:\r\nGets the value from system\r\n",
    prvGetCommand,
    1 
};
const CLI_Command_Definition_t xECommand = {
    "E",
    "\r\nget:\r\nGets the value from system\r\n",
    prvGetCommand,
    1 
};
const CLI_Command_Definition_t xFCommand = {
    "F",
    "\r\nget:\r\nGets the value from system\r\n",
    prvGetCommand,
    1 
};
const CLI_Command_Definition_t xGCommand = {
    "G",
    "\r\nget:\r\nGets the value from system\r\n",
    prvGetCommand,
    1 
};
const CLI_Command_Definition_t xHCommand = {
    "H",
    "\r\nget:\r\nGets the value from system\r\n",
    prvGetCommand,
    1 
};
*/


#ifndef ARRAY_BASED_COMMAND_REGISTER //adding the help command instance to the linkedlist
static CLI_Definition_List_Item_t xRegisteredCommands =
{
	&xHelpCommand,	/* The first command in the list is always the help command, defined in this file. */
	NULL			/* The next pointer is initialised to NULL, as there are no other registered commands yet. */
};
#endif


/* Register Command Function */
BaseType_t CLIRegisterCommand(const CLI_Command_Definition_t * const pxCommandToRegister) {
 
    BaseType_t xReturn = pdFALSE;
    
    #ifndef ARRAY_BASED_COMMAND_REGISTER
	//static CLI_Definition_List_Item_t *pxLastCommandInList = &xRegisteredCommands;//creating a new instance
    //CLI_Definition_List_Item_t *pxNewListItem= NULL; //creating a pxNewListItem instance
	CLI_Definition_List_Item_t *pxExistingCommand = xRegisteredCommands.pxNext;

	while (pxExistingCommand != NULL) {
        if (pxExistingCommand->pxCommandLineDefinition == pxCommandToRegister) {
            printf("Command already registered.\n");
            return pdFALSE; // Command already registered, return failure
        }
        pxExistingCommand = pxExistingCommand->pxNext;
    }

  
	#endif

    if (commandCount < CUSTOM_CLI_MAX_COMMANDS) {
        #ifndef ARRAY_BASED_COMMAND_REGISTER
		/* Create a new list item that will reference the command being registered. */
   		CLI_Definition_List_Item_t *pxNewListItem = ( CLI_Definition_List_Item_t * ) malloc( sizeof( CLI_Definition_List_Item_t ) );	
		
		

        if( pxNewListItem != NULL )//checking if malloc was sucessful.
        {
		    /* Reference the command being registered from the newly created
		    list item. */
			// creating an instance pxNewListItem
			//pxNewListItem.pxCommandLineDefinition="help"


		    pxNewListItem->pxCommandLineDefinition = pxCommandToRegister; //Storing the instance of cli definition(properties)as the first member of pxNewListItem instance

		    /* The new list item will get added to the end of the list, so
		    pxNext has nowhere to point. */
		    pxNewListItem->pxNext = NULL;//Storing the next pointer as NULL in pxNewListItem instance of cli-list

			CLI_Definition_List_Item_t *pxLastCommandInList = &xRegisteredCommands;
			
			


		    /* Add the newly created list item to the end of the already existing
		    list. */
		    //pxLastCommandInList->pxNext = pxNewListItem; //make the new command the last item in list 

		    /* Set the end of list marker to the new list item. */
		    //pxLastCommandInList = pxNewListItem; 


			while (pxLastCommandInList->pxNext != NULL) {
                pxLastCommandInList = pxLastCommandInList->pxNext;
            }
            pxLastCommandInList->pxNext = pxNewListItem;
		


			printf("Command registered: %s\n", pxCommandToRegister->pcCommand);

			xReturn = pdPASS;
		
	    } else{

			printf("Memory allocation failed\n");


		}



	    #else 
        commandArray[commandCount] = *pxCommandToRegister; //array of structures.
        #endif
        commandCount++;
        xReturn = pdPASS;
    }else{
		printf("Cannot Count Max Command Count List reached\n");
		return pdFALSE;

	}
    
    return xReturn;
}

/* Command Processing Function */
BaseType_t CLIProcessCommand(const char * const pcCommandInput, char * pcWriteBuffer, size_t xWriteBufferLen) { 
    BaseType_t xReturn = pdPASS;
    const char *pcRegisteredCommandString = NULL;
    const char *pcRegisteredCommandhelpString = NULL;
    BaseType_t xCommandStringLength;

    
    #ifdef ARRAY_BASED_COMMAND_REGISTER
    static int commandIndex = 0;
	
    if(commandIndex == 0) {//just for help command 

        for (commandIndex = 0; commandIndex < commandCount; commandIndex++) 
        {
            pcRegisteredCommandString = commandArray[commandIndex].pcCommand;
	        xCommandStringLength = strlen( pcRegisteredCommandString );
	        
	        /* To ensure the string lengths match exactly, so as not to pick up
	        a sub-string of a longer command, check the byte after the expected
	        end of the string is either the end of the string or a space before
	   	    a parameter. */
		    if( ( pcCommandInput[ xCommandStringLength ] == ' ' ) || ( pcCommandInput[ xCommandStringLength ] == 0x00 ) )
		    {
	            if (strncmp(pcCommandInput, commandArray[commandIndex].pcCommand, strlen(commandArray[commandIndex].pcCommand)) == 0) //("help","help",4)
	            {
	                printf("VAMSI : Command received - %s\n",pcRegisteredCommandString); //doubt
	               /* The command has been found.  Check it has the expected
			      number of parameters.  If cExpectedNumberOfParameters is -1,
				  then there could be a variable number of parameters and no
				  check is made. */
			       if( commandArray[commandIndex].cExpectedNumberOfParameters >= 0 )
				       {
				       if( prvGetNumberOfParameters( pcCommandInput ) != commandArray[commandIndex].cExpectedNumberOfParameters )
					   {
					       xReturn = pdFALSE;
					   } else {
					       printf("VAMSI : Number of parameters are - %d\n",commandArray[commandIndex].cExpectedNumberOfParameters);
					   }
				   }
                   break;
                }
            }
        }
	}

    if( xReturn == pdFALSE ) //this is for everything else except help
	{
		/* The command was found, but the number of parameters with the command
		was incorrect. */
		strncpy( pcWriteBuffer, "Incorrect command parameter(s).  Enter \"help\" to view a list of available commands.\r\n\r\n", xWriteBufferLen );
		printf("%s\n",pcWriteBuffer);
	}   
	else if( ( xReturn == pdPASS ) && ( commandIndex < commandCount) )
	{
	    printf("commandIndex is %d\n",commandIndex);
	    /* Call the callback function that is registered to this command. */
		xReturn = commandArray[commandIndex].pxCommandInterpreter( pcWriteBuffer, xWriteBufferLen, pcCommandInput);

		/* If xReturn is pdFALSE, then no further strings will be returned
		after this one */
		if( xReturn == pdFALSE )
		{
			commandIndex = 0;
		}
	}
	else
	{
		strncpy( pcWriteBuffer, "Command not recognised.  Enter 'help' to view a list of available commands.\r\n\r\n", xWriteBufferLen );
		xReturn = pdFALSE;
		printf("%s\n",pcWriteBuffer);
	}
    
    #else /* Linked list based command registry logic from here */
    
    static const CLI_Definition_List_Item_t *pxCommand = NULL; // creating an pointer to the instance of CLI List 
    
	if( pxCommand == NULL )
	{
		/* Search for the command string in the list of registered commands. */
		for( pxCommand = &xRegisteredCommands; pxCommand != NULL; pxCommand = pxCommand->pxNext )
		{
			pcRegisteredCommandString = pxCommand->pxCommandLineDefinition->pcCommand;
			pcRegisteredCommandhelpString = pxCommand->pxCommandLineDefinition->pcHelpString;
			xCommandStringLength = strlen( pcRegisteredCommandString );
		
			/* To ensure the string lengths match exactly, so as not to pick up
			a sub-string of a longer command, check the byte after the expected
			end of the string is either the end of the string or a space before
			a parameter. */
			if( ( pcCommandInput[ xCommandStringLength ] == ' ' ) || ( pcCommandInput[ xCommandStringLength ] == 0x00 ) )
			{
			   	if( strncmp( pcCommandInput, pcRegisteredCommandString, xCommandStringLength ) == 0 )
				{
				    printf("The Command received is - %s\n",pcRegisteredCommandString);
					/* The command has been found.  Check it has the expected
					number of parameters.  If cExpectedNumberOfParameters is -1,
					then there could be a variable number of parameters and no
					check is made. */
					if( pxCommand->pxCommandLineDefinition->cExpectedNumberOfParameters >= 0 )
					{
					    if( prvGetNumberOfParameters( pcCommandInput ) != pxCommand->pxCommandLineDefinition->cExpectedNumberOfParameters )
						{
						    xReturn = pdFALSE;
						} else {
						    printf("The Number of parameters are - %d\n",pxCommand->pxCommandLineDefinition->cExpectedNumberOfParameters);
						}
					}
					break;
				}
			}
		}
	}

	if( ( pxCommand != NULL ) && ( xReturn == pdFALSE ) )
	{
		/* The command was found, but the number of parameters with the command
		was incorrect. */
		strncpy( pcWriteBuffer, "Incorrect command parameter(s).  Enter \"help\" to view a list of available commands.\r\n\r\n", xWriteBufferLen );
		pxCommand = NULL;
		printf("%s\n",pcWriteBuffer);
	}
	else if( pxCommand != NULL )
	{
	   	/* Call the callback function that is registered to this command. */
		xReturn = pxCommand->pxCommandLineDefinition->pxCommandInterpreter( pcWriteBuffer, xWriteBufferLen, pcCommandInput );

		/* If xReturn is pdFALSE, then no further strings will be returned
		after this one, and	pxCommand can be reset to NULL ready to search
		for the next entered command. */
		if( xReturn == pdFALSE )
		{
			pxCommand = NULL;
		}
	}
	else
	{
	    /* pxCommand was NULL, the command was not found. */
		strncpy( pcWriteBuffer, "Command not recognised.  Enter 'help' to view a list of available commands.\r\n\r\n", xWriteBufferLen );
		xReturn = pdFALSE;
		printf("%s\n",pcWriteBuffer);
	}
    
    #endif
    
    //new
    pxCommand = NULL;
  
    

    return xReturn;
}


/* Help Command Implementation */
BaseType_t prvHelpCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {
    const char *pcRegisteredCommandString = NULL;
    BaseType_t xCommandStringLength;
    memset(pcWriteBuffer, 0, xWriteBufferLen);
    
    #ifdef ARRAY_BASED_COMMAND_REGISTER
    BaseType_t commandIndex = 0;
    for (commandIndex = 0; commandIndex < commandCount; commandIndex++) 
    {
        if(commandIndex == 0) continue;
        pcRegisteredCommandString = commandArray[commandIndex].pcCommand;
        xCommandStringLength = strlen( pcRegisteredCommandString );
        
        if(commandIndex == 1) {
           strncpy(pcWriteBuffer, pcRegisteredCommandString, xCommandStringLength);
        } else {
            strcat(pcWriteBuffer,"\r\n"); 
            strcat(pcWriteBuffer,pcRegisteredCommandString); 
        }
    }
    printf("Here are the List of available commands  - %s%s","\n",pcWriteBuffer);
    #else
    const CLI_Definition_List_Item_t *pxCommand = &xRegisteredCommands;
    pxCommand = pxCommand->pxNext->pxNext;
    for( pxCommand ; pxCommand != NULL; pxCommand = pxCommand->pxNext )
    {
        pcRegisteredCommandString = pxCommand->pxCommandLineDefinition->pcCommand;
        xCommandStringLength = strlen( pcRegisteredCommandString );
        
        if(strlen(pcWriteBuffer) == 0) {
        strncpy(pcWriteBuffer, pcRegisteredCommandString, xCommandStringLength);
        } else {
            strcat(pcWriteBuffer,"\r\n"); 
            strcat(pcWriteBuffer,pcRegisteredCommandString); 
        }
    }
     printf("Here are the List of available commands - %s%s\n","\n",pcWriteBuffer);
    #endif 
}

/* Set Command Implementation */
//this command basically clears the write buffer, then takes the first and 2nd parameters from the string and store it into write buffer

BaseType_t prvSetCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {
    char * pcParameter;
    BaseType_t lParameterStringLength = 0;
    
    /* Start with an empty string. */ 
	//first parameter
    memset(pcWriteBuffer, 0, xWriteBufferLen);
    pcParameter = ( char * ) FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParameterStringLength);
    strncpy(pcWriteBuffer, pcParameter, lParameterStringLength);
    printf("VAMSI : Param 1- %s\n",pcWriteBuffer);
    
    /* Start with an empty string. */ 
	//second parameter
    memset(pcWriteBuffer, 0, xWriteBufferLen);
    pcParameter = ( char * ) FreeRTOS_CLIGetParameter(pcCommandString, 2, &lParameterStringLength);
    strncpy(pcWriteBuffer, pcParameter, lParameterStringLength);
    printf("VAMSI : Param 2- %s\n",pcWriteBuffer);
}

/* Get Command Implementation */
BaseType_t prvGetCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {
    char * pcParameter;
    BaseType_t lParameterStringLength = 0;
    
    memset(pcWriteBuffer, 0, xWriteBufferLen);
    pcParameter = ( char * ) FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParameterStringLength);//temp
    strncpy(pcWriteBuffer, pcParameter, lParameterStringLength);
    printf("VAMSI : Param 1- %s\n",pcWriteBuffer);
   
    if(strcmp(pcWriteBuffer,"temp")==0){
        memset(pcWriteBuffer, 0, xWriteBufferLen);
        //random number generator
        int min=0;
        int max=100;
        srand(time(NULL));
        volatile int sensor_data=min + rand() % (max - min + 1);
        snprintf(pcWriteBuffer, xWriteBufferLen, "%d", sensor_data);
        printf("VAMSI : Temperature is - %s\n",pcWriteBuffer);
    }
    
   
}


BaseType_t prvGetNumberOfParameters( const char *pcCommandString )
{
BaseType_t cParameters = 0;
BaseType_t xLastCharacterWasSpace = pdFALSE;

	/* Count the number of space delimited words in pcCommandString. */
	while( *pcCommandString != 0x00 )
	{
		if( ( *pcCommandString ) == ' ' )
		{
			if( xLastCharacterWasSpace != pdPASS )// last character shouldn't be a space.
			{
				cParameters++;
				xLastCharacterWasSpace = pdPASS;
			}
		}
		else
		{
			xLastCharacterWasSpace = pdFALSE;
		}

		pcCommandString++;
	}

	/* If the command string ended with spaces, then there will have been too
	many parameters counted. */
	if( xLastCharacterWasSpace == pdPASS )
	{
		cParameters--;
	}

	/* The value returned is one less than the number of space delimited words,
	as the first word should be the command itself. */
	return cParameters;
}

const char *FreeRTOS_CLIGetParameter( const char *pcCommandString, BaseType_t uxWantedParameter, BaseType_t *pxParameterStringLength )
{
BaseType_t uxParametersFound = 0;
const char *pcReturn = NULL;


	*pxParameterStringLength = 0;
	
	while( uxParametersFound < uxWantedParameter ) //2
	{
		/* Index the character pointer past the current word.  If this is the start
		of the command string then the first word is the command itself. */

		//if we take set temp 50, this loop will pass through set word and space
		//while(if string has not ended and if it is not a space )

		while( ( ( *pcCommandString ) != 0x00 ) && ( ( *pcCommandString ) != ' ' ) )//skip the set
		{
		    pcCommandString++;
		}

		/* Find the start of the next string. */     //skipped the space.
		while( ( ( *pcCommandString ) != 0x00 ) && ( ( *pcCommandString ) == ' ' ) )
		{
		    pcCommandString++;
		}
	
		/* Was a string found? */
		if( *pcCommandString != 0x00 ) // if we found new words
		{
			/* Is this the start of the required parameter? */
			uxParametersFound++;

			if( uxParametersFound == uxWantedParameter ) //set temp 50
			{
				/* How long is the parameter? */
				pcReturn = pcCommandString; 
				
				//(while there is a string and there is no space) 
				while( ( ( *pcCommandString ) != 0x00 ) && ( ( *pcCommandString ) != ' ' ) )
				{
					( *pxParameterStringLength )++;
					pcCommandString++; // this section extracts those specific parameter words 
				}
			
				if( *pxParameterStringLength == 0 )
				{
					pcReturn = NULL;
				}

				break;
			}
		}
		else
		{
			break;
		}
	}

	return pcReturn;
}

/*Main Function (For Demonstration) 
int main(void) {



    CLIRegisterCommand(&xHelpCommand);
    CLIRegisterCommand(&xSetCommand);
	CLIRegisterCommand(&xGetCommand);
	
	
	CLIRegisterCommand(&xACommand);
	CLIRegisterCommand(&xBCommand);
	CLIRegisterCommand(&xCCommand);
	CLIRegisterCommand(&xDCommand);
	CLIRegisterCommand(&xECommand);
	CLIRegisterCommand(&xFCommand);
	CLIRegisterCommand(&xGCommand);
	CLIRegisterCommand(&xHCommand);
    

    
    CLIProcessCommand("help", writeBuffer, sizeof(writeBuffer));
	CLIProcessCommand("set temp 50", writeBuffer, sizeof(writeBuffer));
    
    return 0;
}

*/

/* Main Function (For Demonstration) */
int main(void) {
    CLIRegisterCommand(&xHelpCommand); // Register help command initially
     printf("Welcome to Custom CLI\n");
     printf("\n");
     printf("1.Use register command to add a command in the system. for eg :- register set\n");
     printf("2.Enter 'help' to get a list of registered command.\n");
     printf("3.Enter 'help {commandname}' to know more about a registered command. for eg :- help set\n");
     printf("\n");
    
    
    // Shell interface loop
    char userInput[50];
    while (1) {
        printf(">>");
        fgets(userInput, sizeof(userInput), stdin);
        
        
         // Remove newline character if present
        size_t len = strlen(userInput);
            if (len > 0 && userInput[len - 1] == '\n') {
                userInput[len - 1] = '\0';
            }
        
        if (strncmp(userInput, "register", 8) == 0) {
             // Check if user is admin
            printf("Enter admin password: ");
            char adminPassword[20];
            scanf("%s",adminPassword);
            getchar();
            
            if (strcmp(adminPassword, IMPORTANT) != 0) {
                printf("Invalid admin password. Command registration denied.\n");
                continue; // Go back to the beginning of the loop
            }
            
           
            
            // User wants to register a command
            char commandName[20];
            sscanf(userInput, "%*s %s", commandName);
            
            if (strncmp(commandName, "set", 3) == 0) {
                CLIRegisterCommand(&xSetCommand);
                printf("Command 'set' registered successfully.\n");
            } else if (strncmp(commandName, "get", 3) == 0) {
                CLIRegisterCommand(&xGetCommand);
                printf("Command 'get' registered successfully.\n");
            } else {
                printf("Invalid command name. No function registered.\n");
            }
        
        } else if (strncmp(userInput, "help", 4) == 0) {
            
            if(userInput[4]=='\0'){
                CLIProcessCommand(userInput, writeBuffer, sizeof(writeBuffer));
            }
            else{
            // User wants help for a command
            
            char commandName[20];
            sscanf(userInput, "%*s %s", commandName);
            
            // Traverse the linked list to find the command
            const CLI_Definition_List_Item_t *pxCommand = &xRegisteredCommands;
            pxCommand = pxCommand->pxNext;
            while (pxCommand != NULL) {
                if (strncmp(commandName, pxCommand->pxCommandLineDefinition->pcCommand, strlen(pxCommand->pxCommandLineDefinition->pcCommand)) == 0) {
                    // Command found, print its help string
                    printf("Help for '%s' command: %s\n", pxCommand->pxCommandLineDefinition->pcCommand, pxCommand->pxCommandLineDefinition->pcHelpString);
                    break;
                }
                pxCommand = pxCommand->pxNext;
            }
        
            // If command not found in the list
            if (pxCommand == NULL) {
                printf("Invalid command name. No help available.\n");
            }
            
            
            
            /*
            if (strncmp(commandName, "set", 3) == 0) {
                printf("Help for 'set' command: %s\n", xSetCommand.pcHelpString);
            } else if (strncmp(commandName, "get", 3) == 0) {
                printf("Help for 'get' command: %s\n", xGetCommand.pcHelpString);
            } else {
                printf("Invalid command name. No help available.\n");
            }
            */
                
            }
        } else {
            // Process other commands
            CLIProcessCommand(userInput, writeBuffer, sizeof(writeBuffer));
        }
    }

    return 0;
}