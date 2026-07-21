#pragma once
#include <string>
// using namespace std;

struct Search {
  int ax[31];
  int po[31];

  int flip[31];
  int twist[31];
  int slice[31];

  int parity[31];
  int URFtoDLF[31];
  int FRtoBR[31];
  int URtoUL[31];
  int UBtoDF[31];
  int URtoDF[31];

  int minDistPhase1[31];
  int minDistPhase2[31];

  std::string solutionToString(int length, int depthPhase1 = -1);
  int totalDepth(int depthPhase1, int maxDepth);
  std ::string solution(const std ::string &facelets, int maxDepth = 22,
                        int timeOut = 10, bool useSeparator = false);
};
