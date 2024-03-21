#include <stdlib.h>
#include "open62541.h"

int main(int argc, char **argv)
{
	char conn_info[256];
	
	if (argc != 2 )
	{
		printf("USAGE: simple_clint SERVER_IP\r\n");
		return EXIT_FAILURE;
	}
	
	sprintf(conn_info, "opc.tcp://%s:4840", argv[1]);
	
	UA_Client *client = UA_Client_new();
	UA_ClientConfig_setDefault(UA_Client_getConfig(client));
	UA_StatusCode retval = UA_Client_connect(client, conn_info);
	
	if( retval != UA_STATUSCODE_GOOD )
	{
		UA_Client_delete(client);
		return (int)retval;
	}
	
	UA_Variant input;
	UA_String argString = UA_STRING("Patrick");
	UA_Variant_init(&input);
	UA_Variant_setScalarCopy(&input, &argString, &UA_TYPES[UA_TYPES_STRING]);
	
	size_t outputSize;
	UA_Variant *output;
	retval = UA_Client_call(client, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
			UA_NODEID_NUMERIC(1, 62541), 1, &input, &outputSize, &output);
	if( retval == UA_STATUSCODE_GOOD )
	{
		printf("Method call was successful, and %lu returned values available.\r\n",
			(unsigned long)outputSize);
		UA_String outString = *(UA_String*)output->data;
		printf("%lu:%s\r\n", outString.length, outString.data);
		
		UA_Array_delete(output, outputSize, &UA_TYPES[UA_TYPES_VARIANT]);
	}
	else
	{
		printf("Method call was unsuccessful and %x returned values available\r\n", retval);
	}
	
	UA_Variant_clear(&input);
	UA_Client_delete(client);
	
	return EXIT_SUCCESS;
}

// end of file
