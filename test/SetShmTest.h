#ifndef SET_SHM_TEST_H
#define SET_SHM_TEST_H

#include "cuTest.h"

typedef void (*Test)(CuTest *const cuTest);
CuSuite *getShareMemoryTestSuite();

#endif /*SET_SHM_TEST_H*/