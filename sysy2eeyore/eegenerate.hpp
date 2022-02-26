#pragma once

#include <string>
#include "global.hpp"
#include "eebasic.hpp"

extern int numofdec;
extern int numofend;
extern int numoftmp;
extern int numoflabel;

void Treatmain(PointerOfBaseAST);
void TraverseAST(PointerOfBaseAST);
void DumpEE2file();
