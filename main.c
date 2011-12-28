#include "UniversalTuringMachine.h"

void onesFusion();
void matching();
void copyLeft();
void blockSubstitution();
void testUTM();

int main(){

    //onesFusion();
    //copyLeft();
    //matching();
    //blockSubstitution();
    testUTM();

    return 0;
}

void onesFusion(){
/**
    Ejemplo 1: Fusion de bloques de 1's

    Entrada:    ... [1] 1 1 0 0 0 1 1 ...
    Salida:     ... 0 0 0 1 1 1 [1] 1 ...

    Status: OK
*/
    // creamos la cinta
    Tape t = NilTape();
    snoc(t, 1); snoc(t, 1); snoc(t, 1); snoc(t, 0);
    snoc(t, 0); snoc(t, 0); snoc(t, 1); snoc(t, 1);

    positionCurrent(t, 1, 1);
    TuringMachine tm1 = newTM(2, 4, 0);  // creamos la maquina
    // transiciones del estado Qo
    addHalt(tm1, 0, 0); addTR(tm1, 0, 1, 0, R, 1);
    // estado Q1
    addTR(tm1, 1, 0, 1, R, 2); addTR(tm1, 1, 1, 1, R, 1);
    // estado Q2
    addTR(tm1, 2, 0, 0, L, 3); addHalt(tm1, 2, 1);
    // estado Q3
    addTR(tm1, 3, 0, 0, R, 0); addTR(tm1, 3, 1, 1, L, 3);

    run(tm1, t);                          // corremos la maquina

    // Testing machine2Tape - Debe dar:
    // ... 1 0 1 1 0 1 0 1 1 0 1 1 0 0            (Q0, 1, 0, R, 1)
    // ... 1 1 0 1 0 1 1 0 1 1 0 1 1 1 0 0        (Q1, 0, 1, R, 2)
    // ... 1 1 0 1 1 0 1 1 0 1 1 0 1 1 0 0        (Q1, 1, 1, R, 1)
    // ... 1 1 1 0 1 0 1 0 1 0 1 1 1 1 0 0        (Q2, 0, 0, L, 3)
    // ... 1 1 1 1 0 1 0 1 0 1 1 0 1 0 0          (Q3, 0, 0, R, 0)
    // ... 1 1 1 1 0 1 1 0 1 1 0 1 0 1 1 1 1 0 0  (Q3, 1, 1, L, 3)
    printf("Codificacion de la maquina anterior: ");
    printTape(machine2Tape(tm1));
}

void copyLeft(){
/**
    Ejemplo 2: Copia de un bloque de 1's indicado por B hacia la
        izquierda de la cinta, a partir de la posicion determinada por A.

    Entrada:    ... A 0 0 0 0 0 0 1 1 0 [B] 1 1 1 1 0 0 ...
    Salida:     ... [A] 1 1 1 1 0 0 1 1 0 B 1 1 1 1 0 0 ...

    Precondicion: Hay suficientes vacios siguientes a A para contener
        el bloque de 1's de B.

    Status: OK
*/
    Tape t = NilTape();                     // creamos la cinta
    snoc(t, A); snoc(t, 0); snoc(t, 0); snoc(t, 0); snoc(t, 0); snoc(t, 0);
    snoc(t, 0); snoc(t, 1); snoc(t, 1); snoc(t, 0); snoc(t, B); snoc(t, 1);
    snoc(t, 1); snoc(t, 1); snoc(t, 1); snoc(t, 0);
    positionCurrent(t, 1, B);               // posicionamos el current
    TuringMachine tm1 = newTM(5, 5, 1);     // creamos la maquina
    // transiciones del estado Qo
    addTR(tm1, 0, 0, 0, L, 0); addTR(tm1, 0, 1, 1, L, 0); addHalt(tm1, 0, A);
    addTR(tm1, 0, B, B, L, 0); addTR(tm1, 0, X, 1, L, 0);
    // estado Q1
    addTR(tm1, 1, 0, 0, R, 0); addTR(tm1, 1, 1, X, R, 2); addTR(tm1, 1, A, A, R, 1);
    addTR(tm1, 1, B, B, R, 1); addTR(tm1, 1, X, X, R, 1);
    // estado Q2
    addTR(tm1, 2, 0, 0, L, 2); addTR(tm1, 2, 1, 1, L, 2); addTR(tm1, 2, A, A, R, 3);
    addTR(tm1, 2, B, B, L, 2); addTR(tm1, 2, X, X, L, 2);
    // estado Q3
    addTR(tm1, 3, 0, 1, R, 4); addTR(tm1, 3, 1, 1, R, 3); addTR(tm1, 3, A, A, R, 3);
    addTR(tm1, 3, B, B, R, 3); addTR(tm1, 3, X, X, R, 3);
    // estado Q4
    addTR(tm1, 4, 0, 0, R, 4); addTR(tm1, 4, 1, 1, R, 4); addTR(tm1, 4, A, A, R, 4);
    addTR(tm1, 4, B, B, R, 1); addTR(tm1, 4, X, X, R, 4);
    run(tm1, t);                            // corremos la maquina
}

void matching(){
/**
    Ejemplo 3: Chequea igualdad de bloques de 1's.
        (bloques determinados por A y B)

    Observacion: En este caso, la cinta resultante no indica
        nada, porque el objetivo no es modificarla. El estado
        final indica si los bloques son de igual longitud o no.

    Caso 1:    ... [A] 1 1 1 1 0 ... [B] 1 1 1 1 0 ... ( = )
    Caso 2:    ... [A] 1 1 1 0 0 ... [B] 1 1 1 1 0 ... ( != )

    Para este ejemplo, el estado que indica igualdad es el 6,
        y el que indica desigualdad es el 7.

    Status: OK - Ambos casos.
*/
    // la cinta que debe dar igualdad (debe terminar en el estado 6)
//    Tape t = NilTape();
//    snoc(t, A); snoc(t, 1); snoc(t, 1); snoc(t, 1); snoc(t, 1); snoc(t, 0);
//    snoc(t, B); snoc(t, 1); snoc(t, 1); snoc(t, 1); snoc(t, 1); snoc(t, 0);

    // la cinta que no debe dar igualdad (debe terminar en el estado 7)
    Tape t = NilTape();
    snoc(t, A); snoc(t, 1); snoc(t, 1); snoc(t, 1); snoc(t, 0); snoc(t, 0);
    snoc(t, B); snoc(t, 1); snoc(t, 1); snoc(t, 1); snoc(t, 1); snoc(t, 0);

    positionCurrent(t, 1, A);               // posicionamos el current
    TuringMachine tm1 = newTM(5, 8, 1);     // creamos la maquina
    // transiciones del estado Qo
    addTR(tm1, 0, 0, 0, R, 0); addTR(tm1, 0, 1, 1, R, 0); addTR(tm1, 0, A, A, R, 0);
    addTR(tm1, 0, B, B, R, 5); addTR(tm1, 0, X, X, R, 0);
    // estado Q1
    addTR(tm1, 1, 0, 0, R, 0); addTR(tm1, 1, 1, X, R, 4); addTR(tm1, 1, A, A, R, 1);
    addTR(tm1, 1, B, B, R, 1); addTR(tm1, 1, X, X, R, 1);
    // estado Q2
    addTR(tm1, 2, 0, 0, L, 2); addTR(tm1, 2, 1, 1, L, 2); addTR(tm1, 2, A, A, R, 1);
    addTR(tm1, 2, B, B, L, 2); addTR(tm1, 2, X, X, L, 2);
    // estado Q3
    addTR(tm1, 3, 0, 0, L, 7); addTR(tm1, 3, 1, X, L, 2); addTR(tm1, 3, A, A, R, 3);
    addTR(tm1, 3, B, B, R, 3); addTR(tm1, 3, X, X, R, 3);
    // estado Q4
    addTR(tm1, 4, 0, 0, R, 4); addTR(tm1, 4, 1, 1, R, 4); addTR(tm1, 4, A, A, R, 4);
    addTR(tm1, 4, B, B, R, 3); addTR(tm1, 4, X, X, R, 4);
    // estado Q5
    addTR(tm1, 5, 0, 0, L, 6); addTR(tm1, 5, 1, 1, L, 7); addTR(tm1, 5, A, A, R, 5);
    addTR(tm1, 5, B, B, R, 5); addTR(tm1, 5, X, X, R, 5);
    // estado Q6
    addTR(tm1, 6, 0, 0, L, 6); addTR(tm1, 6, 1, 1, L, 6); addHalt(tm1, 6, A);
    addTR(tm1, 6, B, B, L, 6); addTR(tm1, 6, X, 1, L, 6);
    // estado Q7
    addTR(tm1, 7, 0, 0, L, 7); addTR(tm1, 7, 1, 1, L, 7); addHalt(tm1, 7, A);
    addTR(tm1, 7, B, B, L, 7); addTR(tm1, 7, X, 1, L, 7);
    run(tm1, t);                            // corremos la maquina
}

void blockSubstitution(){
/**
    Ejemplo 4: Sustitucion de bloques de 1's.
        (bloques determinados por B y C)

    Entrada:   ... [B] 1 1 1 1 1 0 1 0 0 C 1 1 1 0 1 0 1 ...
    Salida:    ... [B] 1 1 1 1 1 0 1 0 0 C 1 1 1 1 1 0 1 0 1 ...

    Status: OK.
*/
    Tape t = NilTape();
    snoc(t, B); snoc(t, 1); snoc(t, 1); snoc(t, 1); snoc(t, 1); snoc(t, 1);
    snoc(t, 0); snoc(t, 1); snoc(t, 0); snoc(t, 0); snoc(t, C); snoc(t, 1);
    snoc(t, 1); snoc(t, 1); snoc(t, 0); snoc(t, 1); snoc(t, 0); snoc(t, 1);

    positionCurrent(t, 1, B);               // posicionamos el current
    TuringMachine tm1 = newTM(5, 13, 0);    // creamos la maquina
    // transiciones del estado Qo
    addTR(tm1, 0, 0, 0, R, 0); addTR(tm1, 0, 1, 1, R, 0); addTR(tm1, 0, B, B, R, 0);
    addTR(tm1, 0, C, C, R, 1); addTR(tm1, 0, X, X, R, 0);
    // estado Q1
    addTR(tm1, 1, 0, 0, L, 6); addTR(tm1, 1, 1, 1, R, 5); addTR(tm1, 1, B, B, R, 1);
    addTR(tm1, 1, C, C, R, 1); addTR(tm1, 1, X, X, R, 1);
    // estado Q2
    addTR(tm1, 2, 0, 1, L, 3); addTR(tm1, 2, 1, 1, L, 2); addTR(tm1, 2, B, B, L, 2);
    addTR(tm1, 2, C, C, R, 1); addTR(tm1, 2, X, X, L, 2);
    // estado Q3
    addTR(tm1, 3, 0, 0, L, 3); addTR(tm1, 3, 1, 0, L, 2); addTR(tm1, 3, B, B, L, 3);
    addTR(tm1, 3, C, C, L, 3); addTR(tm1, 3, X, X, L, 3);
    // estado Q4
    addTR(tm1, 4, 0, 0, L, 3); addTR(tm1, 4, 1, 1, R, 5); addTR(tm1, 4, B, B, R, 4);
    addTR(tm1, 4, C, C, R, 4); addTR(tm1, 4, X, X, R, 4);
    // estado Q5
    addTR(tm1, 5, 0, 0, R, 4); addTR(tm1, 5, 1, 1, R, 5); addTR(tm1, 5, B, B, R, 5);
    addTR(tm1, 5, C, C, R, 5); addTR(tm1, 5, X, X, R, 5);
    // estado Q6
    addTR(tm1, 6, 0, 0, L, 6); addTR(tm1, 6, 1, 1, L, 6); addTR(tm1, 6, B, B, R, 7);
    addTR(tm1, 6, C, C, L, 6); addTR(tm1, 6, X, X, L, 6);
    // estado Q7
    addTR(tm1, 7, 0, 0, L, 12); addTR(tm1, 7, 1, X, R, 11); addTR(tm1, 7, B, B, R, 7);
    addTR(tm1, 7, C, C, R, 7); addTR(tm1, 7, X, X, R, 7);
    // estado Q8
    addTR(tm1, 8, 0, 0, L, 8); addTR(tm1, 8, 1, 1, L, 8); addTR(tm1, 8, B, B, L, 8);
    addTR(tm1, 8, C, C, L, 8); addTR(tm1, 8, X, X, R, 7);
    // estado Q9
    addTR(tm1, 9, 0, 0, L, 8); addTR(tm1, 9, 1, 0, R, 10); addTR(tm1, 9, B, B, R, 9);
    addTR(tm1, 9, C, C, R, 9); addTR(tm1, 9, X, X, R, 9);
    // estado Q10
    addTR(tm1, 10, 0, 1, R, 9); addTR(tm1, 10, 1, 1, R, 10); addTR(tm1, 10, B, B, R, 10);
    addTR(tm1, 10, C, C, R, 10); addTR(tm1, 10, X, X, R, 10);
    // estado Q11
    addTR(tm1, 11, 0, 0, R, 11); addTR(tm1, 11, 1, 1, R, 11); addTR(tm1, 11, B, B, R, 11);
    addTR(tm1, 11, C, C, R, 10); addTR(tm1, 11, X, X, R, 11);
    // estado Q12
    addTR(tm1, 12, 0, 0, L, 12); addTR(tm1, 12, 1, 1, L, 12); addHalt(tm1, 12, B);
    addTR(tm1, 12, C, C, L, 12); addTR(tm1, 12, X, 1, L, 12);
    run(tm1, t);                            // corremos la maquina
}

void testUTM(){
/**
    testUTM: Prueba el funcionamiento de la maquina de Turing
        Universal, emulando la maquina del ejemplo 1. (fusion
        de 1's).

    Se asume que la maquina emulada tiene como estado inicial Q0.
*/
    Tape t = NilTape();         // creamos la cinta (de la maquina emulada)
    snoc(t, 1); snoc(t, 1); snoc(t, 1); snoc(t, 0);
    snoc(t, 0); snoc(t, 0); snoc(t, 1); snoc(t, 1);
    positionCurrent(t, 1, 1);

    TuringMachine tm1 = newTM(2, 4, 0);  // creamos la maquina emulada
    addHalt(tm1, 0, 0); addTR(tm1, 0, 1, 0, R, 1); addTR(tm1, 1, 0, 1, R, 2);
    addTR(tm1, 1, 1, 1, R, 1); addTR(tm1, 2, 0, 0, L, 3); addHalt(tm1, 2, 1);
    addTR(tm1, 3, 0, 0, R, 0); addTR(tm1, 3, 1, 1, L, 3);

    TuringMachine utm = newUTM();       // creamos la maquina universal
    runUTM(utm, tm1, t);                // corremos la maquina universal
}
