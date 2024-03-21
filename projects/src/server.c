#include "open62541.h"

#include <signal.h>
#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

int show_ip(const char *adapter)
{
	int fd;
	struct ifreq ifr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	/* I want to get an IPv4 IP address */
	ifr.ifr_addr.sa_family = AF_INET;

	/* I want IP address attached to "eth0" */
	strncpy(ifr.ifr_name, adapter, IFNAMSIZ-1);

	if( ioctl(fd, SIOCGIFADDR, &ifr) < 0 )
	{
		printf("%s is not available\n", adapter);
	}
	else
	{
		printf("Server IP (this board) : %s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
	}

	close(fd);
}

UA_Boolean running = true;

static void stopHandler(int sign) 
{
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-a");
	running = false;
}

static void addVariable(UA_Server *server)
{
	UA_VariableAttributes attr = UA_VariableAttributes_default;
	UA_Int32 myInteger = 42;

	UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
	
	attr.description = UA_LOCALIZEDTEXT("en-us", "the answer");
	attr.displayName = UA_LOCALIZEDTEXT("en-us", "the answer");
	attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
	attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	
	UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "the.answer");
	UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, "the answer");
	UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
	UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	UA_Server_addVariableNode(server, myIntegerNodeId, parentNodeId,
		parentReferenceNodeId, myIntegerName,
		UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
		attr, NULL, NULL);
}

int main(int argc, char **argv)
{
	show_ip("eth0");

	signal(SIGINT, stopHandler);
	signal(SIGTERM, stopHandler);
	
	UA_Server *server = UA_Server_new();
	UA_ServerConfig_setDefault(UA_Server_getConfig(server));
	
	addVariable(server);
	
	UA_StatusCode retval = UA_Server_run(server, &running);
	UA_Server_delete(server);
	
	return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}

// end of file

