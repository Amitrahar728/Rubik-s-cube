#include "search.h"
#include "enums.h"
#include "cubiecube.h"
#include "coordcube.h"
#include <chrono>
#include <algorithm>
#include <cmath>

std::string Search::solutionToString(int length, int depthPhase1) {
    std::string s = "";
    for (int i = 0; i < length; i++) {
        switch (ax[i]) {
            case 0: s += "U"; break;
            case 1: s += "R"; break;
            case 2: s += "F"; break;
            case 3: s += "D"; break;
            case 4: s += "L"; break;
            case 5: s += "B"; break;
        }
        switch (po[i]) {
            case 1: s += " "; break;
            case 2: s += "2 "; break;
            case 3: s += "' "; break;
        }
        if (i == depthPhase1 - 1) {
            s += ". ";
        }
    }
    // Remove trailing space
    if (!s.empty() && s.back() == ' ') {
        s.pop_back();
    }
    return s;
}

int Search::totalDepth(int depthPhase1, int maxDepth) {
    int mv = 0, d1 = 0, d2 = 0;
    int maxDepthPhase2 = std::min(10, maxDepth - depthPhase1);
    for (int i = 0; i < depthPhase1; i++) {
        mv = 3 * ax[i] + po[i] - 1;
        URFtoDLF[i + 1] = CoordCubeTables::URFtoDLF_Move[URFtoDLF[i]][mv];
        FRtoBR[i + 1] = CoordCubeTables::FRtoBR_Move[FRtoBR[i]][mv];
        parity[i + 1] = CoordCubeTables::parityMove[parity[i]][mv];
    }

    d1 = getPruning(
        CoordCubeTables::Slice_URFtoDLF_Parity_Prun,
        (N_SLICE2 * URFtoDLF[depthPhase1] + FRtoBR[depthPhase1]) * 2 + parity[depthPhase1]
    );
    if (d1 > maxDepthPhase2) return -1;

    for (int i = 0; i < depthPhase1; i++) {
        mv = 3 * ax[i] + po[i] - 1;
        URtoUL[i + 1] = CoordCubeTables::URtoUL_Move[URtoUL[i]][mv];
        UBtoDF[i + 1] = CoordCubeTables::UBtoDF_Move[UBtoDF[i]][mv];
    }

    URtoDF[depthPhase1] = CoordCubeTables::MergeURtoULandUBtoDF[URtoUL[depthPhase1]][UBtoDF[depthPhase1]];

    d2 = getPruning(
        CoordCubeTables::Slice_URtoDF_Parity_Prun,
        (N_SLICE2 * URtoDF[depthPhase1] + FRtoBR[depthPhase1]) * 2 + parity[depthPhase1]
    );
    if (d2 > maxDepthPhase2) return -1;

    minDistPhase2[depthPhase1] = std::max(d1, d2);
    if (minDistPhase2[depthPhase1] == 0) return depthPhase1;

    int depthPhase2 = 1;
    int n = depthPhase1;
    bool busy = false;
    po[depthPhase1] = 0;
    ax[depthPhase1] = 0;
    minDistPhase2[n + 1] = 1;

    do {
        do {
            if (depthPhase1 + depthPhase2 - n > minDistPhase2[n + 1] && !busy) {
                if (ax[n] == 0 || ax[n] == 3) {
                    ax[++n] = 1;
                    po[n] = 2;
                } else {
                    ax[++n] = 0;
                    po[n] = 1;
                }
            } else if (
                (ax[n] == 0 || ax[n] == 3)
                    ? ++po[n] > 3
                    : (po[n] = po[n] + 2) > 3
            ) {
                do {
                    if (++ax[n] > 5) {
                        if (n == depthPhase1) {
                            if (depthPhase2 >= maxDepthPhase2) return -1;
                            else {
                                depthPhase2++;
                                ax[n] = 0;
                                po[n] = 1;
                                busy = false;
                                break;
                            }
                        } else {
                            n--;
                            busy = true;
                            break;
                        }
                    } else {
                        if (ax[n] == 0 || ax[n] == 3) po[n] = 1;
                        else po[n] = 2;
                        busy = false;
                    }
                } while (
                    n != depthPhase1 &&
                    (ax[n - 1] == ax[n] || ax[n - 1] - 3 == ax[n])
                );
            } else {
                busy = false;
            }
        } while (busy);

        mv = 3 * ax[n] + po[n] - 1;
        URFtoDLF[n + 1] = CoordCubeTables::URFtoDLF_Move[URFtoDLF[n]][mv];
        FRtoBR[n + 1] = CoordCubeTables::FRtoBR_Move[FRtoBR[n]][mv];
        parity[n + 1] = CoordCubeTables::parityMove[parity[n]][mv];
        URtoDF[n + 1] = CoordCubeTables::URtoDF_Move[URtoDF[n]][mv];

        minDistPhase2[n + 1] = std::max(
            getPruning(
                CoordCubeTables::Slice_URtoDF_Parity_Prun,
                (N_SLICE2 * URtoDF[n + 1] + FRtoBR[n + 1]) * 2 + parity[n + 1]
            ),
            getPruning(
                CoordCubeTables::Slice_URFtoDLF_Parity_Prun,
                (N_SLICE2 * URFtoDLF[n + 1] + FRtoBR[n + 1]) * 2 + parity[n + 1]
            )
        );
    } while (minDistPhase2[n + 1] != 0);

    return depthPhase1 + depthPhase2;
}

std::string Search::solution(const std::string& facelets, int maxDepth, int timeOut, bool useSeparator) {
    int count[6] = { 0 };
    try {
        for (int i = 0; i < 54; i++) {
            count[charToColor(facelets[i])]++;
        }
    } catch (...) {
        return "Error 1";
    }
    for (int i = 0; i < 6; i++) {
        if (count[i] != 9) return "Error 1";
    }

    FaceCube fc(facelets);
    CubieCube cc = fc.toCubieCube();
    int verify_status = cc.verify();
    if (verify_status != 0) {
        return "Error " + std::to_string(std::abs(verify_status));
    }

    CoordCube c(cc);

    po[0] = 0;
    ax[0] = 0;
    flip[0] = c.flip;
    twist[0] = c.twist;
    parity[0] = c.parity;
    slice[0] = c.FRtoBR / 24;
    URFtoDLF[0] = c.URFtoDLF;
    FRtoBR[0] = c.FRtoBR;
    URtoUL[0] = c.URtoUL;
    UBtoDF[0] = c.UBtoDF;

    minDistPhase1[1] = 1;
    int mv = 0, n = 0;
    bool busy = false;
    int depthPhase1 = 1;

    auto tStart = std::chrono::steady_clock::now();

    do {
        do {
            if (depthPhase1 - n > minDistPhase1[n + 1] && !busy) {
                if (ax[n] == 0 || ax[n] == 3) {
                    ax[++n] = 1;
                } else {
                    ax[++n] = 0;
                }
                po[n] = 1;
            } else if (++po[n] > 3) {
                do {
                    if (++ax[n] > 5) {
                        auto now = std::chrono::steady_clock::now();
                        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - tStart).count();
                        if (elapsed_ms > (timeOut << 10)) {
                            return "Error 8";
                        }

                        if (n == 0) {
                            if (depthPhase1 >= maxDepth) return "Error 7";
                            else {
                                depthPhase1++;
                                ax[n] = 0;
                                po[n] = 1;
                                busy = false;
                                break;
                            }
                        } else {
                            n--;
                            busy = true;
                            break;
                        }
                    } else {
                        po[n] = 1;
                        busy = false;
                    }
                } while (
                    n != 0 &&
                    (ax[n - 1] == ax[n] || ax[n - 1] - 3 == ax[n])
                );
            } else {
                busy = false;
            }
        } while (busy);

        mv = 3 * ax[n] + po[n] - 1;
        flip[n + 1] = CoordCubeTables::flipMove[flip[n]][mv];
        twist[n + 1] = CoordCubeTables::twistMove[twist[n]][mv];
        slice[n + 1] = CoordCubeTables::FRtoBR_Move[slice[n] * 24][mv] / 24;
        minDistPhase1[n + 1] = std::max(
            getPruning(
                CoordCubeTables::Slice_Flip_Prun,
                N_SLICE1 * flip[n + 1] + slice[n + 1]
            ),
            getPruning(
                CoordCubeTables::Slice_Twist_Prun,
                N_SLICE1 * twist[n + 1] + slice[n + 1]
            )
        );

        if (minDistPhase1[n + 1] == 0 && n >= depthPhase1 - 5) {
            minDistPhase1[n + 1] = 10;
            if (n == depthPhase1 - 1) {
                int s = totalDepth(depthPhase1, maxDepth);
                if (s >= 0) {
                    if (s == depthPhase1 ||
                        (ax[depthPhase1 - 1] != ax[depthPhase1] &&
                         ax[depthPhase1 - 1] != ax[depthPhase1] + 3)) {
                        return useSeparator
                            ? solutionToString(s, depthPhase1)
                            : solutionToString(s);
                    }
                }
            }
        }
    } while (true);
}
