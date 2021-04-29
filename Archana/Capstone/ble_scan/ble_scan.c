#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <time.h>
#include <string.h>

#include "gattlib.h"

#define BLE_SCAN_TIMEOUT   4

typedef void (*ble_discovered_device_t)(const char* addr, const char* name);

// We use a mutex to make the BLE connections synchronous
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

LIST_HEAD(listhead, connection_t) g_ble_connections;
struct connection_t {
	pthread_t thread;
	char* addr;
	LIST_ENTRY(connection_t) entries;
};

// function to create a substring from a string
void substring(char s[], char sub[], int p, int l){
	int c = 0;
	while ( c < l) {
		sub[c] = s[p+c-1];
		c++;
	}
	sub[c] = '\0';
}
  
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
  
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

 void *ble_connect_device(void *arg) {
	struct connection_t *connection = arg;
	char* addr = connection->addr;
	gatt_connection_t* gatt_connection;
	gattlib_primary_service_t* services;
	gattlib_characteristic_t* characteristics;
	int services_count, characteristics_count;
	char uuid_str[MAX_LEN_UUID_STR + 1];
	int ret, i;
	size_t len;

	pthread_mutex_lock(&g_mutex);

	printf("------------START %s ---------------\n", addr);

	gatt_connection = gattlib_connect(NULL, addr, GATTLIB_CONNECTION_OPTIONS_LEGACY_DEFAULT);
	if (gatt_connection == NULL) {
		fprintf(stderr, "Failed to connect to the bluetooth device.\n");
		goto connection_exit;
	} else {
		puts("Succeeded to connect to the bluetooth device.");
	}
/*
	ret = gattlib_discover_primary(gatt_connection, &services, &services_count);
	if (ret != 0) {
		fprintf(stderr, "Fail to discover primary services.\n");
		goto disconnect_exit;
	}

	for (i = 0; i < services_count; i++) {
		gattlib_uuid_to_string(&services[i].uuid, uuid_str, sizeof(uuid_str));

		printf("service[%d] start_handle:%02x end_handle:%02x uuid:%s\n", i,
				services[i].attr_handle_start, services[i].attr_handle_end,
				uuid_str);
	}
	free(services);
*/
	ret = gattlib_discover_char(gatt_connection, &characteristics, &characteristics_count);
	if (ret != 0) {
		fprintf(stderr, "Fail to discover characteristics.\n");
		goto disconnect_exit;
	}
	for (i = 0; i < characteristics_count; i++) {
		gattlib_uuid_to_string(&characteristics[i].uuid, uuid_str, sizeof(uuid_str));
		if(strcmp(uuid_str, "6e400003-b5a3-f393-e0a9-e50e24dcca9e") == 0)
		{

			printf("characteristic[%d] properties:%02x value_handle:%04x uuid:%s\n", i,
				characteristics[i].properties, characteristics[i].value_handle,
				uuid_str);
			//delay(1000);
			// uint8_t *buffer = NULL;
			//uint8_t* buffer = (uint8_t*) malloc(20);
			float temp;
			char first_string[5];
			char second_string[15];
                         


			//for(unsigned int j =0; j!=100; ++j)
			//while(1)
			//{

			//delay(1000);
			//uint8_t* buffer = (uint8_t*) malloc(30);
			uint8_t* buffer = NULL;	
			do{
				ret = gattlib_read_char_by_uuid(gatt_connection, &characteristics[i].uuid, (void **)&buffer, &len);
				
			}while(buffer[0] == '\0');

                	if (ret != GATTLIB_SUCCESS) {

                       		 if (ret == GATTLIB_NOT_FOUND) {
                                	fprintf(stderr, "Could not find GATT Characteristic with UUID %s. "
                                        		"You might call the program with '--gatt-discovery'.\n", uuid_str);
                        	} else {
                                	fprintf(stderr, "Error while reading GATT Characteristic with UUID %s (ret:%d)\n", uuid_str, ret);
                        		free(buffer);
				}
                        	goto disconnect_exit;
			
                	}
			//printf("Read UUID completed: ");
                	// printf("Temperature in *C: ");
			//int l = strlen(buffer);
			substring(buffer, first_string, 1, 2);

			substring(buffer, second_string, 3, len-4);
			FILE* t_ptr = fopen("temperature.txt", "a");
			FILE* a_ptr = fopen("audio.txt", "a");
                        if ((t_ptr == NULL) || (a_ptr == NULL))
                        {
                                printf("\nError creating file!!");
                                exit(1);
                        }
			//temp = atof(second_string);
			if( strcmp(first_string, "T ") == 0)
                        {
                                 temp = atof(second_string);
				 fprintf(t_ptr, "%f\n", temp);
				 printf("%s", first_string);
				 printf("%f", temp);
                        } 
			else if ( strcmp(first_string, "A ") == 0)
			{
                                 fprintf(a_ptr, "%s\n", second_string);
	                         //printf("%s", buffer);
			}
			fclose(a_ptr);
			fclose(t_ptr);
			//for (i = 0; i < len; ++i) 
                         //	printf(buffer);
			//printf("\nLength of buffer : %d\n", strlen(buffer)); 
			//for( i = 0; i < len; ++i)
			//printf("%c ", buffer);
                	//}
                	printf("\n");

                	free(buffer);
			delay(100);// 200 ms seconds delay
			
			//}
		}	
	}
	free(characteristics);

disconnect_exit:
	gattlib_disconnect(gatt_connection);

connection_exit:
	printf("------------DONE %s ---------------\n", addr);
	pthread_mutex_unlock(&g_mutex);
	return NULL;
}

 void ble_discovered_device(void *adapter, const char* addr, const char* name, void *user_data) {
	struct connection_t *connection;
	int ret;

	if (name) {
		printf("Discovered %s - '%s'\n", addr, name);
	} else {
		printf("Discovered %s\n", addr);
	}
	if(strcmp(addr, "C2:87:6C:41:3C:E1") == 0)
	{
		connection = malloc(sizeof(struct connection_t));
		if (connection == NULL) {
			fprintf(stderr, "Fail to allocate connection.\n");
			return;
		}
		connection->addr = strdup(addr);

		ret = pthread_create(&connection->thread, NULL,	ble_connect_device, connection);
		if (ret != 0) {
			fprintf(stderr, "Fail to create BLE connection thread.\n");
			free(connection);
			return;
		}
		LIST_INSERT_HEAD(&g_ble_connections, connection, entries);
	}
}

int main(int argc, const char *argv[]) {
	remove("temperature.txt");
	const char* adapter_name;
	void* adapter;
	int ret;

	if (argc == 1) {
		adapter_name = NULL;
	} else if (argc == 2) {
		adapter_name = argv[1];
	} else {
		fprintf(stderr, "%s [<bluetooth-adapter>]\n", argv[0]);
		return 1;
	}
	for(int i=0; i< 50; ++i)
	{
	LIST_INIT(&g_ble_connections);

	ret = gattlib_adapter_open(adapter_name, &adapter);
	if (ret) {
		fprintf(stderr, "ERROR: Failed to open adapter.\n");
		return 1;
	}

	pthread_mutex_lock(&g_mutex);
	ret = gattlib_adapter_scan_enable(adapter, ble_discovered_device, BLE_SCAN_TIMEOUT, NULL /* user_data */);
	if (ret) {
		fprintf(stderr, "ERROR: Failed to scan.\n");
		goto EXIT;
	}

	gattlib_adapter_scan_disable(adapter);

	puts("Scan completed");
	pthread_mutex_unlock(&g_mutex);

	// Wait for the thread to complete
	while (g_ble_connections.lh_first != NULL) {
		struct connection_t* connection = g_ble_connections.lh_first;
		pthread_join(connection->thread, NULL);
		LIST_REMOVE(g_ble_connections.lh_first, entries);
		free(connection->addr);
		free(connection);
	}

EXIT:
	gattlib_adapter_close(adapter);
	return ret;
	
	delay(200);
	} // end of foor loop

}
