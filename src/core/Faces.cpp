//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2026-01-26 17:42:17 taubin>
//------------------------------------------------------------------------
//
// Faces.cpp
//
// Written by: Samson Tsegai
//
// Software developed for the course
// Digital Geometry Processing
// Copyright (c) 2026, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <unordered_set>
#include <stdexcept>
#include <math.h>
#include "Faces.hpp"

Faces::Faces(const int nV, const vector<int>& coordIndex) :
    _nV(nV), _nF(0), _nC(static_cast<int>(coordIndex.size())), _coordIndex(coordIndex)
{
    try {
        if (_nV <= 0) {
            throw std::invalid_argument("Invalid number of vertices in Faces class");
        }

        if (_coordIndex.empty()) {
            throw std::invalid_argument("Empty coordIndex in Faces class");
        }

        unordered_set<int> face;

        for (int i = 0; i < _coordIndex.size(); ++i) {
            int v = _coordIndex[i];

            if (v < -1) {
                throw std::invalid_argument("Invalid coord index in Faces class");
            }

            if (v == -1) {
                if (face.size() < 3) {
                    throw std::invalid_argument("Face has less than 3 vertices");
                }
                face.clear();

                _coordIndex[i] = -(++_nF);
            } else {
                if (!face.insert(v).second) {
                    throw std::invalid_argument("Face has repeated vertices");
                }

                if (face.size() == 1) _firstCornerFace.push_back(i);

                if (v == _nV) _nV++;
            }
        }
    } catch (exception& e) {
        fprintf(stderr,"ERROR | %s\n", e.what());
    }

}

int Faces::getNumberOfVertices() const {
    return _nV;
}

int Faces::getNumberOfFaces() const {
    return _nF;
}

int Faces::getNumberOfCorners() const {
    return _nC;
}

int Faces::getFaceSize(const int iF) const {
    int iC = getFaceFirstCorner(iF);
    if (iC < 0) return 0;

    int size = 0;
    for ( ; _coordIndex[iC] >= 0; iC++) size++;
    return size;
}

int Faces::getFaceFirstCorner(const int iF) const {
    return iF >= 0 && iF < _nF ? _firstCornerFace[iF] : -1;
}

int Faces::getFaceVertex(const int iF, const int j) const {
    int iC = getFaceFirstCorner(iF);
    return (iC >= 0 && j >= 0 && j < getFaceSize(iF)) ?
               _coordIndex[iC + j] : -1;
}

int Faces::getCornerFace(const int iC) const {
    if (iC >= 0 && iC < _nC && _coordIndex[iC] > -1) {
        for (int i = iC + 1; i < _nC; i++) {
            if (_coordIndex[i] < 0) return -_coordIndex[i] - 1;
        }
    }
    return -1;
}

int Faces::getNextCorner(const int iC) const {
    if (iC >= 0 && iC < _nC && _coordIndex[iC] > -1) {
        return (_coordIndex[iC + 1] < 0) ?
                   getFaceFirstCorner(getCornerFace(iC)) :
                   iC + 1;
    }
    return -1;
}

