#Sudoku Solver

Load file : choose image file and extract the grid
Solve : solve Sudoku
Help : give one solution (one random number from all numbers which were not yet found)


Dependencies
-----------
* OpenCV
* Tesseract
* clone both git repo
* configure with cmake or script ./configure
* compilation : 
* make : compilation only
* make install : compilation + copy in the system



##Compilation
* Adapt CMakeLists.txt : with correct path to tesseract and opencv
* cmake source_path
- make -j8
- export export TESSDATA_PREFIX=path_language_tesseract
- ./Sudoku

