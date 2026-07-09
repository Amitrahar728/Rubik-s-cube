#include "coordcube.h"
#include <algorithm>

namespace CoordCubeTables {
    int twistMove[N_TWIST][18];
    int flipMove[N_FLIP][18];
    int FRtoBR_Move[N_FRtoBR][18];
    int URFtoDLF_Move[N_URFtoDLF][18];
    int URtoDF_Move[N_URtoDF][18];
    int URtoUL_Move[N_URtoUL][18];
    int UBtoDF_Move[N_UBtoDF][18];
    int MergeURtoULandUBtoDF[336][336];

    const int parityMove[2][18] = {
        {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1},
        {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0}
    };

    uint8_t Slice_URFtoDLF_Parity_Prun[N_SLICE2 * N_URFtoDLF * N_PARITY / 2];
    uint8_t Slice_URtoDF_Parity_Prun[N_SLICE2 * N_URtoDF * N_PARITY / 2];
    uint8_t Slice_Twist_Prun[(N_SLICE1 * N_TWIST) / 2 + 1];
    uint8_t Slice_Flip_Prun[N_SLICE1 * N_FLIP / 2];
}

void setPruning(uint8_t table[], int index, uint8_t value) {
    if ((index & 1) == 0) {
        table[index / 2] &= 0xf0 | value;
    } else {
        table[index / 2] &= 0x0f | (value << 4);
    }
}

uint8_t getPruning(const uint8_t table[], int index) {
    if ((index & 1) == 0) {
        return table[index / 2] & 0x0f;
    } else {
        return (table[index / 2] & 0xf0) >> 4;
    }
}

void generateTwistMove() {
    CubieCube a;
    for (int i = 0; i < N_TWIST; i++) {
        a.setTwist(i);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                a.cornerMultiply(CubieCube::moveCube[j]);
                CoordCubeTables::twistMove[i][3 * j + k] = a.getTwist();
            }
            a.cornerMultiply(CubieCube::moveCube[j]);
        }
    }
}

void generateFlipMove() {
    CubieCube a;
    for (int i = 0; i < N_FLIP; i++) {
        a.setFlip(i);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                a.edgeMultiply(CubieCube::moveCube[j]);
                CoordCubeTables::flipMove[i][3 * j + k] = a.getFlip();
            }
            a.edgeMultiply(CubieCube::moveCube[j]);
        }
    }
}

void generateFRtoBR_Move() {
    CubieCube a;
    for (int i = 0; i < N_FRtoBR; i++) {
        a.setFRtoBR(i);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                a.edgeMultiply(CubieCube::moveCube[j]);
                CoordCubeTables::FRtoBR_Move[i][3 * j + k] = a.getFRtoBR();
            }
            a.edgeMultiply(CubieCube::moveCube[j]);
        }
    }
}

void generateURFtoDLF_Move() {
    CubieCube a;
    for (int i = 0; i < N_URFtoDLF; i++) {
        a.setURFtoDLF(i);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                a.cornerMultiply(CubieCube::moveCube[j]);
                CoordCubeTables::URFtoDLF_Move[i][3 * j + k] = a.getURFtoDLF();
            }
            a.cornerMultiply(CubieCube::moveCube[j]);
        }
    }
}

void generateURtoDF_Move() {
    CubieCube a;
    for (int i = 0; i < N_URtoDF; i++) {
        a.setURtoDF(i);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                a.edgeMultiply(CubieCube::moveCube[j]);
                CoordCubeTables::URtoDF_Move[i][3 * j + k] = a.getURtoDF();
            }
            a.edgeMultiply(CubieCube::moveCube[j]);
        }
    }
}

void generateURtoUL_Move() {
    CubieCube a;
    for (int i = 0; i < N_URtoUL; i++) {
        a.setURtoUL(i);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                a.edgeMultiply(CubieCube::moveCube[j]);
                CoordCubeTables::URtoUL_Move[i][3 * j + k] = a.getURtoUL();
            }
            a.edgeMultiply(CubieCube::moveCube[j]);
        }
    }
}

void generateUBtoDF_Move() {
    CubieCube a;
    for (int i = 0; i < N_UBtoDF; i++) {
        a.setUBtoDF(i);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                a.edgeMultiply(CubieCube::moveCube[j]);
                CoordCubeTables::UBtoDF_Move[i][3 * j + k] = a.getUBtoDF();
            }
            a.edgeMultiply(CubieCube::moveCube[j]);
        }
    }
}

void generateMergeURtoULandUBtoDF() {
    for (int uRtoUL = 0; uRtoUL < 336; uRtoUL++) {
        for (int uBtoDF = 0; uBtoDF < 336; uBtoDF++) {
            CoordCubeTables::MergeURtoULandUBtoDF[uRtoUL][uBtoDF] = CubieCube::getURtoDF(uRtoUL, uBtoDF);
        }
    }
}

void generateSlice_URFtoDLF_Parity_Prun() {
    std::fill_n(CoordCubeTables::Slice_URFtoDLF_Parity_Prun, N_SLICE2 * N_URFtoDLF * N_PARITY / 2, 0xff);
    int depth = 0;
    setPruning(CoordCubeTables::Slice_URFtoDLF_Parity_Prun, 0, 0);
    int done = 1;
    while (done != N_SLICE2 * N_URFtoDLF * N_PARITY) {
        for (int i = 0; i < N_SLICE2 * N_URFtoDLF * N_PARITY; i++) {
            int parity = i % 2;
            int URFtoDLF = (i / 2) / N_SLICE2;
            int slice = (i / 2) % N_SLICE2;
            if (getPruning(CoordCubeTables::Slice_URFtoDLF_Parity_Prun, i) == depth) {
                for (int j = 0; j < 18; j++) {
                    switch (j) {
                        case 3: case 5: case 6: case 8: case 12: case 14: case 15: case 17:
                            continue;
                        default: {
                            int newSlice = CoordCubeTables::FRtoBR_Move[slice][j];
                            int newURFtoDLF = CoordCubeTables::URFtoDLF_Move[URFtoDLF][j];
                            int newParity = CoordCubeTables::parityMove[parity][j];
                            int idx = (N_SLICE2 * newURFtoDLF + newSlice) * 2 + newParity;
                            if (getPruning(CoordCubeTables::Slice_URFtoDLF_Parity_Prun, idx) == 0x0f) {
                                setPruning(CoordCubeTables::Slice_URFtoDLF_Parity_Prun, idx, depth + 1);
                                done++;
                            }
                        }
                    }
                }
            }
        }
        depth++;
    }
}

void generateSlice_URtoDF_Parity_Prun() {
    std::fill_n(CoordCubeTables::Slice_URtoDF_Parity_Prun, N_SLICE2 * N_URtoDF * N_PARITY / 2, 0xff);
    int depth = 0;
    setPruning(CoordCubeTables::Slice_URtoDF_Parity_Prun, 0, 0);
    int done = 1;
    while (done != N_SLICE2 * N_URtoDF * N_PARITY) {
        for (int i = 0; i < N_SLICE2 * N_URtoDF * N_PARITY; i++) {
            int parity = i % 2;
            int URtoDF = (i / 2) / N_SLICE2;
            int slice = (i / 2) % N_SLICE2;
            if (getPruning(CoordCubeTables::Slice_URtoDF_Parity_Prun, i) == depth) {
                for (int j = 0; j < 18; j++) {
                    switch (j) {
                        case 3: case 5: case 6: case 8: case 12: case 14: case 15: case 17:
                            continue;
                        default: {
                            int newSlice = CoordCubeTables::FRtoBR_Move[slice][j];
                            int newURtoDF = CoordCubeTables::URtoDF_Move[URtoDF][j];
                            int newParity = CoordCubeTables::parityMove[parity][j];
                            int idx = (N_SLICE2 * newURtoDF + newSlice) * 2 + newParity;
                            if (getPruning(CoordCubeTables::Slice_URtoDF_Parity_Prun, idx) == 0x0f) {
                                setPruning(CoordCubeTables::Slice_URtoDF_Parity_Prun, idx, depth + 1);
                                done++;
                            }
                        }
                    }
                }
            }
        }
        depth++;
    }
}

void generateSlice_Twist_Prun() {
    std::fill_n(CoordCubeTables::Slice_Twist_Prun, (N_SLICE1 * N_TWIST) / 2 + 1, 0xff);
    int depth = 0;
    setPruning(CoordCubeTables::Slice_Twist_Prun, 0, 0);
    int done = 1;
    while (done != N_SLICE1 * N_TWIST) {
        for (int i = 0; i < N_SLICE1 * N_TWIST; i++) {
            int twist = i / N_SLICE1;
            int slice = i % N_SLICE1;
            if (getPruning(CoordCubeTables::Slice_Twist_Prun, i) == depth) {
                for (int j = 0; j < 18; j++) {
                    int newSlice = CoordCubeTables::FRtoBR_Move[slice * 24][j] / 24;
                    int newTwist = CoordCubeTables::twistMove[twist][j];
                    int idx = N_SLICE1 * newTwist + newSlice;
                    if (getPruning(CoordCubeTables::Slice_Twist_Prun, idx) == 0x0f) {
                        setPruning(CoordCubeTables::Slice_Twist_Prun, idx, depth + 1);
                        done++;
                    }
                }
            }
        }
        depth++;
    }
}

void generateSlice_Flip_Prun() {
    std::fill_n(CoordCubeTables::Slice_Flip_Prun, N_SLICE1 * N_FLIP / 2, 0xff);
    int depth = 0;
    setPruning(CoordCubeTables::Slice_Flip_Prun, 0, 0);
    int done = 1;
    while (done != N_SLICE1 * N_FLIP) {
        for (int i = 0; i < N_SLICE1 * N_FLIP; i++) {
            int flip = i / N_SLICE1;
            int slice = i % N_SLICE1;
            if (getPruning(CoordCubeTables::Slice_Flip_Prun, i) == depth) {
                for (int j = 0; j < 18; j++) {
                    int newSlice = CoordCubeTables::FRtoBR_Move[slice * 24][j] / 24;
                    int newFlip = CoordCubeTables::flipMove[flip][j];
                    int idx = N_SLICE1 * newFlip + newSlice;
                    if (getPruning(CoordCubeTables::Slice_Flip_Prun, idx) == 0x0f) {
                        setPruning(CoordCubeTables::Slice_Flip_Prun, idx, depth + 1);
                        done++;
                    }
                }
            }
        }
        depth++;
    }
}

void generateTables() {
    generateTwistMove();
    generateFlipMove();
    generateFRtoBR_Move();
    generateURFtoDLF_Move();
    generateURtoDF_Move();
    generateURtoUL_Move();
    generateUBtoDF_Move();
    generateMergeURtoULandUBtoDF();
    generateSlice_URFtoDLF_Parity_Prun();
    generateSlice_URtoDF_Parity_Prun();
    generateSlice_Twist_Prun();
    generateSlice_Flip_Prun();
}

// ==========================================
// CoordCube Implementation
// ==========================================

CoordCube::CoordCube(const CubieCube& c) {
    twist = c.getTwist();
    flip = c.getFlip();
    parity = c.cornerParity();
    FRtoBR = c.getFRtoBR();
    URFtoDLF = c.getURFtoDLF();
    URtoUL = c.getURtoUL();
    UBtoDF = c.getUBtoDF();
    URtoDF = c.getURtoDF();
}

void CoordCube::move(int m) {
    twist = CoordCubeTables::twistMove[twist][m];
    flip = CoordCubeTables::flipMove[flip][m];
    parity = CoordCubeTables::parityMove[parity][m];
    FRtoBR = CoordCubeTables::FRtoBR_Move[FRtoBR][m];
    URFtoDLF = CoordCubeTables::URFtoDLF_Move[URFtoDLF][m];
    URtoUL = CoordCubeTables::URtoUL_Move[URtoUL][m];
    UBtoDF = CoordCubeTables::UBtoDF_Move[UBtoDF][m];
    if (URtoUL < 336 && UBtoDF < 336) {
        URtoDF = CoordCubeTables::MergeURtoULandUBtoDF[URtoUL][UBtoDF];
    }
}
