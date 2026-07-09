#pragma once
#include <cstdint>
#include "cubiecube.h"

// Dimension constants
const int N_TWIST = 2187;
const int N_FLIP = 2048;
const int N_SLICE1 = 495;
const int N_SLICE2 = 24;
const int N_PARITY = 2;
const int N_URFtoDLF = 20160;
const int N_FRtoBR = 11880;
const int N_URtoUL = 1320;
const int N_UBtoDF = 1320;
const int N_URtoDF = 20160;
const int N_URFtoDLB = 40320;
const int N_URtoBR = 479001600;
const int N_MOVE = 18;

namespace CoordCubeTables {
    extern int twistMove[N_TWIST][18];
    extern int flipMove[N_FLIP][18];
    extern int FRtoBR_Move[N_FRtoBR][18];
    extern int URFtoDLF_Move[N_URFtoDLF][18];
    extern int URtoDF_Move[N_URtoDF][18];
    extern int URtoUL_Move[N_URtoUL][18];
    extern int UBtoDF_Move[N_UBtoDF][18];
    extern int MergeURtoULandUBtoDF[336][336];

    extern const int parityMove[2][18];

    extern uint8_t Slice_URFtoDLF_Parity_Prun[N_SLICE2 * N_URFtoDLF * N_PARITY / 2];
    extern uint8_t Slice_URtoDF_Parity_Prun[N_SLICE2 * N_URtoDF * N_PARITY / 2];
    extern uint8_t Slice_Twist_Prun[(N_SLICE1 * N_TWIST) / 2 + 1];
    extern uint8_t Slice_Flip_Prun[N_SLICE1 * N_FLIP / 2];
}

void setPruning(uint8_t table[], int index, uint8_t value);
uint8_t getPruning(const uint8_t table[], int index);

void generateTables();

struct CoordCube {
    int twist;
    int flip;
    int parity;
    int FRtoBR;
    int URFtoDLF;
    int URtoUL;
    int UBtoDF;
    int URtoDF;

    CoordCube(const CubieCube& c);
    void move(int m);
};
