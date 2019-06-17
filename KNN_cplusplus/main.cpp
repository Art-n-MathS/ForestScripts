#include <iostream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include<sstream>
#include <iomanip>
#include <exception>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <thread>
#include <map>

#include "CSVManager.h"

//-----------------------------------------------------------------------------
/// @file main.cpp
/// @author Dr Milto Miltiadou
/// @version 1.0
/// @date 16/03/19
/// @brief this program takes as input testing and negative feature vectors
/// @brief and uses them as reference to perform weighten KNN
/// @brief Before KNN is applied it removes line with height bigger than 70
/// @brief since it is classified as noise
/// @brief It also removes ground using a given col number of the .csv
///
/// @brief KNN_cplusplus -ipos <positive_feature_vectors.csv>
///                      -ineg <negative_feature_vectors.csv>
///                      -iall <all_pixels_feature_vectors.csv>
///                      -heightCol <col>
///                      -keeps <col_1> <col_2> ... <col_n>
///                      -cols <col_1> <col_2> ... <col_m>
///                      -weights <weight_1> <weight_2> ... <weight_m>
///                      -oKKN  <KNN_results.csv>
///                      -k <no of k in KNN>
///
///
/// @note Keeps are the columns are are copied paste to the final .csv file
/// Cols are the columns used as features in the KNN and each one of them as
/// an associated weight. Therefore Cols and weights must have the same length
///
/* example: -ipos "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/all_dead_cylinder_vl0.8_H12_R4_scaled.csv"
            -ineg "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/all_live_cylinder_vl0.8_H12_R4_scaled.csv"
            -iall "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/all_all_cylinder_vl0.8_H12_R4_scaled_forTesting.csv"
            -heightCol 3 -keeps 0 1 2 3 -cols 4 5 6 7 8 9 10 11 12 13 -weights 15 14 13 12 11 10 9 8 7 6
            -oKKN "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/KNN.csv" -k 7

  example in a single line:
-ipos "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/all_dead_cylinder_vl0.8_H12_R4_scaled.csv" -ineg "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/all_live_cylinder_vl0.8_H12_R4_scaled.csv" -iall "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/all_all_cylinder_vl0.8_H12_R4_scaled_forTesting.csv"  -heightCol 3 -keeps 0 1 2 3 -cols 4 5 6 7 8 9 10 11 12 13 -weights 15 14 13 12 11 10 9 8 7 6 -oKKN "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/KNN.csv" -k 7

*/
//-----------------------------------------------------------------------------


int main(int argc, char *argv[])
{

    std::string ipos(""), ineg(""), iall(""), oKNN("");
    unsigned short int heightCol(1000),k(1000);
    std::vector<unsigned short int> cols, weights, keeps;
    // PARSING
    // pair arguments to numbers to ease search
    std::map<std::string, int> tags;


    tags["-ipos"     ] = 1; // -ipos <positive_feature_vectors.csv>
    tags["-ineg"     ] = 2; // -ineg <negative_feature_vectors.csv>
    tags["-heightCol"] = 3; // -heightCol <col>
    tags["-keeps"    ] = 4; // -keeps <col_1> <col_2> ... <col_n>
    tags["-cols"     ] = 5; // -cols <col_1> <col_2> ... <col_m>
    tags["-weights"  ] = 6; // -weights <weight_1> <weight_2> ... <weight_m>
    tags["-oKKN"     ] = 7; // -oKKN  <KNN_results.csv>
    tags["-k"        ] = 8; // -k <no of k in KNN>
    tags["-iall"     ] = 9; // -iall <all_pixels_feature_vectors.csv>

    try
    {
      int argvIndex = 1;
      while(argvIndex<argc)
      {
         switch (tags[argv[argvIndex]])
         {
         case 1: //  -ipos <positive_feature_vectors.csv>
         {
            argvIndex ++;
            if (argvIndex<argc)
            {
               ipos = argv[argvIndex];
            }
            break;
         }
         case 2: //  -ineg <negative_feature_vectors.csv>
         {
            argvIndex ++;
            if (argvIndex<argc)
            {
               ineg = argv[argvIndex];
            }
            break;
         }
         case 3: // -heightCol <col>
         {
            argvIndex ++;
            if (argvIndex<argc)
            {
               heightCol = atoi(argv[argvIndex]);
            }
            break;
         }
         case 4: //  -keeps <col_1> <col_2> ... <col_n>
         {
            while (argvIndex+1<argc && argv[argvIndex+1][0]!='-')
            {
               argvIndex++;
               keeps.push_back(atoi(argv[argvIndex]));
            }
            break;
         }
         case 5: // -cols <col_1> <col_2> ... <col_m>
         {
            while (argvIndex+1<argc && argv[argvIndex+1][0]!='-')
            {
               argvIndex++;
               cols.push_back(atoi(argv[argvIndex]));
            }
            break;
         }
         case 6: // -weights <weight_1> <weight_2> ... <weight_m>
         {
            while (argvIndex+1<argc && argv[argvIndex+1][0]!='-')
            {
               argvIndex++;
               weights.push_back(atoi(argv[argvIndex]));
            }
            break;
         }
         case 7: // -oKKN  <KNN_results.csv>
         {
             argvIndex++;
             if (argvIndex<argc)
             {
                oKNN = argv[argvIndex];
             }
             break;
         }
         case 8: // -k <no of k in KNN>
         {
            argvIndex ++;
            if (argvIndex<argc)
            {
               k = atoi(argv[argvIndex]);
            }
            break;
         }
         case 9: // -iall <all_pixels_feature_vectors.csv>
         {
             argvIndex++;
             if (argvIndex<argc)
             {
                iall = argv[argvIndex];
             }
             break;
         }
         default:
         {
            std::cout << "WARNING: Unkown tag: " << argv[argvIndex] << "\n";
            break;
         }
         }
         argvIndex++;
      }
     }
     catch (char const* e)
     {
        std::cout << e  << std::endl;
        std::cout << "Please use the following format:\n"
                  << " KNN_cplusplus -ipos <positive_feature_vectors.csv>\n"
                  << "               -ineg <negative_feature_vectors.csv>\n"
                  << "               -iall <all_pixels_feature_vectors.csv>\n"
                  << "               -heightCol <col>\n"
                  << "               -keeps <col_1> <col_2> ... <col_n>\n"
                  << "               -cols <col_1> <col_2> ... <col_m>\n"
                  << "               -weights <weight_1> <weight_2> ... <weight_m>\n"
                  << "               -oKKN  <KNN_results.csv>\n"
                  << "               -k <no of k in KNN>\n"
                  << "\n WARNING: No of cols must be equal to no of weights\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(20000));
        return EXIT_FAILURE;
     }



    if (ipos=="" || ineg=="" || iall=="" || heightCol==1000 ||cols.size()==0 ||
            weights.size()!=cols.size() || oKNN=="" || k==1000)
    {
        std::cout << "ERROR: Missing arguments. Please use the following format:\n"
                  << " KNN_cplusplus -ipos <positive_feature_vectors.csv>\n"
                  << "               -ineg <negative_feature_vectors.csv>\n"
                  << "               -iall <all_pixels_feature_vectors.csv>\n"
                  << "               -heightCol <col>\n"
                  << "               -keeps <col_1> <col_2> ... <col_n>\n"
                  << "               -cols <col_1> <col_2> ... <col_m>\n"
                  << "               -weights <weight_1> <weight_2> ... <weight_m>\n"
                  << "               -oKKN  <KNN_results.csv>\n"
                  << "               -k <no of k in KNN>\n"
                  << "\n WARNING: No of cols must be equal to no of weights\n";
        std::exit(EXIT_FAILURE);
    }


    // print read parameters
    std::cout << "ipos      = " << ipos      << "\n";
    std::cout << "ineg      = " << ineg      << "\n";
    std::cout << "iall      = " << iall      << "\n";
    std::cout << "oKNN      = " << oKNN      << "\n";
    std::cout << "heightCol = " << heightCol << "\n";
    std::cout << "k         = " << k         << "\n";
    std::cout << "keeps     = ";
    for(unsigned short int i=0;i<keeps.size();++i)
    {
       std::cout << keeps[i] << " ";
    }
    std::cout <<"\n";
    std::cout << "cols      = ";
    for(unsigned short int i=0;i<cols.size();++i)
    {
       std::cout << cols[i] << " ";
    }
    std::cout <<"\n";
    std::cout << "weights   = ";
    for(unsigned short int i=0;i<weights.size();++i)
    {
       std::cout << weights[i] << " ";
    }
    std::cout <<"\n\n";


    CSVManager csvManager(ipos,ineg,iall,oKNN,heightCol,cols,weights,keeps,k);
    csvManager.interpret();


    std::cout << "   ***   EXIT   ***\n";
    return EXIT_SUCCESS;
}
