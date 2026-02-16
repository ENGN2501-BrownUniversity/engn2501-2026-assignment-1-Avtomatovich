//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2026-01-26 17:42:17 taubin>
//------------------------------------------------------------------------
//
// SaverStl.cpp
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

#include "SaverStl.hpp"

#include "StrException.hpp"
#include "wrl/Shape.hpp"
#include "wrl/IndexedFaceSet.hpp"

#include "core/Faces.hpp"

#include <filesystem>

const char* SaverStl::_ext = "stl";

//////////////////////////////////////////////////////////////////////
bool SaverStl::save(const char* filename, SceneGraph& wrl) const {
    bool success = false;
    if (filename != (char*)0) {

        if (wrl.getNumberOfChildren() != 1) return success;
        pNode node = wrl.getChildren().front();

        if (!node->isShape()) return success;
        Shape *pShape = dynamic_cast<Shape*>(node);

        if (!pShape->hasGeometryIndexedFaceSet()) return success;
        IndexedFaceSet *pIfs = dynamic_cast<IndexedFaceSet*>(pShape->getGeometry());

        Faces faces(pIfs->getNumberOfCoord(), pIfs->getCoordIndex());

        if (pIfs->isTriangleMesh() && pIfs->getNormalBinding() == IndexedFaceSet::PB_PER_FACE) {
            FILE* fp = fopen(filename,"w");

            try {

                if(fp != (FILE*)0) {
                    string solidName = pIfs->getName();
                    if (solidName.empty()) {
                        solidName = std::filesystem::path(filename).stem().string();
                    }
                    fprintf(fp,"solid %s\n",solidName.c_str());

                    const vector<float>& normal = pIfs->getNormal();
                    const vector<float>& coord = pIfs->getCoord();

                    int nF = faces.getNumberOfFaces();
                    if (nF == 0) {
                        throw new StrException("Number of faces is 0");
                    }

                    for (int iF = 0; iF < nF; ++iF) {
                        int iN = 3 * iF;
                        fprintf(fp,"facet normal %e %e %e\n",
                                normal.at(iN), normal.at(iN+1), normal.at(iN+2));
                        fprintf(fp,"\touter loop\n");

                        int faceSize = faces.getFaceSize(iF);
                        if (faceSize == 0) {
                            throw new StrException("Face has size of 0");
                        }

                        for (int j = 0; j < faceSize; ++j) {
                            int iV = 3 * faces.getFaceVertex(iF, j);
                            if (iV < 0) {
                                throw new StrException("Coord index out of range");
                            }

                            fprintf(fp,"\t\tvertex %e %e %e\n",
                                    coord.at(iV), coord.at(iV+1), coord.at(iV+2));
                        }

                        fprintf(fp,"\tendloop\n");
                        fprintf(fp,"endfacet\n");
                    }

                    fclose(fp);
                    success = true;
                }

            } catch (StrException* e) {

                if(fp != (FILE*)0) fclose(fp);
                fprintf(stderr,"ERROR | %s\n",e->what());
                delete e;

            }

        }

    }
    return success;
}
