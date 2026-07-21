#include "cubiecube.h"
#include <algorithm>
#include <stdexcept>

// Helper n choose k
int Cnk(int n, int k) {
    if (n < k) return 0;
    if (k > n / 2) k = n - k;
    int s = 1, i = n, j = 1;
    for (; i != n - k; i--, j++) {
        s *= i;
        s /= j;
    }
    return s;
}

// ==========================================
// FaceCube Implementation
//  inke struct cubiecube m defined hai 
// ==========================================

FaceCube::FaceCube(const std::string& s) {
    std::string str = s.empty() ? "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB" : s;
    for (int i = 0; i < 54; i++) {
        f[i] = charToColor(str[i]);
    }
}

std::string FaceCube::toString() const {
    std::string s = "";
    for (int i = 0; i < 54; i++) {
        s += ColorChar[f[i]];
    }
    return s;
}

CubieCube FaceCube::toCubieCube() const {
    CubieCube ccRet;
    for (int i = 0; i < 8; i++) ccRet.cp[i] = URF;
    for (int i = 0; i < 12; i++) ccRet.ep[i] = UR;
    int ori;
    Color col1, col2;
    for (int i = 0; i < 8; i++) { // For each corner position
        for(ori = 0; ori < 3; ori++) {
            if (f[cornerFacelet[i][ori]] == U || f[cornerFacelet[i][ori]] == D)
                break;
        }
        col1 = f[cornerFacelet[i][(ori + 1) % 3]];
        col2 = f[cornerFacelet[i][(ori + 2) % 3]];

        for (int j = 0; j < 8; j++) { // For each corner cubie
            if (col1 == cornerColor[j][1] && col2 == cornerColor[j][2]) {
                ccRet.cp[i] = static_cast<Corner>(j);
                ccRet.co[i] = ori % 3;
                break;
            }
        }
    }
    for (int i = 0; i < 12; i++) { // For each edge position
        for (int j = 0; j < 12; j++) { // For each edge cubie
            if (f[edgeFacelet[i][0]] == edgeColor[j][0] &&
                f[edgeFacelet[i][1]] == edgeColor[j][1]) {
                ccRet.ep[i] = static_cast<Edge>(j);
                ccRet.eo[i] = 0;
                break;
            }
            if (f[edgeFacelet[i][0]] == edgeColor[j][1] &&
                f[edgeFacelet[i][1]] == edgeColor[j][0]) {
                ccRet.ep[i] = static_cast<Edge>(j);
                ccRet.eo[i] = 1;
                break;
            }
        }
    }
    return ccRet;
}

// ==========================================
// CubieCube Implementation
// ==========================================

CubieCube::CubieCube() {
    // yaha check ho rha h ki if all are at there perfect positions or they are not.
    //cp_init correct vali hai or cp ye match krenge and dusri orientation hai 

    Corner cp_init[8] = { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
    for (int i = 0; i < 8; i++) {
        cp[i] = cp_init[i];
        co[i] = 0;
    }


    Edge ep_init[12] = { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };
    for (int i = 0; i < 12; i++) {
        ep[i] = ep_init[i];
        eo[i] = 0;
    }

}

CubieCube::CubieCube(const Corner cp_[8], const int8_t co_[8], const Edge ep_[12], const int8_t eo_[12]) {
    for (int i = 0; i < 8; i++) {
        cp[i] = cp_[i];
        co[i] = co_[i];
    }
    for (int i = 0; i < 12; i++) {
        ep[i] = ep_[i];
        eo[i] = eo_[i];
    }
}

void CubieCube::rotateLeft(int arr[], int l, int r) {
    int temp = arr[l];
    for (int i = l; i < r; i++) arr[i] = arr[i + 1];
    arr[r] = temp;
}

void CubieCube::rotateLeft(Corner arr[], int l, int r) {
    Corner temp = arr[l];
    for (int i = l; i < r; i++) arr[i] = arr[i + 1];
    arr[r] = temp;
}

void CubieCube::rotateLeft(Edge arr[], int l, int r) {
    Edge temp = arr[l];
    for (int i = l; i < r; i++) arr[i] = arr[i + 1];
    arr[r] = temp;
}

void CubieCube::rotateRight(int arr[], int l, int r) {
    int temp = arr[r];
    for (int i = r; i > l; i--) arr[i] = arr[i - 1];
    arr[l] = temp;
}

void CubieCube::rotateRight(Corner arr[], int l, int r) {
    Corner temp = arr[r];
    for (int i = r; i > l; i--) arr[i] = arr[i - 1];
    arr[l] = temp;
}

void CubieCube::rotateRight(Edge arr[], int l, int r) {
    Edge temp = arr[r];
    for (int i = r; i > l; i--) arr[i] = arr[i - 1];
    arr[l] = temp;
}

FaceCube CubieCube::toFaceCube() const {
    FaceCube fcRet("");
    for (int i = 0; i < 8; i++) {
        int j = cp[i];
        int ori = co[i];
        for (int n = 0; n < 3; n++) {
            fcRet.f[cornerFacelet[i][(n + ori) % 3]] = cornerColor[j][n];
        }
    }
    for (int i = 0; i < 12; i++) {
        int j = ep[i];
        int ori = eo[i];
        for (int n = 0; n < 2; n++) {
            fcRet.f[edgeFacelet[i][(n + ori) % 2]] = edgeColor[j][n];
        }
    }
    return fcRet;
}

void CubieCube::cornerMultiply(const CubieCube& b) {
    Corner cPerm[8];
    int8_t cOri[8];
    for (int corn = 0; corn < 8; corn++) {
        cPerm[corn] = cp[b.cp[corn]];
        int oriA = co[b.cp[corn]];
        int oriB = b.co[corn];
        int ori = 0;
        if (oriA < 3 && oriB < 3) {
            ori = oriA + oriB;
            if (ori >= 3) ori -= 3;
        } else if (oriA < 3 && oriB >= 3) {
            ori = oriA + oriB;
            if (ori >= 6) ori -= 3;
        } else if (oriA >= 3 && oriB < 3) {
            ori = oriA - oriB;
            if (ori < 3) ori += 3;
        } else if (oriA >= 3 && oriB >= 3) {
            ori = oriA - oriB;
            if (ori < 0) ori += 3;
        }
        cOri[corn] = ori;
    }
    for (int c = 0; c < 8; c++) {
        cp[c] = cPerm[c];
        co[c] = cOri[c];
    }
}

void CubieCube::edgeMultiply(const CubieCube& b) {
    Edge ePerm[12];
    int8_t eOri[12];
    for (int edge = 0; edge < 12; edge++) {
        ePerm[edge] = ep[b.ep[edge]];
        eOri[edge] = (b.eo[edge] + eo[b.ep[edge]]) % 2;
    }
    for (int e = 0; e < 12; e++) {
        ep[e] = ePerm[e];
        eo[e] = eOri[e];
    }
}

void CubieCube::multiply(const CubieCube& b) {
    cornerMultiply(b);
    // Matches original solverWorker.js which only multiplies corners in multiply()
}

void CubieCube::invCubieCube(CubieCube& c) const {
    for (int edge = 0; edge < 12; edge++) {
        c.ep[ep[edge]] = static_cast<Edge>(edge);
    }
    for (int edge = 0; edge < 12; edge++) {
        c.eo[edge] = eo[c.ep[edge]];
    }
    for (int corn = 0; corn < 8; corn++) {
        c.cp[cp[corn]] = static_cast<Corner>(corn);
    }
    for (int corn = 0; corn < 8; corn++) {
        int ori = co[c.cp[corn]];
        if (ori >= 3) {
            c.co[corn] = ori;
        } else {
            c.co[corn] = -ori;
            if (c.co[corn] < 0) c.co[corn] += 3;
        }
    }
}

int CubieCube::getTwist() const {
    int ret = 0;
    for (int i = URF; i < DRB; i++) {
        ret = 3 * ret + co[i];
    }
    return ret;
}

void CubieCube::setTwist(int twist) {
    int twistParity = 0;
    for (int i = DRB - 1; i >= URF; i--) {
        co[i] = twist % 3;
        twistParity += co[i];
        twist /= 3;
    }
    co[DRB] = (3 - (twistParity % 3)) % 3;
}

int CubieCube::getFlip() const {
    int ret = 0;
    for (int i = UR; i < BR; i++) {
        ret = 2 * ret + eo[i];
    }
    return ret;
}

void CubieCube::setFlip(int flip) {
    int flipParity = 0;
    for (int i = BR - 1; i >= UR; i--) {
        eo[i] = flip % 2;
        flipParity += eo[i];
        flip /= 2;
    }
    eo[BR] = (2 - (flipParity % 2)) % 2;
}

int CubieCube::cornerParity() const {
    int s = 0;
    for (int i = DRB; i >= URF + 1; i--) {
        for (int j = i - 1; j >= URF; j--) {
            if (cp[j] > cp[i]) s++;
        }
    }
    return s % 2;
}

int CubieCube::edgeParity() const {
    int s = 0;
    for (int i = BR; i >= UR + 1; i--) {
        for (int j = i - 1; j >= UR; j--) {
            if (ep[j] > ep[i]) s++;
        }
    }
    return s % 2;
}

int CubieCube::getFRtoBR() const {
    int a = 0, x = 0;
    int edge4[4];
    for (int j = BR; j >= UR; j--) {
        if (FR <= ep[j] && ep[j] <= BR) {
            a += Cnk(11 - j, x + 1);
            edge4[3 - x++] = ep[j];
        }
    }
    int b = 0;
    for (int j = 3; j > 0; j--) {
        int k = 0;
        while (edge4[j] != j + 8) {
            // Rotate left edge4 between 0 and j
            int temp = edge4[0];
            for (int i = 0; i < j; i++) edge4[i] = edge4[i + 1];
            edge4[j] = temp;
            k++;
        }
        b = (j + 1) * b + k;
    }
    return 24 * a + b;
}

void CubieCube::setFRtoBR(int idx) {
    int x;
    int sliceEdge[4] = { FR, FL, BL, BR };
    int otherEdge[8] = { UR, UF, UL, UB, DR, DF, DL, DB };
    int b = idx % 24;
    int a = idx / 24;
    for (int e = 0; e < 12; e++) {
        ep[e] = DB;
    }
    for (int j = 1; j < 4; j++) {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0) {
            int temp = sliceEdge[j];
            for (int i = j; i > 0; i--) sliceEdge[i] = sliceEdge[i - 1];
            sliceEdge[0] = temp;
        }
    }
    x = 3;
    for (int j = UR; j <= BR; j++) {
        if (a - Cnk(11 - j, x + 1) >= 0) {
            ep[j] = static_cast<Edge>(sliceEdge[3 - x]);
            a -= Cnk(11 - j, x-- + 1);
        }
    }
    x = 0;
    for (int j = UR; j <= BR; j++) {
        if (ep[j] == DB) ep[j] = static_cast<Edge>(otherEdge[x++]);
    }
}

int CubieCube::getURFtoDLF() const {
    int a = 0, x = 0;
    int corner6[6];
    for (int j = URF; j <= DRB; j++) {
        if (cp[j] <= DLF) {
            a += Cnk(j, x + 1);
            corner6[x++] = cp[j];
        }
    }
    int b = 0;
    for (int j = 5; j > 0; j--) {
        int k = 0;
        while (corner6[j] != j) {
            int temp = corner6[0];
            for (int i = 0; i < j; i++) corner6[i] = corner6[i + 1];
            corner6[j] = temp;
            k++;
        }
        b = (j + 1) * b + k;
    }
    return 720 * a + b;
}

void CubieCube::setURFtoDLF(int idx) {
    int x;
    int corner6[6] = { URF, UFL, ULB, UBR, DFR, DLF };
    int otherCorner[2] = { DBL, DRB };
    int b = idx % 720;
    int a = idx / 720;
    for (int c = 0; c < 8; c++) {
        cp[c] = DRB;
    }
    for (int j = 1; j < 6; j++) {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0) {
            int temp = corner6[j];
            for (int i = j; i > 0; i--) corner6[i] = corner6[i - 1];
            corner6[0] = temp;
        }
    }
    x = 5;
    for (int j = DRB; j >= 0; j--) {
        if (a - Cnk(j, x + 1) >= 0) {
            cp[j] = static_cast<Corner>(corner6[x]);
            a -= Cnk(j, x-- + 1);
        }
    }
    x = 0;
    for (int j = URF; j <= DRB; j++) {
        if (cp[j] == DRB) cp[j] = static_cast<Corner>(otherCorner[x++]);
    }
}

int CubieCube::getURtoDF() const {
    int a = 0, x = 0;
    int edge6[6];
    for (int j = UR; j <= BR; j++) {
        if (ep[j] <= DF) {
            a += Cnk(j, x + 1);
            edge6[x++] = ep[j];
        }
    }
    int b = 0;
    for (int j = 5; j > 0; j--) {
        int k = 0;
        while (edge6[j] != j) {
            int temp = edge6[0];
            for (int i = 0; i < j; i++) edge6[i] = edge6[i + 1];
            edge6[j] = temp;
            k++;
        }
        b = (j + 1) * b + k;
    }
    return 720 * a + b;
}

void CubieCube::setURtoDF(int idx) {
    int x;
    int edge6[6] = { UR, UF, UL, UB, DR, DF };
    int otherEdge[6] = { DL, DB, FR, FL, BL, BR };
    int b = idx % 720;
    int a = idx / 720;
    for (int e = 0; e < 12; e++) {
        ep[e] = BR;
    }
    for (int j = 1; j < 6; j++) {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0) {
            int temp = edge6[j];
            for (int i = j; i > 0; i--) edge6[i] = edge6[i - 1];
            edge6[0] = temp;
        }
    }
    x = 5;
    for (int j = BR; j >= 0; j--) {
        if (a - Cnk(j, x + 1) >= 0) {
            ep[j] = static_cast<Edge>(edge6[x]);
            a -= Cnk(j, x-- + 1);
        }
    }
    x = 0;
    for (int j = UR; j <= BR; j++) {
        if (ep[j] == BR) ep[j] = static_cast<Edge>(otherEdge[x++]);
    }
}

int CubieCube::getURtoUL() const {
    int a = 0, x = 0;
    int edge3[3];
    for (int j = UR; j <= BR; j++) {
        if (ep[j] <= UL) {
            a += Cnk(j, x + 1);
            edge3[x++] = ep[j];
        }
    }
    int b = 0;
    for (int j = 2; j > 0; j--) {
        int k = 0;
        while (edge3[j] != j) {
            int temp = edge3[0];
            for (int i = 0; i < j; i++) edge3[i] = edge3[i + 1];
            edge3[j] = temp;
            k++;
        }
        b = (j + 1) * b + k;
    }
    return 6 * a + b;
}

void CubieCube::setURtoUL(int idx) {
    int x;
    int edge3[3] = { UR, UF, UL };
    int b = idx % 6;
    int a = idx / 6;
    for (int e = 0; e < 12; e++) {
        ep[e] = BR;
    }
    for (int j = 1; j < 3; j++) {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0) {
            int temp = edge3[j];
            for (int i = j; i > 0; i--) edge3[i] = edge3[i - 1];
            edge3[0] = temp;
        }
    }
    x = 2;
    for (int j = BR; j >= 0; j--) {
        if (a - Cnk(j, x + 1) >= 0) {
            ep[j] = static_cast<Edge>(edge3[x]);
            a -= Cnk(j, x-- + 1);
        }
    }
}

int CubieCube::getUBtoDF() const {
    int a = 0, x = 0;
    int edge3[3];
    for (int j = UR; j <= BR; j++) {
        if (UB <= ep[j] && ep[j] <= DF) {
            a += Cnk(j, x + 1);
            edge3[x++] = ep[j];
        }
    }
    int b = 0;
    for (int j = 2; j > 0; j--) {
        int k = 0;
        while (edge3[j] != UB + j) {
            int temp = edge3[0];
            for (int i = 0; i < j; i++) edge3[i] = edge3[i + 1];
            edge3[j] = temp;
            k++;
        }
        b = (j + 1) * b + k;
    }
    return 6 * a + b;
}

void CubieCube::setUBtoDF(int idx) {
    int x;
    int edge3[3] = { UB, DR, DF };
    int b = idx % 6;
    int a = idx / 6;
    for (int e = 0; e < 12; e++) {
        ep[e] = BR;
    }
    for (int j = 1; j < 3; j++) {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0) {
            int temp = edge3[j];
            for (int i = j; i > 0; i--) edge3[i] = edge3[i - 1];
            edge3[0] = temp;
        }
    }
    x = 2;
    for (int j = BR; j >= 0; j--) {
        if (a - Cnk(j, x + 1) >= 0) {
            ep[j] = static_cast<Edge>(edge3[x]);
            a -= Cnk(j, x-- + 1);
        }
    }
}

int CubieCube::getURFtoDLB() const {
    int perm[8];
    int b = 0;
    for (int i = 0; i < 8; i++) perm[i] = cp[i];
    for (int j = 7; j > 0; j--) {
        int k = 0;
        while (perm[j] != j) {
            int temp = perm[0];
            for (int i = 0; i < j; i++) perm[i] = perm[i + 1];
            perm[j] = temp;
            k++;
        }
        b = (j + 1) * b + k;
    }
    return b;
}

void CubieCube::setURFtoDLB(int idx) {
    int perm[8] = { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
    int k;
    for (int j = 1; j < 8; j++) {
        k = idx % (j + 1);
        idx /= j + 1;
        while (k-- > 0) {
            int temp = perm[j];
            for (int i = j; i > 0; i--) perm[i] = perm[i - 1];
            perm[0] = temp;
        }
    }
    int x = 7;
    for (int j = 7; j >= 0; j--) cp[j] = static_cast<Corner>(perm[x--]);
}

int CubieCube::getURtoBR() const {
    int perm[12];
    int b = 0;
    for (int i = 0; i < 12; i++) perm[i] = ep[i];
    for (int j = 11; j > 0; j--) {
        int k = 0;
        while (perm[j] != j) {
            int temp = perm[0];
            for (int i = 0; i < j; i++) perm[i] = perm[i + 1];
            perm[j] = temp;
            k++;
        }
        b = (j + 1) * b + k;
    }
    return b;
}

void CubieCube::setURtoBR(int idx) {
    int perm[12] = { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };
    int k;
    for (int j = 1; j < 12; j++) {
        k = idx % (j + 1);
        idx /= j + 1;
        while (k-- > 0) {
            int temp = perm[j];
            for (int i = j; i > 0; i--) perm[i] = perm[i - 1];
            perm[0] = temp;
        }
    }
    int x = 11;
    for (int j = 11; j >= 0; j--) ep[j] = static_cast<Edge>(perm[x--]);
}

int CubieCube::verify() const {
    int sum = 0;
    int edgeCount[12] = { 0 };
    for (int i = 0; i < 12; i++) {
        edgeCount[ep[i]]++;
    }
    for (int i = 0; i < 12; i++) {
        if (edgeCount[i] != 1) return -2;
    }

    for (int i = 0; i < 12; i++) sum += eo[i];
    if (sum % 2 != 0) return -3;

    int cornerCount[8] = { 0 };
    for (int i = 0; i < 8; i++) {
        cornerCount[cp[i]]++;
    }
    for (int i = 0; i < 8; i++) {
        if (cornerCount[i] != 1) return -4;
    }

    sum = 0;
    for (int i = 0; i < 8; i++) sum += co[i];
    if (sum % 3 != 0) return -5;

    if ((edgeParity() ^ cornerParity()) != 0) return -6;

    return 0;
}

int CubieCube::getURtoDF(int idx1, int idx2) {
    CubieCube a;
    CubieCube b;
    a.setURtoUL(idx1);
    b.setUBtoDF(idx2);
    for (int i = 0; i < 8; i++) {
        if (a.ep[i] != BR) {
            if (b.ep[i] != BR) {
                return -1;
            } else {
                b.ep[i] = a.ep[i];
            }
        }
    }
    return b.getURtoDF();
}

const CubieCube CubieCube::moveCube[6] = {
    // U move
    CubieCube(
        // agar up move kiya 
        (const Corner[]){ UBR, URF, UFL, ULB, DFR, DLF, DBL, DRB },
        (const int8_t[]){ 0, 0, 0, 0, 0, 0, 0, 0 },
        (const Edge[]){ UB, UR, UF, UL, DR, DF, DL, DB, FR, FL, BL, BR },
        (const int8_t[]){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    ),
    // R move
    CubieCube(
        (const Corner[]){ DFR, UFL, ULB, URF, DRB, DLF, DBL, UBR },
        (const int8_t[]){ 2, 0, 0, 1, 1, 0, 0, 2 },
        (const Edge[]){ FR, UF, UL, UB, BR, DF, DL, DB, DR, FL, BL, UR },
        (const int8_t[]){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    ),
    // F move
    CubieCube(
        (const Corner[]){ UFL, DLF, ULB, UBR, URF, DFR, DBL, DRB },
        (const int8_t[]){ 1, 2, 0, 0, 2, 1, 0, 0 },
        (const Edge[]){ UR, FL, UL, UB, DR, FR, DL, DB, UF, DF, BL, BR },
        (const int8_t[]){ 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 }
    ),
    // D move
    CubieCube(
        (const Corner[]){ URF, UFL, ULB, UBR, DLF, DBL, DRB, DFR },
        (const int8_t[]){ 0, 0, 0, 0, 0, 0, 0, 0 },
        (const Edge[]){ UR, UF, UL, UB, DF, DL, DB, DR, FR, FL, BL, BR },
        (const int8_t[]){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    ),
    // L move
    CubieCube(
        (const Corner[]){ URF, ULB, DBL, UBR, DFR, UFL, DLF, DRB },
        (const int8_t[]){ 0, 1, 2, 0, 0, 2, 1, 0 },
        (const Edge[]){ UR, UF, BL, UB, DR, DF, FL, DB, FR, UL, DL, BR },
        (const int8_t[]){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    ),
    // B move
    CubieCube(
        (const Corner[]){ URF, UFL, UBR, DRB, DFR, DLF, ULB, DBL },
        (const int8_t[]){ 0, 0, 1, 2, 0, 0, 2, 1 },
        (const Edge[]){ UR, UF, UL, BR, DR, DF, DL, BL, FR, FL, UB, DB },
        (const int8_t[]){ 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 }
    )
};
