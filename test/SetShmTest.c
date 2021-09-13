// source: https://github.com/alejoaquili/c-unit-testing-example
#include <stdio.h>
#include "SetShmTest.h"
#include "include/shmADT.h"
#include <errno.h>
#define SEM_NAME "nametest"

static const size_t TestQuantity = 3;
static const Test ShareMemoryTests[] = {testShareMemoryCreation, testCloseShareMemory};

static char * ShmName;
static shmADT toBeTestedProducer;
static shmADT toBeTesedConsumer;

void testShareMemoryCreation(CuTest *const cuTest);
//void testCloseShareMemory(CuTest *const cuTest);

void givenACertainShmName(void);

void whenShareMemoryIsCreated(CuTest *const cuTest);
void whenClosingShareMemory(CuTest *const cuTest);

void thenAValidFdIsReturned();

static void givenAnAvailableMemorySpace(CuTest *const cuTest);

void testShareMemoryCreation(CuTest *const cuTest){
    givenACertainShmName();
    whenShareMemoryIsCreated(cuTest);
    thenAValidFdIsReturned();
}

/* 
void testCloseShareMemory(CuTest *const cuTest){
    whenClosingShareMemory(cuTest);
}
*/

CuSuite *getShareMemoryTestSuite(void) {
	CuSuite *const suite = CuSuiteNew();

	for (size_t i = 0; i < TestQuantity; i++)
		SUITE_ADD_TEST(suite, ShareMemoryTests[i]);

	return suite;
}

void givenACertainShmName(){
    ShmName = "MyShm";
}

void whenShareMemoryIsCreated(CuTest *const cuTest){
    toBeTestedProducer = newShm(ShmName, SEM_NAME, O_RDWR, 0666)
    cuAssertTrue(cuTest, "[whenShareMemoryIsCreated]: Couldn't create a Shm without flag O_CREAT.", toBeTestedProducer == NULL);
    toBeTestedProducer = newShm(ShmName, SEM_NAME, O_CREAT | O_RDONLY, 0666)
    cuAssert(cuTest, "[whenShareMemoryIsCreated]: wrong arguments for ftruncate.", toBeTestedProducer == NULL);
    toBeTestedProducer = newShm(ShmName, SEM_NAME, O_CREAT | O_RDWR, 0666)
    cuAsset(cuTest, "[whenShareMemoryIsCreated]: Not enough space to creat Share Memory.", errno == ENOMEM);
    toBeTestedConsumer = newShm(ShmName, SEM_NAME, O_RDWR, 0666);
}

void thenAValidFdIsReturned(){
    
}

// Not really necessary; style decision.
void thenAValidFdIsReturned(CuTest *const cuTest){
    
}