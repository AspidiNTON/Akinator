#ifndef DUMP_H
#define DUMP_H

#include <stdio.h>

#include "tree.h"

bool initializeLogger();

void closeLogger();

bool dumpTree(Node* node);

#endif