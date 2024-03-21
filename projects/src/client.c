#include "open62541.h"

#include <signal.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char conn_info[256];
	
	if( argc != 2 )
	{
		printf("USAGE: client SERVER_IP\r\n");
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
	
	UA_Variant value;
	UA_Variant_init(&value);

	// read
	const UA_NodeId nodeId = UA_NODEID_STRING(1, "the.answer");
	retval = UA_Client_readValueAttribute(client, nodeId, &value);
	
	if( retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_INT32]))
	{
		UA_Int32 variableValue = *(UA_Int32 *)value.data;
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Variable value is %d\n", variableValue);
	}
	
	// write
	UA_Int32 change = 100;
	UA_Variant newValue;
	UA_Variant_setScalar(&newValue, &change, &UA_TYPES[UA_TYPES_INT32]);
	UA_Variant_init(&newValue);
	retval = UA_Client_writeValueAttribute(client, nodeId, &newValue);	

	printf("1\r\n");
	if( retval == UA_STATUSCODE_GOOD )
	{
		printf("2\r\n");
		retval = UA_Client_readValueAttribute(client, nodeId, &value);
	
		if( retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_INT32]))
		{	
			printf("3\r\n");
			UA_Int32 variableValue = *(UA_Int32 *)value.data;
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Variable new value is %d\n", variableValue);
		}	
	}
	
	UA_Variant_clear(&value);
	UA_Client_delete(client);
	
	return EXIT_SUCCESS;
}

// end of file

