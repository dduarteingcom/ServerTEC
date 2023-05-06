//
// Created by dadu on 29/04/23.
//
/*
 * 0=Suma
 * 1=Resta
 * 2=Multiplicacion
 * 3=División
 */
#include "Calculator.h"
#include "iostream"
Calculator::Calculator() {
    iniciado = false;
    //error = false;
}


int Calculator::operation(int num, int x) {
    if(!iniciado){
        tmp = num;
        tmpc = x;
        iniciado = true;
        return 0;
    }
    else{

        switch (tmpc) {
            case 0:
                tmp += num;
                tmpc = x;
                break;
            case 1:
                tmp -= num;
                tmpc = x;
                break;
            case 2:
                tmp *= num;
                tmpc = x;
                break;
            case 3:
                tmp /= num;
                tmpc = x;
                break;
        }
        if(x==4){
            iniciado = false;
            return tmp;
        }
        return 0;
    }

}