#include "UniversalTuringMachine.h"

Tape commonTape2UTMTape(Tape t){
/**
    Observacion: Se ubica un simbolo C justo antes del
        current de la cinta recibida como parametro, ademas
        de convertir a 1's y 0's la cinta.
*/
    Tape res = NilTape();
    struct tNode* curr = t->first;      // no se puede usar el current del struct
    while(not(curr == NULL)){
        if(curr == t->current)
            snoc(res, C);               // aqui va a estar la cinta "posicionada"
        else
            snoc(res, 0);               // delimitador de simbolo
        snocN(res, curr->value+1, 1);   // simbolo codificado a 1's
        curr = curr->next;              // pasar al siguiente
    }
    return res;
}

Tape UTMTape2CommonTape(Tape t){
/**
    Retorna una nueva cinta, descartando el buffer y la maquina
    codificada de la cinta de la maquina universal y decodificando
    lo que resta de la cinta. Quita el simbolo C y en esta posicion
    coloca el puntero current.

    Precondicion: El simbolo B indica el comienzo de la cinta y el
        current de la cinta recibida esta posicionado en B (como
        finaliza en el computo de la maquina universal).
*/
    Tape res = NilTape();
    int n = 0;
    t->current = t->current->next->next;  // pasamos al siguiente '1' despues de B
    while(not(t->current == NULL)){
        if((t->current->value == 0 || t->current->value == C)
                && (t->current->prev->value != 0)){
            snoc(res, n-1);     // cantidad de 1's (n-1) determina el simbolo a poner
            n = 0;
            if(t->current->value == C)
                res->current = res->last;  // seteo el current de la nueva cinta
        }
        else if(t->current->value == 1)
            n++;
        t->current = t->current->next;
    }
    res->current = res->current->next;   // el verdadero current
    // se puede pedir seguro pues es el que le corresponde a la cinta
    return res;
}

Tape mkTape(TuringMachine tm, Tape t){
    Tape res = NilTape();
    snoc(res, A);                                   // inicio de buffer
    snocN(res, tm->states+tm->symbols+2, 0);        // region de buffer
    snoc(res, B);                                   // inicio de maquina codificada
    append(res, machine2Tape(tm));                  // toda la maquina codificada
    snoc(res, 0);                                   // fin de maquina codificada
    append(res, commonTape2UTMTape(t));             // la cinta de la maquina codificada
    return res;
}

TuringMachine newUTM(){
/**
    newUTM: Constructor de una maquina de Turing Universal
*/
    // Se crea la maquina universal, sus 69 estados y 408 transiciones
    TuringMachine utm = newTM(6, 69, 0);
    //Q0
    addTR(utm, 0, 0, 0, L, 4); addTR(utm, 0, A, A, R, 0); addTR(utm, 0, C, C, R, 0);
    addTR(utm, 0, 1, X, L, 1); addTR(utm, 0, B, B, R, 0); addTR(utm, 0, X, X, R, 0);
    //Q1
    addTR(utm, 1, 0, 0, L, 1); addTR(utm, 1, A, A, R, 2); addTR(utm, 1, C, C, L, 1);
    addTR(utm, 1, 1, 1, L, 1); addTR(utm, 1, B, B, L, 1); addTR(utm, 1, X, X, L, 1);
    //Q2
    addTR(utm, 2, 0, 1, R, 3); addTR(utm, 2, A, A, R, 2); addTR(utm, 2, C, C, R, 2);
    addTR(utm, 2, 1, 1, R, 2); addTR(utm, 2, B, B, R, 2); addTR(utm, 2, X, X, R, 2);
    //Q3
    addTR(utm, 3, 0, 0, R, 3); addTR(utm, 3, A, A, R, 3); addTR(utm, 3, C, C, R, 3);
    addTR(utm, 3, 1, 1, R, 3); addTR(utm, 3, B, B, R, 0); addTR(utm, 3, X, X, R, 3);
    //Q4
    addTR(utm, 4, 0, 0, L, 4); addTR(utm, 4, A, A, R, 5); addTR(utm, 4, C, C, L, 4);
    addTR(utm, 4, 1, 1, L, 4); addTR(utm, 4, B, B, L, 4); addTR(utm, 4, X, 1, L, 4);
    //Q5
    addTR(utm, 5, 0, A, R, 6); addTR(utm, 5, A, A, R, 5); addTR(utm, 5, C, C, R, 5);
    addTR(utm, 5, 1, 1, R, 5); addTR(utm, 5, B, B, R, 5); addTR(utm, 5, X, X, R, 5);
    //Q6
    addTR(utm, 6, 0, 0, R, 6); addTR(utm, 6, A, A, R, 6); addTR(utm, 6, C, C, R, 7);
    addTR(utm, 6, 1, 1, R, 6); addTR(utm, 6, B, 0, R, 6); addTR(utm, 6, X, X, R, 6);
    //Q7
    addTR(utm, 7, 0, 0, L, 11); addTR(utm, 7, A, A, R, 7); addTR(utm, 7, C, C, R, 7);
    addTR(utm, 7, 1, X, L, 8); addTR(utm, 7, B, B, R, 7); addTR(utm, 7, X, X, R, 7);
    //Q8
    addTR(utm, 8, 0, 0, L, 8); addTR(utm, 8, A, A, R, 9); addTR(utm, 8, C, C, L, 8);
    addTR(utm, 8, 1, 1, L, 8); addTR(utm, 8, B, B, L, 8); addTR(utm, 8, X, X, L, 8);
    //Q9
    addTR(utm, 9, 0, 1, R, 10); addTR(utm, 9, A, A, R, 9); addTR(utm, 9, C, C, R, 9);
    addTR(utm, 9, 1, 1, R, 9); addTR(utm, 9, B, B, R, 9); addTR(utm, 9, X, X, R, 9);
    //Q10
    addTR(utm, 10, 0, 0, R, 10); addTR(utm, 10, A, A, R, 10); addTR(utm, 10, C, C, R, 7);
    addTR(utm, 10, 1, 1, R, 10); addTR(utm, 10, B, B, R, 10); addTR(utm, 10, X, X, R, 10);
    //Q11
    addTR(utm, 11, 0, 0, L, 11); addTR(utm, 11, A, 0, R, 12); addTR(utm, 11, C, C, L, 11);
    addTR(utm, 11, 1, 1, L, 11); addTR(utm, 11, B, B, L, 11); addTR(utm, 11, X, 1, L, 11);
    //Q12
    addTR(utm, 12, 0, 0, R, 13); addTR(utm, 12, A, A, R, 12); addTR(utm, 12, C, C, R, 12);
    addTR(utm, 12, 1, 1, R, 12); addTR(utm, 12, B, B, R, 12); addTR(utm, 12, X, X, R, 12);
    //Q13
    addTR(utm, 13, 0, 0, R, 13); addTR(utm, 13, A, A, R, 13); addTR(utm, 13, C, C, R, 13);
    addTR(utm, 13, 1, 1, L, 14); addTR(utm, 13, B, B, R, 13); addTR(utm, 13, X, X, R, 13);
    //Q14
    addTR(utm, 14, 0, B, L, 15); addTR(utm, 14, A, A, L, 14); addTR(utm, 14, C, C, L, 14);
    addTR(utm, 14, 1, 1, L, 14); addTR(utm, 14, B, B, L, 14); addTR(utm, 14, X, X, L, 14);
    //Q15
    addTR(utm, 15, 0, 0, L, 15); addTR(utm, 15, A, A, R, 16); addTR(utm, 15, C, C, L, 15);
    addTR(utm, 15, 1, 1, L, 15); addTR(utm, 15, B, B, L, 15); addTR(utm, 15, X, X, L, 15);
    //Q16
    addTR(utm, 16, 0, 0, R, 20); addTR(utm, 16, A, A, R, 16); addTR(utm, 16, C, C, R, 16);
    addTR(utm, 16, 1, X, R, 19); addTR(utm, 16, B, B, R, 16); addTR(utm, 16, X, X, R, 16);
    //Q17
    addTR(utm, 17, 0, 0, L, 17); addTR(utm, 17, A, A, R, 16); addTR(utm, 17, C, C, L, 17);
    addTR(utm, 17, 1, 1, L, 17); addTR(utm, 17, B, B, L, 17); addTR(utm, 17, X, X, L, 17);
    //Q18
    addTR(utm, 18, 0, 0, L, 23); addTR(utm, 18, A, A, R, 18); addTR(utm, 18, C, C, R, 18);
    addTR(utm, 18, 1, X, L, 17); addTR(utm, 18, B, B, R, 18); addTR(utm, 18, X, X, R, 18);
    //Q19
    addTR(utm, 19, 0, 0, R, 19); addTR(utm, 19, A, A, R, 19); addTR(utm, 19, C, C, R, 19);
    addTR(utm, 19, 1, 1, R, 19); addTR(utm, 19, B, B, R, 18); addTR(utm, 19, X, X, R, 19);
    //Q20
    addTR(utm, 20, 0, 0, R, 20); addTR(utm, 20, A, A, R, 20); addTR(utm, 20, C, C, R, 20);
    addTR(utm, 20, 1, 1, R, 20); addTR(utm, 20, B, B, R, 21); addTR(utm, 20, X, X, R, 20);
    //Q21
    addTR(utm, 21, 0, 0, L, 22); addTR(utm, 21, A, A, R, 21); addTR(utm, 21, C, C, R, 21);
    addTR(utm, 21, 1, 1, L, 23); addTR(utm, 21, B, A, R, 21); addTR(utm, 21, X, X, R, 21);
    //Q22
    addTR(utm, 22, 0, 0, L, 22); addTR(utm, 22, A, A, R, 29); addTR(utm, 22, C, C, L, 22);
    addTR(utm, 22, 1, 1, L, 22); addTR(utm, 22, B, B, L, 22); addTR(utm, 22, X, 1, L, 22);
    //Q23
    addTR(utm, 23, 0, 0, L, 23); addTR(utm, 23, A, A, R, 28); addTR(utm, 23, C, C, L, 23);
    addTR(utm, 23, 1, 1, L, 23); addTR(utm, 23, B, B, L, 23); addTR(utm, 23, X, 1, L, 23);
    //Q24
    addTR(utm, 24, 0, 0, L, 24); addTR(utm, 24, A, A, R, 16); addTR(utm, 24, C, C, L, 24);
    addTR(utm, 24, 1, 1, L, 24); addTR(utm, 24, B, B, L, 24); addTR(utm, 24, X, X, L, 24);
    //Q25 (halt: halt de la maquina emulada)
    addHalt(utm, 25, 0); addTR(utm, 25, A, A, R, 25); addTR(utm, 25, C, C, R, 25);
    addTR(utm, 25, 1, 1, L, 24); addTR(utm, 25, B, B, R, 25); addTR(utm, 25, X, X, R, 25);
    //Q26
    addTR(utm, 26, 0, B, R, 25); addTR(utm, 26, A, A, R, 26); addTR(utm, 26, C, C, R, 26);
    addTR(utm, 26, 1, 1, R, 27); addTR(utm, 26, B, B, R, 26); addTR(utm, 26, X, X, R, 26);
    //Q27
    addTR(utm, 27, 0, 0, R, 26); addTR(utm, 27, A, A, R, 27); addTR(utm, 27, C, C, R, 27);
    addTR(utm, 27, 1, 1, R, 27); addTR(utm, 27, B, B, R, 27); addTR(utm, 27, X, X, R, 27);
    //Q28
    addTR(utm, 28, 0, 0, R, 28); addTR(utm, 28, A, A, R, 28); addTR(utm, 28, C, C, R, 28);
    addTR(utm, 28, 1, 1, R, 28); addTR(utm, 28, B, 0, R, 27); addTR(utm, 28, X, X, R, 28);
    //Q29
    addTR(utm, 29, 0, A, R, 30); addTR(utm, 29, A, A, R, 29); addTR(utm, 29, C, C, R, 29);
    addTR(utm, 29, 1, 1, R, 29); addTR(utm, 29, B, B, R, 29); addTR(utm, 29, X, X, R, 29);
    //Q30
    addTR(utm, 30, 0, 0, R, 30); addTR(utm, 30, A, A, R, 30); addTR(utm, 30, C, C, R, 30);
    addTR(utm, 30, 1, 1, R, 30); addTR(utm, 30, B, 0, R, 31); addTR(utm, 30, X, X, R, 30);
    //Q31
    addTR(utm, 31, 0, B, L, 32); addTR(utm, 31, A, A, R, 31); addTR(utm, 31, C, C, R, 31);
    addTR(utm, 31, 1, 1, R, 31); addTR(utm, 31, B, B, R, 31); addTR(utm, 31, X, X, R, 31);
    //Q32
    addTR(utm, 32, 0, 0, L, 32); addTR(utm, 32, A, A, R, 33); addTR(utm, 32, C, C, L, 32);
    addTR(utm, 32, 1, 1, L, 32); addTR(utm, 32, B, B, L, 32); addTR(utm, 32, X, X, L, 32);
    //Q33
    addTR(utm, 33, 0, 0, R, 37); addTR(utm, 33, A, A, R, 33); addTR(utm, 33, C, C, R, 33);
    addTR(utm, 33, 1, X, R, 36); addTR(utm, 33, B, B, R, 33); addTR(utm, 33, X, X, R, 33);
    //Q34
    addTR(utm, 34, 0, 0, L, 34); addTR(utm, 34, A, A, R, 33); addTR(utm, 34, C, C, L, 34);
    addTR(utm, 34, 1, 1, L, 34); addTR(utm, 34, B, B, L, 34); addTR(utm, 34, X, X, L, 34);
    //Q35
    addTR(utm, 35, 0, 0, L, 40); addTR(utm, 35, A, A, R, 35); addTR(utm, 35, C, C, R, 35);
    addTR(utm, 35, 1, X, L, 34); addTR(utm, 35, B, B, R, 35); addTR(utm, 35, X, X, R, 35);
    //Q36
    addTR(utm, 36, 0, 0, R, 36); addTR(utm, 36, A, A, R, 36); addTR(utm, 36, C, C, R, 36);
    addTR(utm, 36, 1, 1, R, 36); addTR(utm, 36, B, B, R, 35); addTR(utm, 36, X, X, R, 36);
    //Q37
    addTR(utm, 37, 0, 0, R, 37); addTR(utm, 37, A, A, R, 37); addTR(utm, 37, C, C, R, 37);
    addTR(utm, 37, 1, 1, R, 37); addTR(utm, 37, B, B, R, 38); addTR(utm, 37, X, X, R, 37);
    //Q38
    addTR(utm, 38, 0, 0, L, 39); addTR(utm, 38, A, A, R, 38); addTR(utm, 38, C, C, R, 38);
    addTR(utm, 38, 1, 1, L, 40); addTR(utm, 38, B, B, R, 38); addTR(utm, 38, X, X, R, 38);
    //Q39
    addTR(utm, 39, 0, 0, L, 39); addTR(utm, 39, A, 0, R, 41); addTR(utm, 39, C, C, L, 39);
    addTR(utm, 39, 1, 1, L, 39); addTR(utm, 39, B, B, L, 39); addTR(utm, 39, X, 1, L, 39);
    //Q40
    addTR(utm, 40, 0, 0, L, 40); addTR(utm, 40, A, 0, R, 28); addTR(utm, 40, C, C, L, 40);
    addTR(utm, 40, 1, 1, L, 40); addTR(utm, 40, B, B, L, 40); addTR(utm, 40, X, 1, L, 40);
    //Q41
    addTR(utm, 41, 0, 0, L, 42); addTR(utm, 41, A, A, R, 41); addTR(utm, 41, C, C, R, 41);
    addTR(utm, 41, 1, 1, R, 41); addTR(utm, 41, B, B, R, 41); addTR(utm, 41, X, X, R, 41);
    //Q42
    addTR(utm, 42, 0, 0, L, 42); addTR(utm, 42, A, A, R, 43); addTR(utm, 42, C, C, L, 42);
    addTR(utm, 42, 1, 0, L, 42); addTR(utm, 42, B, B, L, 42); addTR(utm, 42, X, X, L, 42);
    //Q43
    addTR(utm, 43, 0, 0, R, 43); addTR(utm, 43, A, A, R, 43); addTR(utm, 43, C, C, R, 43);
    addTR(utm, 43, 1, 1, R, 43); addTR(utm, 43, B, 0, R, 44); addTR(utm, 43, X, X, R, 43);
    //Q44
    addTR(utm, 44, 0, B, R, 45); addTR(utm, 44, A, A, R, 44); addTR(utm, 44, C, C, R, 44);
    addTR(utm, 44, 1, 1, R, 44); addTR(utm, 44, B, B, R, 44); addTR(utm, 44, X, X, R, 44);
    //Q45
    addTR(utm, 45, 0, 0, R, 45); addTR(utm, 45, A, A, R, 45); addTR(utm, 45, C, C, R, 46);
    addTR(utm, 45, 1, 1, R, 45); addTR(utm, 45, B, B, R, 45); addTR(utm, 45, X, X, R, 45);
    //Q46
    addTR(utm, 46, 0, 0, L, 51); addTR(utm, 46, A, A, R, 46); addTR(utm, 46, C, C, R, 46);
    addTR(utm, 46, 1, 1, R, 47); addTR(utm, 46, B, B, R, 46); addTR(utm, 46, X, X, R, 46);
    //Q47
    addTR(utm, 47, 0, 0, R, 48); addTR(utm, 47, A, A, R, 47); addTR(utm, 47, C, C, R, 47);
    addTR(utm, 47, 1, 1, R, 47); addTR(utm, 47, B, B, R, 47); addTR(utm, 47, X, X, R, 47);
    //Q48
    addTR(utm, 48, 0, 0, L, 49); addTR(utm, 48, A, A, R, 48); addTR(utm, 48, C, C, R, 48);
    addTR(utm, 48, 1, 1, R, 47); addTR(utm, 48, B, B, R, 48); addTR(utm, 48, X, X, R, 48);
    //Q49
    addTR(utm, 49, 0, 0, L, 49); addTR(utm, 49, A, A, L, 49); addTR(utm, 49, C, C, L, 49);
    addTR(utm, 49, 1, 0, L, 50); addTR(utm, 49, B, B, L, 49); addTR(utm, 49, X, X, L, 49);
    //Q50
    addTR(utm, 50, 0, 1, L, 49); addTR(utm, 50, A, A, L, 50); addTR(utm, 50, C, C, R, 46);
    addTR(utm, 50, 1, 1, L, 50); addTR(utm, 50, B, B, L, 50); addTR(utm, 50, X, X, L, 50);
    //Q51
    addTR(utm, 51, 0, 0, L, 51); addTR(utm, 51, A, A, L, 51); addTR(utm, 51, C, C, L, 51);
    addTR(utm, 51, 1, 1, L, 51); addTR(utm, 51, B, B, R, 52); addTR(utm, 51, X, X, L, 51);
    //Q52
    addTR(utm, 52, 0, 0, L, 57); addTR(utm, 52, A, A, R, 52); addTR(utm, 52, C, C, R, 52);
    addTR(utm, 52, 1, X, R, 53); addTR(utm, 52, B, B, R, 52); addTR(utm, 52, X, X, R, 52);
    //Q53
    addTR(utm, 53, 0, 0, R, 53); addTR(utm, 53, A, A, R, 53); addTR(utm, 53, C, C, R, 54);
    addTR(utm, 53, 1, 1, R, 53); addTR(utm, 53, B, B, R, 53); addTR(utm, 53, X, X, R, 53);
    //Q54
    addTR(utm, 54, 0, 1, R, 55); addTR(utm, 54, A, A, R, 54); addTR(utm, 54, C, C, R, 54);
    addTR(utm, 54, 1, 1, R, 54); addTR(utm, 54, B, B, R, 54); addTR(utm, 54, X, X, R, 54);
    //Q55
    addTR(utm, 55, 0, 0, L, 56); addTR(utm, 55, A, A, R, 55); addTR(utm, 55, C, C, R, 55);
    addTR(utm, 55, 1, 0, R, 54); addTR(utm, 55, B, B, R, 55); addTR(utm, 55, X, X, R, 55);
    //Q56
    addTR(utm, 56, 0, 0, L, 56); addTR(utm, 56, A, A, L, 56); addTR(utm, 56, C, C, L, 56);
    addTR(utm, 56, 1, 1, L, 56); addTR(utm, 56, B, B, L, 56); addTR(utm, 56, X, X, R, 52);
    //Q57
    addTR(utm, 57, 0, 0, L, 57); addTR(utm, 57, A, A, L, 57); addTR(utm, 57, C, C, L, 57);
    addTR(utm, 57, 1, 1, L, 57); addTR(utm, 57, B, 0, R, 58); addTR(utm, 57, X, 1, L, 57);
    //Q58
    addTR(utm, 58, 0, 0, R, 59); addTR(utm, 58, A, A, R, 58); addTR(utm, 58, C, C, R, 58);
    addTR(utm, 58, 1, 1, R, 58); addTR(utm, 58, B, B, R, 58); addTR(utm, 58, X, X, R, 58);
    //Q59
    addTR(utm, 59, 0, 0, R, 59); addTR(utm, 59, A, A, R, 59); addTR(utm, 59, C, C, R, 59);
    addTR(utm, 59, 1, 1, R, 60); addTR(utm, 59, B, B, R, 59); addTR(utm, 59, X, X, R, 59);
    //Q60
    addTR(utm, 60, 0, B, R, 61); addTR(utm, 60, A, A, R, 60); addTR(utm, 60, C, C, R, 60);
    addTR(utm, 60, 1, 1, R, 65); addTR(utm, 60, B, B, R, 60); addTR(utm, 60, X, X, R, 60);
    //Q61
    addTR(utm, 61, 0, 0, R, 61); addTR(utm, 61, A, A, R, 61); addTR(utm, 61, C, 0, L, 62);
    addTR(utm, 61, 1, 1, R, 61); addTR(utm, 61, B, B, R, 61); addTR(utm, 61, X, X, R, 61);
    //Q62 (halt: la maquina se quedo sin cinta)
    addHalt(utm, 62, 0); addTR(utm, 62, A, A, L, 62); addTR(utm, 62, C, C, L, 62);
    addTR(utm, 62, 1, 1, L, 63); addTR(utm, 62, B, B, L, 62); addTR(utm, 62, X, X, L, 62);
    //Q63
    addTR(utm, 63, 0, C, L, 64); addTR(utm, 63, A, A, L, 63); addTR(utm, 63, C, C, L, 63);
    addTR(utm, 63, 1, 1, L, 63); addTR(utm, 63, B, B, L, 63); addTR(utm, 63, X, X, L, 63);
    //Q64
    addTR(utm, 64, 0, 0, L, 64); addTR(utm, 64, A, A, L, 64); addTR(utm, 64, C, C, L, 64);
    addTR(utm, 64, 1, 1, L, 64); addTR(utm, 64, B, B, R, 0); addTR(utm, 64, X, X, L, 64);
    //Q65
    addTR(utm, 65, 0, B, R, 66); addTR(utm, 65, A, A, R, 65); addTR(utm, 65, C, C, R, 65);
    addTR(utm, 65, 1, 1, R, 65); addTR(utm, 65, B, B, R, 65); addTR(utm, 65, X, X, R, 65);
    //Q66
    addTR(utm, 66, 0, 0, R, 66); addTR(utm, 66, A, A, R, 66); addTR(utm, 66, C, 0, R, 67);
    addTR(utm, 66, 1, 1, R, 66); addTR(utm, 66, B, B, R, 66); addTR(utm, 66, X, X, R, 66);
    //Q67
    addTR(utm, 67, 0, C, R, 68); addTR(utm, 67, A, A, R, 67); addTR(utm, 67, C, C, R, 67);
    addTR(utm, 67, 1, 1, R, 67); addTR(utm, 67, B, B, R, 67); addTR(utm, 67, X, X, R, 67);
    //Q68
    addTR(utm, 68, 0, 1, L, 64); addTR(utm, 68, A, A, R, 68); addTR(utm, 68, C, C, R, 68);
    addTR(utm, 68, 1, 1, L, 64); addTR(utm, 68, B, B, R, 68); addTR(utm, 68, X, X, R, 68);

    return utm;
}

void runUTM(TuringMachine utm, TuringMachine tm, Tape t){
    printf("Cinta de la maquina emulada (antes de computar): ");
    printTape(t);
    Tape utmTape = mkTape(tm, t);
    positionCurrent(utmTape, 1, B);
    printf("\nCinta de la maquina universal (antes de computar): ");
    printTape(utmTape);

    compute(utm, utmTape);

    printf("\nCinta de la maquina universal (despues de computar): ");
    printTape(utmTape);
    t = UTMTape2CommonTape(utmTape);
    printf("\nCinta de la maquina emulada (despues de computar): ");
    printTape(t);

    printf("\nEstado final: %d : ", utm->currentState);
    if(utm->currentState == 62)
        printf("La maquina emulada se quedo sin cinta.\n");
    else if(utm->currentState == 25)
        printf("La maquina emulada finalizo correctamente.\n");
}
