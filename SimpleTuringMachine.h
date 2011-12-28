#include "Prelude.h"

#define State int
#define Dir int
#define L 0
#define R 1

#define Symbol int
#define X 2
#define A 3
#define B 4
#define C 5

struct tNode{               /** Lista doblemente enlazada   */
    struct tNode*   prev;
    ELEM_TYPE       value;
    struct tNode*   next;
};

struct tHeader{             /** La cinta    */
    struct tNode* first;
    struct tNode* current;
    struct tNode* last;
};
typedef struct tHeader* Tape;

/** Operaciones sobre la cinta  */
Tape NilTape();                         // crea una cinta vacia
void cons(ELEM_TYPE x, Tape t);         // agregar al inicio de la cinta
void snoc(Tape t, ELEM_TYPE x);         // agregar al final de la cinta
void append(Tape t1, Tape t2);          // unir 2 cintas
void snocN(Tape t, int n, ELEM_TYPE x); // agrega n elementos iguales al final
void moveTape(Tape t, Dir d);           // realiza el movimiento hacia izquierda o
                                        // derecha, y hace la cinta "infinita"
// Posiciona el puntero current de la cinta en la 'n' esima
// ocurrencia del simbolo 's' (comenzando de izq a der)
void positionCurrent(Tape t, int n, Symbol s);

void printTape(Tape t);             // para imprimir cintas
void printSymbol(Symbol x);         // para imprimir simbolos

/** Las transiciones  */
struct change{
    Symbol  writeSymbol;            // el simbolo a escribir
    Dir     move;                   // el movimiento hacia izq / der
    State   newState;               // el estado nuevo
};
typedef struct change* Change;

/** Estructura de la Maquina de Turing  */
struct TM{
    Change**    data;               // todas las transiciones
    State       currentState;       // el estado actual / inicial
    int         states;             // cantidad de estados
    int         symbols;            // cantidad de simbolos
};
typedef struct TM* TuringMachine;
// la cantidad de estados y de simbolos son usados para la
// implementacion de la maquina universal, en las funciones
// machine2tape() y mkTape()

/** Operaciones de la Maquina de Turing  */
// para construir transiciones
Change newChange(Symbol newS, Dir move, State newSt);
// para construir Maquinas de Turing
TuringMachine newTM(int cantSymbols, int cantStates, State init);
// para agregar transiciones
void addTR(TuringMachine tm, State st, Symbol s, Symbol newS, Dir move, State newSt);
// para agregar finalizaciones
void addHalt(TuringMachine tm, State st, Symbol s);
// para iniciar el computo de la maquina
void run(TuringMachine tm, Tape t);
void compute(TuringMachine tm, Tape t);
// operacion para controlar el fin del computo
Bool halt(TuringMachine tm, Symbol s);
// codificar una maquina de Turing en una cinta
Tape machine2Tape(TuringMachine tm);
