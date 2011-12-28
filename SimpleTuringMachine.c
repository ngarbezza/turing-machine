#include "SimpleTuringMachine.h"

Tape NilTape() {
    Tape res        = malloc(sizeof(struct tHeader));
    res->first      = NULL;
    res->current    = NULL;
    res->last       = NULL;
    return res;
}

void cons(ELEM_TYPE x, Tape t) {
    struct tNode* newNode = malloc(sizeof(struct tNode));
    newNode->prev   = NULL;                   //el primero no tiene previo
    ELEM_TYPE *p    = x;                      //se crea el puntero al valor
    newNode->value  = p;                      //y se lo asigna en value
    newNode->next   = t->first;               //se engancha con el primero
    t->first        = newNode;                //y ahora el primero es el nuevo nodo
    if(t->last == NULL) t->last = newNode;    //newNode es el primer elemento ingresado
    else newNode->next->prev = newNode;       //ya habia otro/s
}

void snoc(Tape t, ELEM_TYPE x) {
    struct tNode* newNode = malloc(sizeof(struct tNode));
    newNode->prev   = t->last;
    ELEM_TYPE *p    = x;
    newNode->value  = p;
    newNode->next   = NULL;
    t->last         = newNode;
    if(t->first == NULL) t->first = newNode;  //newNode es el primer elemento ingresado
    else newNode->prev->next = newNode;       //ya habia otro/s
}

void append(Tape t1, Tape t2) {
/**
    append: Une las dos cintas recibidas modificando
        t1(la cinta que va a quedar primero).
        t2 conserva su forma, pero ahora el primer elemento tiene
        previo y si se modifica t1, t2 tambien se modificara.
        NO modifica el current de ninguna de las dos cintas.

    Precondicion: Las dos cintas no estan vacias.
*/
    t1->last->next = t2->first;     // engancha el ultimo nodo de t1 con
    t2->first->prev = t1->last;     // el primero de t2 en ambos sentidos
    t1->last = t2->last;            // actualiza el nuevo ultimo
}

void snocN(Tape t, int n, ELEM_TYPE x) {
    int i;
    for(i=0; i<n; i++)
        snoc(t, x);
}

void moveTape(Tape t, Dir d) {
/**
    moveTape: realiza el movimiento de la cinta hacia la
        izquierda o derecha, y en caso de no existir un
        nodo en la direccion deseada, se crea uno con el
        simbolo vacio (0). Esto logra una cinta "infinita".
*/
    if(d){                              // Right es 1 y 1 es True
        if(t->current == t->last)       // si no hay siguiente
            snoc(t, 0);                 // se crea (con el simbolo 0)
        t->current = t->current->next;  // se puede pedir next seguro
    }
    else{                               // Left
        if(t->current == t->first)      // si no hay anterior
            cons(0, t);                 // se crea (con el simbolo 0)
        t->current = t->current->prev;  // se puede pedir prev seguro
    }
}

void positionCurrent(Tape t, int n, Symbol s) {
/**
    positionCurrent: Operacion parcial si no existe
        la 'n'esima ocurrencia de 's'
*/
    Bool finish = False;
    t->current = t->first;                        // iniciar recorrido
    while(not(finish)){
        if((Symbol) t->current->value == s) n--;  // una ocurrencia menos
        if(n == 0) finish = True;                 // ya encontre todas las ocurrencias
        else t->current = t->current->next;       // pasar al siguiente
    }
}

void printTape(Tape t) {
/**
    printTape (observacion):
        printTape no usa el puntero current de su estructura para recorrer
        la cinta ya que este no puede ser alterado pues lo usa la maquina
        de Turing.
*/
    struct tNode* curr = t->first;  // inicia recorrido: 'curr' no es t->current !!
    printf("... ");
    while(not(curr == NULL)){       // mientras haya simbolos en la cinta
        if(curr == t->current){     // destaco la posicion del cabezal
            printf("[");
            printSymbol((Symbol) curr->value);
            printf("]");
        }
        else printSymbol((Symbol) curr->value);
        curr = curr->next;                  //paso al siguiente
        printf(" ");
    }
    printf("...");
}

void printSymbol(Symbol x) {
    switch(x){
    case 2: printf("%c", 'X'); break;
    case 3: printf("%c", 'A'); break;
    case 4: printf("%c", 'B'); break;
    case 5: printf("%c", 'C'); break;
    default: printf("%d", (Symbol) x); break;
    }
}

Change newChange(Symbol s, Dir mov, State st) {
/**
    newChange: Constructor de Change.
    Crea una transicion, que consiste en un simbolo a escribir,
    una direccion adonde moverse y un estado nuevo a ingresar.
*/
    Change c        = malloc(sizeof(struct change));
    c->writeSymbol  = s;
    c->move         = mov;
    c->newState     = st;
    return c;
}

TuringMachine newTM(int cantSymbols, int cantStates, State init) {
/**
    newTM: Constructor de TuringMachine.
    Aloca toda la memoria necesaria, conociendo la cantidad de
    simbolos que va a reconocer y la cantidad de estados que puede tomar.
    Tambien recibe la cinta(lista doble enlazada, el puntero current
    indica inicio de la maquina) y el estado inicial.
*/
    TuringMachine tm = malloc(sizeof(struct TM));
    // filas (array de punteros a otros arrays)
    tm->data = (Change **) malloc(cantStates * sizeof(struct change*));
    int i;
    for(i=0; i<cantStates; i++)
        tm->data[i] = (Change *) malloc(cantSymbols * sizeof(struct change));
    tm->currentState    = init;
    tm->states          = cantStates;
    tm->symbols         = cantSymbols;
    return tm;
}

void addTR(TuringMachine tm, State st, Symbol s, Symbol newS, Dir move, State newSt) {
/**
    addTR: agrega una transicion a la maquina de Turing.
    Precondicion: los simbolos y los estados indicados
        existen en la maquina.
*/
    tm->data[st][s] = newChange(newS, move, newSt);
}

void addHalt(TuringMachine tm, State st, Symbol s) {
/**
    addHalt: indica finalizacion en la maquina de Turing.
    Precondicion: el estado y el simbolo indicados existen en
        la maquina.
    Observaciones: se indica finalizacion con el puntero nulo.
*/
    tm->data[st][s] = NULL;
}

Bool halt(TuringMachine tm, Symbol s) {
/**
    halt: Verifica la condicion de finalizacion
        de la maquina de Turing.
*/
    return (tm->data[tm->currentState][s] == NULL);
}

void run(TuringMachine tm, Tape t) {
    printf("Antes de computar: ");
    printTape(t);
    printf("\n");

    compute(tm, t);

    printf("Despues de computar: ");
    printTape(t);
    printf("\n");
    printf("Estado final: %d\n", tm->currentState);
}

void compute(TuringMachine tm, Tape t) {
/**
    compute: realiza todo el computo de la maquina de Turing.
*/
    Symbol currentSymbol = (Symbol) t->current->value;
    while(not(halt(tm, currentSymbol))){

////         para ver como va computando
//        printTape(t);
//        printf("  Estado: %d", tm->currentState);
//        printf("\n");
//
////        pausa
//        getchar();

        Change tr = tm->data[tm->currentState][currentSymbol];
        t->current->value = tr->writeSymbol;            //escribo nuevo simbolo
        moveTape(t, tr->move);                          //muevo la cinta
        tm->currentState = tr->newState;                //cambio de estado
        //guardo nuevo simbolo para la proxima iteracion
        currentSymbol = (Symbol) t->current->value;
    }
}

Tape machine2Tape(TuringMachine tm) {
/**
    machine2Tape: Codifica una maquina de Turing en una cinta
        para ser usada por una maquina universal.
*/
    int i, j;
    Change curr;
    Tape t = NilTape();
    for(i=0; i<tm->states; i++){
        for(j=0; j<tm->symbols; j++){
            curr = tm->data[i][j];
            if(curr != NULL){                   // los halts se ignoran
                snocN(t, i+1, 1); snoc(t, 0);                   // estado actual
                snocN(t, j+1, 1); snoc(t, 0);                   // simbolo actual
                snocN(t, curr->writeSymbol+1, 1); snoc(t, 0);   // nuevo simbolo
                snocN(t, curr->move+1, 1); snoc(t, 0);          // movimiento
                snocN(t, curr->newState+1, 1); snoc(t, 0);      // nuevo estado
                snoc(t, 0);                                     // delimitador
            }
        }
    }
    return t;
}
