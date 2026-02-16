//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2026-01-26 17:42:17 taubin>
//------------------------------------------------------------------------
//
// LoaderStl.cpp
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

#include <stdio.h>
#include "TokenizerFile.hpp"
#include "LoaderStl.hpp"
#include "StrException.hpp"

#include "wrl/Shape.hpp"
#include "wrl/Appearance.hpp"
#include "wrl/Material.hpp"
#include "wrl/IndexedFaceSet.hpp"

// reference
// https://en.wikipedia.org/wiki/STL_(file_format)

const char* LoaderStl::_ext = "stl";

bool LoaderStl::loadFace(Tokenizer& tkn, IndexedFaceSet& ifs) {
    bool success = false;

    vector<int>& coordIndex = ifs.getCoordIndex();
    vector<float>& coord = ifs.getCoord();
    vector<float>& normal = ifs.getNormal();

    if (tkn.equals("facet") && tkn.expecting("normal")) {
        Vec3f n;
        if (!tkn.getVec3f(n)) {
            throw new StrException("expecting Vec3f");
        }
        normal.push_back(n.x);
        normal.push_back(n.y);
        normal.push_back(n.z);
    } else {
        throw new StrException("expected \"facet normal\"");
    }

    if (tkn.expecting("outer") && tkn.expecting("loop")) {
        while (tkn.expecting("vertex")) {
            Vec3f v;
            if (!tkn.getVec3f(v)) {
                throw new StrException("expecting Vec3f");
            }

            string k = to_string(v.x) + to_string(v.y) + to_string(v.z);

            if (vertexToIdx.find(k) == vertexToIdx.end()) {
                int idx = static_cast<int>(coord.size() / 3);
                coordIndex.push_back(idx);
                vertexToIdx[k] = idx;
                coord.push_back(v.x);
                coord.push_back(v.y);
                coord.push_back(v.z);
            } else {
                coordIndex.push_back(vertexToIdx[k]);
            }
        }
    } else {
        throw new StrException("expected \"outer loop\"");
    }

    if (tkn.equals("endloop") && tkn.expecting("endfacet")) {
        coordIndex.push_back(-1);
    } else {
        throw new StrException("expected \"endloop\" and \"endfacet\"");
    }

    success = true;
    return success;
}

bool LoaderStl::load(const char* filename, SceneGraph& wrl) {
    bool success = false;

    // clear the scene graph
    wrl.clear();
    wrl.setUrl("");

    FILE* fp = (FILE*)0;
    try {

        // open the file
        if(filename==(char*)0) throw new StrException("filename==null");
        fp = fopen(filename,"r");
        if(fp==(FILE*)0) throw new StrException("fp==(FILE*)0");

        // use the io/Tokenizer class to parse the input ascii file
        TokenizerFile tkn(fp);
        // first token should be "solid"
        if(tkn.expecting("solid") && tkn.get()) {
            string stlName = tkn; // second token should be the solid name

            Shape* s = new Shape();
            wrl.addChild(s);

            Appearance* a = new Appearance();
            s->setAppearance(a);

            Material* m = new Material();
            a->setMaterial(m);

            IndexedFaceSet* ifs = new IndexedFaceSet();
            s->setGeometry(ifs);

            ifs->setNormalPerVertex(false);
            ifs->setName(stlName);

            // the file should contain a list of triangles in the following format

            // facet normal ni nj nk
            //   outer loop
            //     vertex v1x v1y v1z
            //     vertex v2x v2y v2z
            //     vertex v3x v3y v3z
            //   endloop
            // endfacet

            // - run an infinite loop to parse all the faces
            // - write a private method to parse each face within the loop
            // - the method should return true if successful, and false if not
            // - if your method returns tru
            //     update the normal, coord, and coordIndex variables
            // - if your method returns false
            //     throw an StrException explaining why the method failed

            while (tkn.get()) {
                loadFace(tkn, *ifs);
            }

        }

        // close the file (this statement may not be reached)
        fclose(fp);
        success = true;
    
    } catch(StrException* e) {

        if(fp!=(FILE*)0) fclose(fp);
        fprintf(stderr,"ERROR | %s\n",e->what());
        delete e;

    }

    return success;
}

