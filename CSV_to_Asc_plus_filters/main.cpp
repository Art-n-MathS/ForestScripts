#include <iostream>
#include <vector>
#include <map>
#include<string.h>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include <assert.h>
#include <algorithm>
#include <unordered_map>
#include <sys/stat.h>


//-----------------------------------------------------------------------------
/// @file main.cpp
/// @author Dr Milto Miltiadou
/// @version 1.0
/// @date 16/05/19
/// @brief this program takes as input a folder with .csv files and for each
/// @brief one of them creates a .asc geo-image. It also applies salt
/// @brief and pepper filtering and afterwards median filtering for noise
/// @brief reduction. Images at each stage are exported for testing
///
/// @brief CSV_to_Asc_plus_filters
///              -inDir <input Directory>
///              -oAsc <dir for storing unprocessed asc files>
///              -oSnP <dir for storing asc after salt n pepper noise filter>
///              -oMed <dir for storing asc after median filtering>
///              -pcol <column containing pixel values / KNN results>
///              -xCol <column containing the x positions of the pixels>
///              -yCol <column containing the y positions of the pixels>

///
/* example: -inDir "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/KNN_csv_seperated"
            -oAsc "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs"
            -oSnP "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_snp"
            -oMed "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_snp_med"
            -pcol 3 -xCol 3 -yCol 4


  example in a single line:
-inDir "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/KNN_csv_seperated" -oAsc "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs" -oSnP "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_snp" -oMed "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_snp_med" -pcol 3 -xCol 3 -yCol 4
*/
//-----------------------------------------------------------------------------


int main(int argc, char *argv[])
{
   std::map<std::string, int> tags;
   tags["-inDir"] = 1; /// -inDir <input Directory>
   tags["-oAsc" ] = 2; /// -oAsc <dir for storing unprocessed asc files>
   tags["-oSnP" ] = 3; /// -oSnP <dir for storing asc after salt n pepper noise filter>
   tags["-oMed" ] = 4; /// -oMed <dir for storing asc after median filtering>
   tags["-pcol" ] = 5; /// -pcol <column containing pixel values / KNN results>
   tags["-xCol" ] = 6; /// -xCol <column containing the x positions of the pixels>
   tags["-yCol" ] = 7; /// -yCol <column containing the y positions of the pixels>

   std::string inDir(""), oAsc(""), oSnP(""), oMed("");
   unsigned short int pcol(1000),xCol(1000),yCol(1000);


   // parse files
   int argvIndex = 1;
   while(argvIndex<argc)
   {
      switch (tags[argv[argvIndex]])
      {
      case 1: // -inDir <input Directory>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             inDir = argv[argvIndex];
          }
          break;
      }
      case 2: // -oAsc <dir for storing unprocessed asc files>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             oAsc = argv[argvIndex];
          }
          break;
      }
      case 3: // -oSnP <dir for storing asc after salt n pepper noise filter>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             oSnP = argv[argvIndex];
          }
          break;
      }
      case 4: // -oMed <dir for storing asc after median filtering>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             oMed = argv[argvIndex];
          }
          break;
      }
      case 5: // -pcol <column containing pixel values / KNN results>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             pcol = atoi(argv[argvIndex]);
          }
          break;
      }
      case 6: // -xCol <column containing the x positions of the pixels>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             xCol = atoi(argv[argvIndex]);
          }
          break;
      }
      case 7: // -yCol <column containing the y positions of the pixels>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             yCol = atoi(argv[argvIndex]);
          }
          break;
      }
      default:
      {
         std::cerr << "WARNING: Unkown tag: " << argv[argvIndex] << "\n";
         break;
      }
      }
      argvIndex++;
   }
   if(inDir=="" || oAsc=="" || oSnP=="" || oMed=="" || pcol==1000 ||
           xCol==1000 || yCol==1000)
   {
      std::cerr << "Please define all parameters as follow:\n"
                << "CSV_to_Asc_plus_filters \n"
                << "              -inDir <input Directory>\n"
                << "              -oAsc <dir for storing unprocessed asc files>\n"
                << "              -oSnP <dir for storing asc after salt n pepper noise filter>\n"
                << "              -oMed <dir for storing asc after median filtering>\n"
                << "              -pcol <column containing pixel values / KNN results>\n"
                << "              -xCol <column containing the x positions of the pixels>\n"
                << "              -yCol <column containing the y positions of the pixels>\n";
      std::exit(EXIT_FAILURE);
   }
   std::cout << "inDir=" << inDir << "\n";
   std::cout << "oAsc =" << oAsc << "\n";
   std::cout << "oSnP =" << oSnP << "\n";
   std::cout << "oMed =" << oMed << "\n";
   std::cout << "pCol, xCol, yCol : " <<pcol << ", " << xCol<< ", " <<yCol << "\n";


   std::cout << "   ***   EXIT   ***\n";
   return EXIT_SUCCESS;
}
