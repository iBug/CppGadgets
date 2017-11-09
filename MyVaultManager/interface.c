// File: interface.c
// Author: iBug 

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include "data.h"
#include "file.h"
#include "interface.h"
#include "sys.h"

option_t menuMain(Data** pdata){
	clearScreen();
	puts("******************");
	puts("*                *");
	puts("*  iBug MyVault  *");
	puts("*                *");
	puts("******************");
	puts("");
	if (*pdata == NULL)
		*pdata = readFile("vault.dat");
	if (*pdata == NULL){
		puts("[Info] The default database isn't found. Creating an empty database.\n");
		*pdata = menuNewData();
		if (*pdata == NULL){
			puts("[ERROR] Failed to create default database");
			pause();
			exit(1);
		}
	}
	puts("Choose an action:");
	puts("   1. Display the whole database");
	puts("   2. Query a record");
	puts("   3. Create a new record");
	puts("   4. Change an existing record");
	puts("   5. Delete a record");
	puts("   6. Save to file");
	puts("");
	puts("   0. Quit");
	printf("\nEnter your choice: ");
	option_t option;
	int success = scanf("%d", &option);
	if (!success){
		clearKeyBuffer();
		return OPTION_INVALID;
	}
	if (option < 0){
		clearKeyBuffer();
		return OPTION_INVALID;
	}
	return option;
}

Data* menuNewData(void){
	return createEmptyData();
}

int menuNewRecord(Data* data){
	Date date;
	currency_t amount;
	double amountd;
	char detail[1024];
	printf("\nEnter date:\n");
	printf("    Enter year: "); scanf("%d", &date.year);
	printf("    Enter month: "); scanf("%d", &date.month);
	printf("    Enter day: "); scanf("%d", &date.day);
	printf("Enter amount: "); scanf("%lf", &amountd);
	amount = (currency_t)(amountd * 100.0);
	printf("Enter detail: "); gets(detail); gets(detail);
	Record *record = createRecord(date, amount, detail);
	record->id = insertRecord(data, record);
	puts("\nThis is the new record created:");
	displayRecord(record);
	destroyRecord(record);
	puts("\nPress any key to continue.");
	pause();
	return 0;
}

Record* menuUpdateRecord(Data* data){
	id_t id;
	printf("\nEnter data ID: ");
	scanf("%d", &id);
	Record *oldRecord = queryRecord(data, id);
	if (!oldRecord){
		puts("That record does not exist!");
		puts("\nPress any key to continue.");
		pause();
		return NULL;
	}
	puts("\nThis is the record to be changed:");
	displayRecord(oldRecord);
	Date date;
	currency_t amount;
	double amountd;
	char detail[1024];
	printf("\nEnter new date:\n");
	printf("    Enter year: "); scanf("%d", &date.year);
	printf("    Enter month: "); scanf("%d", &date.month);
	printf("    Enter day: "); scanf("%d", &date.day);
	printf("Enter new amount: "); scanf("%lf", &amountd);
	amount = (currency_t)(amountd * 100.0);
	printf("Enter detail: "); gets(detail); gets(detail);
	Record *record = createRecord(date, amount, detail);
	updateRecord(oldRecord, record);
	destroyRecord(record); 
	puts("\nThis is the updated record:");
	displayRecord(oldRecord);
	puts("\nPress any key to continue.");
	pause();
	return oldRecord;
}

int menuDisplayData(const Data* data){
	clearScreen();
	puts("******  Data dump  ******");
	displayData(data);
	puts("\nPress any key to continue.");
	pause();
	return 0;
}

int menuQueryRecord(Data* data){
	id_t id;
	printf("\nEnter data ID to query: ");
	scanf("%d", &id);
	Record *record = queryRecord(data, id);
	if (!record){
		puts("That record does not exist!");
		puts("\nPress any key to continue.");
		pause();
		return 1;
	}
	puts("This is the record:");
	displayRecord(record);
	puts("\nPress any key to continue.");
	pause();
	return 0;
}

int menuDeleteRecord(Data* data){
	id_t id;
	printf("\nEnter data ID to delete: ");
	scanf("%d", &id);
	Record *record = queryRecord(data, id);
	if (!record){
		puts("That record does not exist!");
		puts("\nPress any key to continue.");
		pause();
		return 1;
	}
	puts("This is the record deleted:");
	displayRecord(record);
	deleteRecord(data, record->id);
	puts("\nPress any key to continue.");
	pause();
	return 0;
}

int menuSaveData(Data* data, const char* filename){
	size_t size = writeFile(filename, data);
	if (size == 0){
		puts("Error saving data.");
		pause();
		return 1;
	}
	printf("%lu bytes successfully written to %s\n", size, filename);
	pause();
	return 0;
}

int displayRecord(const Record* record){
	printf("ID:     %10d\n", record->id);
	printf("Date:   %4d/%2d/%2d\n", record->date.year, record->date.month, record->date.day);
	printf("Amount: %+7d.%02d\n", record->amount/100, record->amount%100); 
	printf("Detail: %s\n", record->detail);
}

int displayData(const Data* data){
	if (!data) return 0;
	printf("There are %d records in the database.\n", data->count);
	for (int i = 0; i < data->count; i ++){
		puts("");
		displayRecord(data->records[i]);
	}
}