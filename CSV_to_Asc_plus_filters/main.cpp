#include <iostream>
#include <vector>
#include <map>
#include<string>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include <assert.h>
#include <algorithm>
#include <unordered_map>
#include <sys/stat.h>
#include <dirent.h>
#include "CSV.h"

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
///              -pCol <column containing pixel values / KNN results>
///              -xCol <column containing the x positions of the pixels>
///              -yCol <column containing the y positions of the pixels>
///              -hCol <column containing height values>
///              -oHSnP <dir for storing heights ascs with SnP noise reduction>

///
/* example: -inDir "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/KNN_csv_seperated"
            -oAsc "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs"
            -oSnP "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_snp"
            -oHSnP "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_height_snp"
            -oSmth "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_snp_med"
            -pCol 5 -xCol 1 -yCol 2 -hCol 4


  example in a single line:
-inDir "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/KNN_csv_seperated" -oHSnP "C:\Users\milto\Documents\TEPAK\Marie_Curie_IF\processing\D3.1\Val1\H12_R4\Ascs_height_snp" -oAsc "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs" -oSnP "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_snp" -oSmth "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_snp_smth" -pCol 5 -xCol 1 -yCol 2 -hCol 4
*/
//-----------------------------------------------------------------------------


int main(int argc, char *argv[])
{
   std::map<std::string, int> tags;
   tags["-inDir"] = 1; /// -inDir <input Directory>
   tags["-oAsc" ] = 2; /// -oAsc <dir for storing unprocessed asc files>
   tags["-oSnP" ] = 3; /// -oSnP <dir for storing asc after salt n pepper noise filter>
   tags["-oSmth"] = 4; /// -oSmth <dir for storing asc after averaging filtering>
   tags["-pCol" ] = 5; /// -pCol <column containing pixel values / KNN results>
   tags["-xCol" ] = 6; /// -xCol <column containing the x positions of the pixels>
   tags["-yCol" ] = 7; /// -yCol <column containing the y positions of the pixels>
   tags["-hCol" ] = 8; /// -hCol <column containing height values>
   tags["-oHSnP"] = 9; /// -oHSnP <dir for storing heights ascs with SnP noise reduction>


   std::string inDir(""), oAsc(""), oSnP(""), oSmth(""), oHSnP("");
   unsigned short int pCol(1000),xCol(1000),yCol(1000),hCol(1000);

   double noDataValue(-100.0);
   double vl=0.8;

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
      case 4: // -oSmth <dir for storing asc after averaging filtering>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             oSmth = argv[argvIndex];
          }
          break;
      }
      case 5: // -pCol <column containing pixel values / KNN results>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             pCol = atoi(argv[argvIndex]);
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
      case 8: // -hCol <column containing height values>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             hCol = atoi(argv[argvIndex]);
          }
          break;
      }
      case 9: // -oHSnP <dir for storing heights ascs with SnP noise reduction>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             oHSnP = argv[argvIndex];
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
   if(inDir=="" || oAsc=="" || oSnP=="" || oSmth=="" || pCol==1000 ||
           xCol==1000 || yCol==1000 || hCol==1000 || oHSnP=="")
   {
      std::cerr << "Please define all parameters as follow:\n"
                << "CSV_to_Asc_plus_filters \n"
                << "              -inDir <input Directory>\n"
                << "              -oAsc <dir for storing unprocessed asc files>\n"
                << "              -oSnP <dir for storing asc after salt n pepper noise filter>\n"
                << "              -oSmth <dir for storing asc after averaging filtering>\n"
                << "              -pCol <column containing pixel values / KNN results>\n"
                << "              -xCol <column containing the x positions of the pixels>\n"
                << "              -yCol <column containing the y positions of the pixels>\n"
                << "              -hCol <column containing height values>\n"
                << "               -oHSnP <dir for storing heights ascs with SnP noise reduction>\n";
      std::exit(EXIT_FAILURE);
   }
   if (inDir.c_str()[inDir.size()-1]!='/') inDir+="/";
   if (oAsc.c_str()[oAsc.size()-1]!='/') oAsc+="/";
   if (oSnP.c_str()[oSnP.size()-1]!='/') oSnP+="/";
   if (oSmth.c_str()[oSmth.size()-1]!='/') oSmth+="/";
   if (oHSnP.c_str()[oHSnP.size()-1]!='/') oHSnP+="/";


   std::cout << "inDir=" << inDir << "\n";
   std::cout << "oAsc =" << oAsc << "\n";
   std::cout << "oSnP =" << oSnP << "\n";
   std::cout << "oMed =" << oSmth << "\n";
   std::cout << "oHSnP =" << oHSnP << "\n";
   std::cout << "pCol, xCol, yCol, hCol : " <<pCol << ", " << xCol<< ", " <<yCol
             << ", " << hCol << "\n";


   DIR *dir;
   struct dirent *ent;
   if ((dir = opendir (inDir.c_str())) != NULL) {
     /* print all the files and directories within directory */
     while ((ent = readdir (dir)) != NULL)
     {
       std::string currentCSV(ent->d_name);
       if(currentCSV.size()>4 && currentCSV.c_str()[currentCSV.size()-1]=='v')
       {
          std::string currentCSVwithDir = inDir + currentCSV ;
          std::cout << "currentCSVwithDir: " << currentCSVwithDir << "\n";

          CSV csvCurrentHeight(currentCSVwithDir,xCol,yCol,hCol);
          ASC *ascCurrentH = csvCurrentHeight.getASC(vl,noDataValue);
          ascCurrentH->saltNpepper();
          ascCurrentH->exportTo(oHSnP+currentCSV+".asc");
          delete ascCurrentH;

          CSV csvCurrent(currentCSVwithDir,xCol,yCol,pCol);


          ASC *ascCurrent = csvCurrent.getASC(vl,noDataValue);
          ascCurrent->exportTo(oAsc+currentCSV+".asc");
          std::cout << "---------------------\n";

          ascCurrent->saltNpepper();
          ascCurrent->exportTo(oSnP+currentCSV+".asc");
          ascCurrent->smooth();
          ascCurrent->exportTo(oSmth+currentCSV+".asc");
          delete ascCurrent;
       }
     }
     closedir (dir);
   }
   else
   {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
   }

   std::cout << "   ***   EXIT   ***\n";
   return EXIT_SUCCESS;
}
