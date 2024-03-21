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
	
	const UA_NodeId nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
	retval = UA_Client_readValueAttribute(client, nodeId, &value);
	
	if( retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_DATETIME]))
	{
		UA_DateTime raw_date = *(UA_DateTime*)value.data;
		UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date);
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "date is: %u-%u-%u %u:%u:%u.%03u\n",
			dts.day, dts.month, dts.year, dts.hour, dts.min, dts.sec, dts.milliSec);
	}
	
	UA_Variant_clear(&value);
	UA_Client_delete(client);
	
	return EXIT_SUCCESS;
}

// end of file

