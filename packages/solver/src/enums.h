#pragma once
#include <string>
#include <stdexcept>

enum Color {
    U = 0, R, F, D, L, B
};

inline const char ColorChar[6] = { 'U', 'R', 'F', 'D', 'L', 'B' };

inline Color charToColor(char c) {
    switch (c) {
        case 'U': return U;
        case 'R': return R;
        case 'F': return F;
        case 'D': return D;
        case 'L': return L;
        case 'B': return B;
        default: throw std::invalid_argument("Invalid color character: " + std::string(1, c));
    }
}

enum Facelet {
    U1 = 0, U2, U3, U4, U5, U6, U7, U8, U9,
    R1, R2, R3, R4, R5, R6, R7, R8, R9,
    F1, F2, F3, F4, F5, F6, F7, F8, F9,
    D1, D2, D3, D4, D5, D6, D7, D8, D9,
    L1, L2, L3, L4, L5, L6, L7, L8, L9,
    B1, B2, B3, B4, B5, B6, B7, B8, B9
};

enum Corner {
    URF = 0, UFL, ULB, UBR, DFR, DLF, DBL, DRB
};

enum Edge {
    UR = 0, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR
};

const Facelet cornerFacelet[8][3] = {
    { U9, R1, F3 },
    { U7, F1, L3 },
    { U1, L1, B3 },
    { U3, B1, R3 },
    { D3, F9, R7 },
    { D1, L9, F7 },
    { D7, B9, L7 },
    { D9, R9, B7 }
};

const Facelet edgeFacelet[12][2] = {
    { U6, R2 },
    { U8, F2 },
    { U4, L2 },
    { U2, B2 },
    { D6, R8 },
    { D2, F8 },
    { D4, L8 },
    { D8, B8 },
    { F6, R4 },
    { F4, L6 },
    { B6, L4 },
    { B4, R6 }
};

const Color cornerColor[8][3] = {
    { U, R, F },
    { U, F, L },
    { U, L, B },
    { U, B, R },
    { D, F, R },
    { D, L, F },
    { D, B, L },
    { D, R, B }
};

const Color edgeColor[12][2] = {
    { U, R },
    { U, F },
    { U, L },
    { U, B },
    { D, R },
    { D, F },
    { D, L },
    { D, B },
    { F, R },
    { F, L },
    { B, L },
    { B, R }
};
