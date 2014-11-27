//TI3 - Uebung 05
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

#include "mm.h"
#include <stdio.h>

//Groesse des Speichers, den die Speicherverwaltung verwalten soll
#define memorySize 10240

//Zustand von einem Speicherblock
enum memBlockState{not_allocated=0, allocated=1};
//zur spaeteren Ausgabe..
char boolStr[2][6] = {{"FALSE"},{"TRUE"}};

//ein Speicherblock
typedef struct _memoryblock {
	void* data;			//Zeiger auf die Daten
	int dataLength;			//Laenge des Blocks
	enum memBlockState state;	//Wird der Block benutzt?
	struct _memoryblock * nextBlock; //Zeiger auf naechsten Speicherblock
} memoryBlock;

#define memoryBlockHeaderSize sizeof(memoryBlock)

//definition von "privaten" funktionen
void mergeFreeBlocks();
memoryBlock* splitBlock(memoryBlock* block, int byteCount);

//Erster Speicherblock in der Liste
memoryBlock* head;
//Unser Speicher, den wir fuer die Daten und deren Verwaltung benutzen koennen
char memory[memorySize];

//Ist unsere Speicherverwaltung initialisiert und bereit benutzt zu werden?
int b_initialized = 0;

//Initialisiere die Datenstruktur(en) fuer unsere Speicherverwaltung
void initialize()
{
	if(!b_initialized)
	{
		//DEBUG
		//printf("Initializing..\n");

		head = (memoryBlock*)memory;
		//Initialisiere head in unserem Speicher <memory>
		//Unser gesamter Speicher <memory> soll als ein memoryBlock interpretiert werden
		//Wir muessen beachten, dass der memoryBlock head auch im Speicher <memory>
		//abgespeichert wird und somit der head auch Speicher der Groesse
		//memoryBlockHeaderSize zur Verwaltung benoetigt.
		//Anfangs ist der restliche Speicher verfuegbar und wird dem head als data zugeordnet.
		// Data beginnt somit nicht am Anfang von <memory>.

		b_initialized = 1;
		//TODO - needs testing - works
		//INITIALISIERUNG VON HEAD
		head->data = head + memoryBlockHeaderSize;
		head->dataLength = memorySize - memoryBlockHeaderSize;
		head->state = not_allocated;
		head->nextBlock = NULL;

		//DEBUG
		//printf("Initialized!");
	}
}

//liefert den ingesamt freien Speicherplatz zurueck
int get_free_space()
{
	if(!b_initialized)
		return -1;
	//DEBUG
	//printf("Start to look for free space..\n");
	int count = 0;
	memoryBlock *block = head;
	while(block != NULL)
	{
		if(block->state == not_allocated)
		{
			count = count + block->dataLength;
		}
		block = block->nextBlock;
	}
	//DEBUG
	//printf("%d free space found!\n",count);
	return(count);
}

//versucht einen Speicherbereich mit <byteCount> vielen Bytes zu reservieren
//Falls dies nicht gelingt wird ein NULL (0) Pointer zurueckgeliefert
void* my_malloc(int byteCount)
{
	if(!b_initialized)
	{
		initialize();
	}
	//DEBUG
	//printf("mAllocating..%d\n",byteCount);
	//Wenn der insgesamt verfuegbare Speicherplatz kleiner ist
	//als der angeforderte, koennen wir gleich aufhoeren!
	if(byteCount > get_free_space())
	{
		return(NULL);
	}
	memoryBlock *block = head;
	//TODO - WIP - seems finished - needs testing - works
	//SUCHE NACH EINEM GEEIGNETEN FREIEN SPEICHERBLOCK, MIT MEHR ALS <byteCount>
	//VIELEN BYTES
	// + memoryBlockHeaderSize ? nvm .. bei einer anforderung die die gesamte blockgroesse benoetigt muss kein header fuer einen splitBlock beruecksichtigt werden
	memoryBlock *return_blockData = NULL;
	while(block != NULL)
	{
		if(block->state == not_allocated){
			if(block->dataLength >= byteCount){
				if(return_blockData == NULL){
					return_blockData = block;
				}else{
					if(return_blockData->dataLength > block->dataLength){
						return_blockData = block;
					}
				}
			}
		}
		block = block->nextBlock;
	}
	// FALLS ES KEIN PASSENDES ELEMENT GIBT, GEBEN WIR NULL ZURUECK.
	if(return_blockData == NULL){
		return NULL;
	}
	// Der Knoten block hat genuegend Speicherplatz
	//if((return_blockData->dataLength) >= (byteCount+memoryBlockHeaderSize)){
		//UNTERTEILUNG DIESES BLOCKS, SO DASS NICHT UNNOETIG VIEL SPEICHERPLATZ VERBRAUCHT WIRD#
		splitBlock(return_blockData,byteCount);
	//}else{
		//ERROR
		//printf("Nicht genug Platz fuer Speicher %d UND header %d in %d",byteCount,memoryBlockHeaderSize,return_blockData->dataLength);

		//return NULL;
	//}
	// UND MARKIERE DIESEN BLOCK
	return_blockData->state = allocated;
	//DEBUG
	//printf("mAllocated!\n");
	//RueCKGABE DES ZEIGERS AUF DEN ZU BENUTZENDEN SPEICHERBEREICH
	return return_blockData->data;
}

//Sofern moeglich teilt die Funktion splitBlock einen Block in 2 Bloecke,
//so dass einer der beiden Bloecke <byteCount> viel Speicherplatz enthaelt.
//Die Funktion liefert denjenigen Block zurueck, der <byteCount> viel Speicherplatz
//bereitstellt und passt die verkette Liste an.
memoryBlock* splitBlock(memoryBlock* block, int byteCount)
{
	// Hinweis: Es ist sinnvoll, wenn <block> die Laenge byteCount hat und
	// der Nachfolger von <block> die Restlaenge umfasst.

	//TODO - WIP - needs testing - works
	//IMPLEMENTIEREN
	//DEBUG
	//printf("splitting block..\n");
	//BERECHNE DIE GROESSE DES NEUEN UND ALTEN BLOCKS
	//-current Block size: block->dataLength
	//-needed Block size: byteCount
	//-new block size: block->dataLength-byteCount
	int newBlock_size = block->dataLength-byteCount-memoryBlockHeaderSize;
	//DEBUG
	//printf("newBlock size: %d\n",newBlock_size);
	//-new block size needs to be > memoryBlockHeaderSize
	//FALLS EIN WEITERER SPEICHERBLOCK IN DEN ALTEN PASST,
	//DEBUG
	//printf("if %d > %d + %d \n",(int)(block->dataLength),(int)byteCount,(int)memoryBlockHeaderSize);
	//if((block->dataLength-byteCount) > memoryBlockHeaderSize){
	//if((int)(block->dataLength) > ((int)byteCount+(int)memoryBlockHeaderSize)){
	if(newBlock_size > 0){
		//ERZEUGEN WIR EINEN NEUEN BLOCK, AENHLICH ZU HEAD AM ANFANG
		//DEBUG
		//printf("new block - %p\n",((memoryBlock*)(block->data)+byteCount+1));
		//printf("new block - %p\n",(memoryBlock*)(block->data+byteCount+1));
		memoryBlock* newBlock = (memoryBlock*)((char*)(block->data)+byteCount+1);
		//DEBUG
		//printf("pointer on data - %p \n",newBlock + memoryBlockHeaderSize);
		newBlock->data 			 	= (void*)((char*)newBlock + memoryBlockHeaderSize);
		//DEBUG
		//printf("dataLength - %d\n",block->dataLength-byteCount-memoryBlockHeaderSize);
		newBlock->dataLength	= block->dataLength-byteCount-memoryBlockHeaderSize;
		//DEBUG
		//printf("state\n");
		newBlock->state				= not_allocated;
		//DEBUG
		//printf("nextBlock pointer\n");
		newBlock->nextBlock		= block->nextBlock;
		//LEGE DEN NEUEN BLOCK ALS NACHFOLGER VOM ALTEN BLOCK FEST
		//DEBUG
		//printf("prev nexB = newB\n");
		block->nextBlock = newBlock;
		// PASSE DIE LAENGE VOM ALTEN BLOCK AN
		block->dataLength = byteCount;

		//DEBUG
		//printf("block splitted\n");
	}else{
		//kein split noetig, wenn kein speicher,da

		//DEBUG
		//printf("no split possible\n");
	}
	return block;
}


//my_free versucht den Speicherbereich, auf den <p> verweist, wieder freizugeben.
//Sofern <p> in unserem Speicher gefunden wird, muessen benachbarte, freie Speicherbloecke
//miteinander verschmolzen werden.
void my_free(void* p)
{
	if(!b_initialized)
	{
		return;
	}
	//TODO - WIP - needs testing - works
	//DEBUG
	//printf("mfree: look for pointer..\n");
	// SUCHE NACH DEM BLOCK MIT ZEIGER <p>
	memoryBlock *block = head;
	while (block != NULL){
		if(block->data == p){
			break;
		}else{
			block = block->nextBlock;
		}
	}
	// FALLS KEINER GEFUNDEN WURDE, GEBE EINE MELDUNG AUS.
	if(block == NULL){
		printf("free: error: Given pointer is no memory block\n");
		return;
	}
	//FREIGEBEN VON DEM ENTSPRECHENDEN SPEICHERBLOCK
	block->state = not_allocated;
	//FREIE SPEICHERBLOECKE MITEINANDER VERSCHMELZEN
	mergeFreeBlocks();
}

//Diese Funktion verschmilzt benachbarte, nicht benutzte Speicherbloecke
void mergeFreeBlocks()
{
	//TODO - WIP - needs testing - tested & works
	//DEBUG
	//printf("merging free blocks..\n");
	// BEGINNE AM ANFANG DER LISTE
	memoryBlock* block = head;
	while(block != NULL && block->nextBlock != NULL){
		// WENN DER AKTUELLE UND DER BENACHBARTE BLOCK FREI SIND,
		if((block->state == not_allocated) && (block->nextBlock->state == not_allocated)){
			//DEBUG
			//printf("block to merge found\n");
			// DANN VERSCHMELZE DIESE INDEM DIE DATENLAENGE
			block->dataLength += (memoryBlockHeaderSize+block->nextBlock->dataLength);
			// UND DER NACHFOLGER VOM AKTUELLEN BLOCK ANGEPASST WERDEN.
			block->nextBlock = block->nextBlock->nextBlock;
			//DEBUG
			//printf("blocks merged\n");
		}else{
			block = block->nextBlock;
		}
	}
	//DEBUG
	//printf("finished merging\n");
}

//Diese Funktion gibt eine Uebersicht ueber die vorhandenen Speicherbloecke aus
void status()
{
	if(!b_initialized)
	{
		initialize();
		b_initialized = 1;
	}
	memoryBlock *block = head;
	int count = 0;
	printf("Uebersicht des Speichers: %d / %d Speicher frei\n", get_free_space(), memorySize);
	printf("------------------------------------------------\n");
	printf("#  at\t\t allocated\t space\t data\t\t\tnext block\n");
	while(block != NULL)
	{
		//TODO: WARNINGS - evtl (void*) pointer casten vor den  memoryBlock pointern? Wer auch immer das Framework erstellt hat, hat wohl ein paar Warnings ignoriert...
		printf("%d  %p\t %s \t\t %d\t [%p,%p]\t%p\n", ++count, /*added cast*/(void*)block, boolStr[block->state], block->dataLength, (block->data), (char*)block->data + block->dataLength-1, /*added void cast*/(void*)(block->nextBlock));
		block = block->nextBlock;
	}
	printf("\n\n");
}
