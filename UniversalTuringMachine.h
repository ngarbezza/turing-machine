#include "SimpleTuringMachine.h"

// crea una maquina universal y la deja lista para computar
TuringMachine newUTM();
// construye la cinta para la maquina universal
Tape mkTape(TuringMachine tm, Tape t);
// transforma una cinta de maquina comun en una cinta apropiada
// para la maquina Universal y viceversa
Tape commonTape2UTMTape(Tape t);
Tape UTMTape2CommonTape(Tape t);
// corre la maquina universal
void runUTM(TuringMachine utm, TuringMachine tm, Tape t);
