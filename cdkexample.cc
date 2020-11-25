/*
 * Filename:            cdkexample.cc
 * Date:                11/08/2020
 * Author:              Carlos Valeriano
 * Email:               cdv190000@utdallas.edu
 * Version:             1.0
 * Copyright:           2020, All Rights Reserved
 *
 * Description:
 *
 *      Build and display a small text based GUI matrix using curses
 *      and the CDK.
 */

#include <iostream>
#include <sstream>
#include "cdk.h"
#include "header.h"

/*
 * For grins and giggles, we will define values using the C
 * Preprocessor instead of C or C++ data types.  These symbols (and
 * their values) get inserted into the Preprocessor's symbol table.
 * The names are replaced by their values when seen later in the code.
 */

#define MATRIX_ROWS 5
#define MATRIX_COLS 3
#define BOX_WIDTH 15
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;


int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  BinaryFileHeader* header = getHeader();

  // CDK uses offset 1 and C/C++ use offset 0.  Therefore, we create one more 
  // slot than necessary and ignore the value at location 0.
  const char 		*rowTitles[MATRIX_ROWS+1] = {"IGNORE", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[MATRIX_COLS+1] = {"IGNORE", "a", "b", "c"};
  int		colWidths[MATRIX_COLS+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[MATRIX_COLS+1] = {vMIXED, vMIXED, vMIXED, vMIXED};
 
  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_ROWS, MATRIX_COLS, MATRIX_ROWS, MATRIX_COLS,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, colWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*
   * Dipslay a message
   */
  
  string temp = "";
  
  stringstream aa;
  aa << "Magic: " << header->magicNumber; 
  setCDKMatrixCell(myMatrix, 1, 1, aa.str().c_str());

  stringstream ab;
  ab << "Version: " <<  header->versionNumber;
  setCDKMatrixCell(myMatrix, 1, 2, ab.str().c_str());

  stringstream ac;
  ac << "NumRecords: " << header->numRecords;
  setCDKMatrixCell(myMatrix, 1, 3, ac.str().c_str());

  //use for loop to do the file records
  int maxCount;
  if (header->numRecords > 4)
    maxCount=4;
  else
    maxCount=header->numRecords;


  for (int i = 0; i < maxCount; i++)
    {
      BinaryFileRecord* record = getRecord();

      stringstream x;
      x << "strlen: " << record->strLength;
      setCDKMatrixCell(myMatrix, 2+i, 1, x.str().c_str());

      stringstream y;
      y << record->stringBuffer;
      setCDKMatrixCell(myMatrix, 2+i, 2, y.str().c_str());
    }

  drawCDKMatrix(myMatrix, true);    /* required  */




  /* so we can see results */
   sleep (30);


  // Cleanup screen
  endCDK();
}
