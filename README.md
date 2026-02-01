[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/obZln9h_)
** DGP2026 Digital Geometry Processing **

Assignment 1

0) First of all, configure your development environment.

1) download the **data-A1.zip** file from the course site, and
unzip it in your computer. Don't unzip it within this directory, named
DGP2026-A1, because otherwise git will try to save the large data
files in your git repository, and it may fail due to size limits. It
is rather difficult to fix this problem once you get there.

2) In this directory, DGP2026-A1, in addition to this README file, you
should find the following subdirectories:

  **assets, forms, qt, and src**

The assets, forms, and qt subdirectories contain files which you
should not modify for the moment. We will come back to these
subdirectories later.

In addition, you should create the following two subdirectories:

  **bin**, and **build**

which should be empty. The build processes will populate these two
subdirectories. You can always delete the contents of these subdirectories
and start the build process over.

When you use CMAKE to generate project files for the command line
dgpTest program, you will find those files in the build directory.
When you successfully make the INSTALL target using CMAKE, you should
find the dgpTest command line executable in the bin directory.

When you use Qt Creator to build the interactive application
DGP2026-A1, qmake will create a new build directory with a longer
name. This new directory can be deleted with all its contents to start
the Qt build process over. You should also find the interactive executable
in the build directory. 

The **src** subdirectory contains the following subdirectories:

  **core, io, test, util, wrl,** and **gui**

The src directory and all these subdirectories except for gui,
also contain a CMakeLists.txt file used by CMAKE.

The qt top subdirectory mentioned above contains the following files

DGP2026-A1.pro
DGP2026-A1.pri

used by qmake. Don't modify this files unless you add new files to the
hierarchy.

In Assignment 1, your work should be restricted to the following files

src/gui/GuiStrings.hpp
src/core/Faces.hpp
src/core/Faces.cpp
src/io/LoaderStl.hpp
src/io/LoaderStl.cpp
src/io/SaverStl.hpp
src/io/SaverStl.cpp

3) Before compiling the application for the first time, edit the file
   src/gui/GuiStrings.hpp

In my version, this files contains the definition of the following strings

#define STUDENT_NAME "Gabriel Taubin"

Replace my name by your name in the last string and save the file.

4) Run CMAKE or Qt Creator to create projects files for the
application, and make sure that it compiles without errors.

5) Verify that you can load and display the *.wrl files contained in
the data-A1 directory using the FILE->Load menu button.

6) Using the FILE-Save menu button, save the files with different
*.wrl names, and verify that you can reload the files that you have
created using the FILE->Load button.

7) Implement the core/Faces class. For this you have to edit the
following two files

  core/Faces.hpp
  
  core/Faces.cpp

8) Implement the io/SaverStl class to save triangle meshes in ASCII
STL format. Use the Faces class in your implementation. For this you
have to edit the following two files

  io/SaverStl.hpp
  io/SaverStl.cpp

Read the comments in the files for further instructions

9) Test your implementation by loading the "tetra1npf.wrl" file
and saving it to a file named "tetra1npf.stl" If you are using a Mac, you
can verify that you had created a correct file in Finder. Otherwise,
install the MeshLab application or some other polygon mesh
viewer/editor.

10) Implement the io/LoaderStl class to read ASCII STL files. For this
you have to edit the following two files

  io/LoaderStl.hpp
  io/LoaderStl.cpp

11) Test your implementation by loading the file "tetra1npf.stl" that you
created in the previous step. Save the resulting file again in STL
format and in WRL format, and verify that you can reload the files
that you generate.

12) Repeat steps 9 & 10 with the file "bunny-mesh-npf.wrl"

13) Download some ASCII STL files from the internet, or use OpenSCAD
to generate some ASCII STL files, and verify that you can load them,
and save them both in STL format and in WRL format.

14) using git, submit your work by the deadline.
  a) locate and remove any temporary files created by your editor
  b) delete the contents of the top subdirectories bin and build
  c) delete the file qt/DGP2026-A1.pro.user created by QtCreator
  d) delete the top build directory created by QtCreator
  e) commit all your changes
  f) push your commit to the GitHub repository
