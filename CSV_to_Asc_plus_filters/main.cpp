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
///              -pcol <column containing pixel values / KNN results>
///              -xCol <column containing the x positions of the pixels>
///              -yCol <column containing the y positions of the pixels>

///
/* example: -inDir "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/KNN_csv_seperated"
            -oAsc "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs"
            -oSnP "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_snp"
            -oSmth "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_snp_med"
            -pcol 3 -xCol 3 -yCol 4


  example in a single line:
-inDir "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/KNN_csv_seperated" -oAsc "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs" -oSnP "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_snp" -oSmth "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/Ascs_snp_smth" -pcol 3 -xCol 3 -yCol 4
*/
//-----------------------------------------------------------------------------


int main(int argc, char *argv[])
{
   std::map<std::string, int> tags;
   tags["-inDir"] = 1; /// -inDir <input Directory>
   tags["-oAsc" ] = 2; /// -oAsc <dir for storing unprocessed asc files>
   tags["-oSnP" ] = 3; /// -oSnP <dir for storing asc after salt n pepper noise filter>
   tags["-oSmth"] = 4; /// -oSmth <dir for storing asc after averaging filtering>
   tags["-pcol" ] = 5; /// -pcol <column containing pixel values / KNN results>
   tags["-xCol" ] = 6; /// -xCol <column containing the x positions of the pixels>
   tags["-yCol" ] = 7; /// -yCol <column containing the y positions of the pixels>

   std::string inDir(""), oAsc(""), oSnP(""), oSmth("");
   unsigned short int pCol(1000),xCol(1000),yCol(1000);


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
      case 5: // -pcol <column containing pixel values / KNN results>
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
      default:
      {
         std::cerr << "WARNING: Unkown tag: " << argv[argvIndex] << "\n";
         break;
      }
      }
      argvIndex++;
   }
   if(inDir=="" || oAsc=="" || oSnP=="" || oSmth=="" || pCol==1000 ||
           xCol==1000 || yCol==1000)
   {
      std::cerr << "Please define all parameters as follow:\n"
                << "CSV_to_Asc_plus_filters \n"
                << "              -inDir <input Directory>\n"
                << "              -oAsc <dir for storing unprocessed asc files>\n"
                << "              -oSnP <dir for storing asc after salt n pepper noise filter>\n"
                << "              -oSmth <dir for storing asc after averaging filtering>\n"
                << "              -pcol <column containing pixel values / KNN results>\n"
                << "              -xCol <column containing the x positions of the pixels>\n"
                << "              -yCol <column containing the y positions of the pixels>\n";
      std::exit(EXIT_FAILURE);
   }
   if (inDir.c_str()[inDir.size()-1]!='/') inDir+="/";
   if (oAsc.c_str()[oAsc.size()-1]!='/') oAsc+="/";
   if (oSnP.c_str()[oSnP.size()-1]!='/') oSnP+="/";
   if (oSmth.c_str()[oSmth.size()-1]!='/') oSmth+="/";


   std::cout << "inDir=" << inDir << "\n";
   std::cout << "oAsc =" << oAsc << "\n";
   std::cout << "oSnP =" << oSnP << "\n";
   std::cout << "oMed =" << oSmth << "\n";
   std::cout << "pCol, xCol, yCol : " <<pCol << ", " << xCol<< ", " <<yCol << "\n";


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
          CSV csvCurrent(currentCSVwithDir,xCol,yCol,pCol);
          ASC *ascCurrent = csvCurrent.getASC();
          ascCurrent->exportTo(oAsc+currentCSV+".asc");
          ascCurrent->saltNpepper();
          ascCurrent->exportTo(oSnP+currentCSV+".asc");
          ascCurrent->smooth();
          ascCurrent->exportTo(oSmth+currentCSV+".asc");
          delete ascCurrent;



//             if(!nextCSV.is_open())
//             {
//                std::cerr<<"ERROR: File was not loaded\n";
//                continue;
//             }
//             std::string line;
//             if(std::getline(nextCSV,line))
//             {
//                if(labels.size()==0)
//                {
//                   allCSVfile << line << "\n";
//                   std::istringstream ss( line );
//                   while (ss)
//                   {
//                      std::string subS;
//                      if (!std::getline( ss, subS, ',' )) break;
//                      labels.push_back(subS);
//                   }
//                   for(auto &l : labels) for (auto & c: l) c = std::toupper(c);
//                }
//                else
//                {
//                   std::vector<std::string> clabels;
//                   std::istringstream ss( line );
//                   while (ss)
//                   {
//                      std::string subS;
//                      if (!std::getline( ss, subS, ',' )) break;
//                      clabels.push_back(subS);
//                   }
//                   for(auto &l : clabels) for (auto & c: l) c = std::toupper(c);
//                   if(labels.size()!=clabels.size())
//                   {
//                      std::cout << "ERROR: header of file do not match with the current header\n";
//                      continue;
//                   }
//                   unsigned int countSame(0);
//                   for(unsigned int i=0; i<labels.size(); ++i)
//                   {
//                      countSame+=(labels[i]==clabels[i]);
//                   }
//                   if(countSame!=labels.size())
//                   {
//                       std::cout << "ERROR: header of file do not match with the current header\n";
//                       continue;
//                   }
//                }
//                count++;
//                while(std::getline(nextCSV,line))
//                {
//                    allCSVfile << line << "\n";
//                }
//                nextCSV.close();
//             }
//             else
//             {
//                std::cout << "WARNING: File is empty\n";
//             }
//          }
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
