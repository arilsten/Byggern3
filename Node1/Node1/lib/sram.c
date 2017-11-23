/*
 * sram.c
 *
 * Created: 27.09.2017 17:21:10
 *  Author: eddas
 */
 
#include "sram.h"

volatile char *SRAM_ptr = (char *) 0x1800;
volatile uint16_t sram_size= 0x800;

void SRAM_test(void){
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size= 0x800;
	uint16_t write_errors	= 0;
	uint16_t retrieval_errors	= 0;
	printf("Starting SRAM test...\n");
	// rand() stores someinternal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before thisfunction)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t ig = 0; ig < ext_ram_size; ig++) {
		uint8_t some_value = rand();
		ext_ram[ig] = some_value;
		uint8_t retreived_value = ext_ram[ig];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", ig,retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the writephase
	srand(seed);
	// reset the PRNG to the stateit had before the write phase
	for (uint16_t ig = 0; ig < ext_ram_size; ig++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[ig];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n",ig, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with\n%4d errors in write phase and\n%4d error sin retrieval phase\n\n", write_errors, retrieval_errors);
}

char sram_getInfo(uint16_t address){
	if (address >= sram_size || address < 0){
		return -1;
	}
	return SRAM_ptr[address];
}

void sram_setInfo(uint16_t address, uint8_t val){
	if (0 <= address && address < sram_size){
		SRAM_ptr[address]=val;
	}	
}

void sram_getName(uint16_t address,char* name){
	for (int i = 0;i<length_of_name;i++){
		name[i]=sram_getInfo(address+i);
	}
	
}

int sram_getScore(uint16_t address){
	return (sram_getInfo(address));
}

void sram_setName(uint16_t address,char* name){
	for (int i = 0;i<length_of_name;i++){
		sram_setInfo(address+i,name[i]);
	}
}

void sram_setScore(uint16_t address,int score){
	return (sram_setInfo(address,score));
}