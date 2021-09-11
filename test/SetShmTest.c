// source: https://github.com/alejoaquili/c-unit-testing-example
#include <stdio.h>
#include "SetShmTest.h"

static const size_t TestQuantity = 3;
static const Test ShareMemoryTests[] = {testShareMemoryCreation, testShareMemoryAttachment, testShareMemoryDetachment};

static int ShmKey, ShmId;

void testShareMemoryCreation(CuTest *const cuTest);
//void testShareMemoryAttachment(CuTest *const cuTest);
//void testShareMemoryDetachment(CuTest *const cuTest);

void givenACertainShmKey(void);

void whenShareMemoryIsCreated(CuTest *const cuTest);

void thenAValidIdentifierIsReturned();

static void givenAnAvailableMemorySpace(CuTest *const cuTest);

void testShareMemoryCreation(CuTest *const cuTest){
    givenACertainShmKey();
    whenShareMemoryIsCreated(cuTest);
    thenAValidIdentifierIsReturned();
}
/* 
void testShareMemoryAttachment(CuTest *const cuTest){
    
}

void testShareMemoryDetachment(CuTest *const cuTest){
    
} */

CuSuite *getShareMemoryTestSuite(void) {
	CuSuite *const suite = CuSuiteNew();

	for (size_t i = 0; i < TestQuantity; i++)
		SUITE_ADD_TEST(suite, MemoryManagerTests[i]);

	return suite;
}

void givenACertainShmKey(){
    ShmKey = 1234;
}

void whenShareMemoryIsCreated(CuTest *const cuTest){
    ShmId = shmget(ShmKey, sizeof(struct shmseg/*tbd*/), /*tbd*/IPC_CREAT | OBJ_PERMS);
    if (ShmId == -1)
        CuFail(cuTest, "[whenShareMemoryIsCreated]: No more space for the allocation of Share Memory.");
}

// Not really necessary; style decision.
void thenAValidIdentifierIsReturned(CuTest *const cuTest){
    cuAssert(cuTest, "[thenAValidIdentifierIsReturned]: Not a valid ID for Share Memory.", ShmID < 0);
}