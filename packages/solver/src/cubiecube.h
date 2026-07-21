#pragma once
#include <string>
#include <cstdint>
#include "enums.h"

struct CubieCube;


// two different methods for describing exact same cube type 
// ye pehla type h jo dikhata h input or output kaisa hoga 

struct FaceCube {
    Color f[54];

    FaceCube(const std::string& s = "");
    std::string toString() const;
    CubieCube toCubieCube() const;
};

// math language jo code ko chahiye 8 corners + 12 edges each with a slot and twist 
struct CubieCube {
    Corner cp[8];
    int8_t co[8];
    Edge ep[12];
    int8_t eo[12];

    CubieCube();
    CubieCube(const Corner cp_[8], const int8_t co_[8], const Edge ep_[12], const int8_t eo_[12]);

    void rotateLeft(int arr[], int l, int r);
    void rotateLeft(Corner arr[], int l, int r);
    void rotateLeft(Edge arr[], int l, int r);

    void rotateRight(int arr[], int l, int r);
    void rotateRight(Corner arr[], int l, int r);
    void rotateRight(Edge arr[], int l, int r);

    FaceCube toFaceCube() const;
    void cornerMultiply(const CubieCube& b);
    void edgeMultiply(const CubieCube& b);
    void multiply(const CubieCube& b);
    void invCubieCube(CubieCube& c) const;

    int getTwist() const;
    void setTwist(int twist);
    int getFlip() const;
    void setFlip(int flip);

    int cornerParity() const;
    int edgeParity() const;

    int getFRtoBR() const;
    void setFRtoBR(int idx);
    int getURFtoDLF() const;
    void setURFtoDLF(int idx);
    int getURtoDF() const;
    void setURtoDF(int idx);
    int getURtoUL() const;
    void setURtoUL(int idx);
    int getUBtoDF() const;
    void setUBtoDF(int idx);
    int getURFtoDLB() const;
    void setURFtoDLB(int idx);
    int getURtoBR() const;
    void setURtoBR(int idx);

    int verify() const;

    static int getURtoDF(int idx1, int idx2);

    static const CubieCube moveCube[6];
};
