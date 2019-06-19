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



// CSV_seperate -i <csvInput> -c <column> -odir <outputDir>
// the file will be seperated into multiple files according to the content of the <column>
// name of output files are the "<content of the column> + .csv"
// IMPORTANT: <outputDir> must finish with "/"

// test examples:
// CSV_seperate -i "/home/milto/Documents/RedGum/Volumes/0.8/CrossValidation/Dataset_1_2_3/cylinder_processed/k-nn.csv" -c VolFileName

// test example 2019
/* CSV_seperate -i "/home/milto/Documents/RedGum/Volumes/0.8/crossValidation2019/Val1/cylinderH12_R4_k-nn.csv" -c Vols_File_Name
   -odir "/home/milto/Documents/RedGum/Volumes/0.8/crossValidation2019/Val1/all_H12_R4_csv"
*/


// test example 2019 - windows version
/*
  CSV_seperate -i  "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/KNN_noThresheight.csv" -c Vols_File_Name -odir "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/KNN_csv_seperated/"

 */

int main(int argc, char *argv[])
{
   std::map<std::string, int> tags;
   tags["-i"     ] = 1; /// -i <csvInput>
   tags["-c"     ] = 2; /// -c <column>
   tags["-odir"  ] = 3; /// -odir <outputDir>

   std::string input("");
   std::string column("");
   std::string outDir("");

   int column_idx(-1);
   std::multimap < std::string , std::string > myMap;
   std::string labels_single_str;

   // parse files
   int argvIndex = 1;
   while(argvIndex<argc)
   {
      switch (tags[argv[argvIndex]])
      {
      case 1: // -i <csvInput>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             input = argv[argvIndex];
          }
          break;
      }
      case 2: // -c <column>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             column = argv[argvIndex];
          }
          break;
      }
      case 3: // -odir <outputDir>
      {
          argvIndex++;
          if (argvIndex<argc)
          {
             outDir = argv[argvIndex];
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
   if(input=="" || column=="")
   {
      std::cerr << "Please define all parameters as follow:\n"
                "CCSV_seperate -i <csvInput> -c <column> -outDir <outDir>\n";
      std::exit(EXIT_FAILURE);
   }
   std::cout << "OutDir=" << outDir << "\n";
//   if(outDir!="")
//   {
//      std::cout << "WARNING: If it fails, it maybe because you format the \"\\\" at the end of the outDir>\n";
//      const int dir_err = mkdir(outDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
//      if (-1 == dir_err)
//      {
//          std::cout << ("Error creating directory. It may already exist\n");
//      }
//   }

   // read file and separe it into the unordered multi-map
   std::ifstream input_file; input_file.open(input.c_str());
   if(!input_file.is_open())
   {
      std::cerr<<"ERROR: failed to open file file: " << input << "\n";
      std::exit(EXIT_FAILURE);
   }

   std::vector<std::string> labels_seperated;
   if(std::getline(input_file,labels_single_str))
   {
       if(labels_seperated.size()==0)
       {
          std::istringstream ss( labels_single_str );
          while (ss)
          {
             std::string subS;
             if (!std::getline( ss, subS, ',' )) break;
             subS.erase(std::remove(subS.begin(), subS.end(), '"'), subS.end());
             labels_seperated.push_back(subS);
          }
       }
    }
    for(unsigned int i=0; i<labels_seperated.size(); ++i)
    {
       if(labels_seperated[i]==column)
       {
          column_idx = i;
       }
    }
    if(column_idx<0)
    {
       std::cerr << "ERROR: Column name was not found!";
       std::exit(EXIT_FAILURE);
    }
    std::string line;
    std::vector<std::string> temp(labels_seperated.size());
    while(std::getline(input_file,line))
    {
       std::istringstream ss( line );
       unsigned int i=0;
       while (ss)
       {
          std::string subS;
          if (!std::getline( ss, subS, ',' )) break;
          temp[i]=subS;
          i++;
       }
       myMap.emplace(temp[column_idx],line);
    }

    for(  std::multimap<std::string,std::string>::iterator it1 = myMap.begin(), end = myMap.end(); it1 != end; it1 = myMap.upper_bound(it1->first))
    {
       std::string tmpStr(it1->first);
       size_t found=tmpStr.find_last_of("/\\");
       std::string filenameWithNoPath = tmpStr.substr(found+1);


       std::string filename(outDir+filenameWithNoPath+".csv");
       filename.erase(std::remove(filename.begin(), filename.end(), '"'), filename.end());
       std::cout << filename << "\n";
       std::ofstream file; file.open(filename);
       file << labels_single_str << "\n";
       auto itsElements = myMap.equal_range(it1->first );
       for (auto it = itsElements.first; it != itsElements.second; ++it)
       {
          file << it->second << "\n";
       }
       file.close();
    }

    input_file.close();
   // exports csv files

   std::cout << "   ***   EXIT   ***\n";
   return EXIT_SUCCESS;
}
