#include <iostream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include<sstream>
#include <iomanip>


//-----------------------------------------------------------------------------
/// @file main.cpp
/// @author Milto Miltiadou
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
///
///
/// @note Keeps are the columns are are copied paste to the final .csv file
/// Cols are the columns used as features in the KNN and each one of them as
/// an associated weight. Therefore Cols and weights must have the same length
///
/* example: -ipos "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/all_dead_cylinder_vl0.8_H12_R4_scaled.csv"
            -ineg "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/all_live_cylinder_vl0.8_H12_R4_scaled.csv"
            -iall "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/all_all_cylinder_vl0.8_H12_R4_scaled.csv"
            -heightCol 3 -keeps 0 1 2 3 -cols 4 5 6 7 8 9 10 11 12 13 -weights 15 14 13 12 11 10 9 8 7 6
            -oKKN "C:/Users/milto/Documents/TEPAK/Marie_Curie_IF/processing/D3.1/Val1/H12_R4/KNN.csv"

*/
//-----------------------------------------------------------------------------


int main(int argc, char *argv[])
{


//    std::string input;
//    std::string output;
//    std::string line;
//    std::vector<std::string> LineVector;
//    int columnX = -1;
//    int columnY = -1;
////    std::vector<Line *> m_lines;
//    std::string labelsStr;
//    double precision(0.001);

//    for( int i=1; i<argc; ++i)
//    {
//       if(std::string(argv[i])=="-i" && i+1<=argc)
//       {
//          ++i;
//          input = argv[i];
//       }
//       else if( std::string(argv[i]) == "-o" && i+1<argc)
//       {
//          ++i;
//          output = argv[i];
//       }
//       else if( std::string(argv[i]) == "-p" && i+1<argc)
//       {
//          ++i;
//          precision = atof(argv[i]);
//       }
//       else
//       {
//          std::cerr << "WARNING: unknown tag : " <<argv[i] << "\n";
//       }
//    }

//    if(input=="" || output=="")
//    {
//       std::cerr << "ERROR: Missing Arguments\n";
//       return EXIT_FAILURE;
//    }



//    std::cout << "Input: " << input << "\n";
//    std::cout << "Output: " << output << "\n";

//    std::ifstream inFile;
//    inFile.open(input.c_str());
//    if(!inFile)
//    {
//       std::cerr << "ERROR: File doesn't exist\n";
//       return EXIT_FAILURE;
//    }
//    std::ofstream outFile;
//    outFile.open(output.c_str());
//    if(!outFile)
//    {
//       std::cerr << "ERROR: couldn't create file\n";
//       return EXIT_FAILURE;
//    }
//    if(std::getline(inFile,labelsStr))
//    {
//       // read labels
//       std::istringstream ss( labelsStr );
//       while (ss)
//       {
//          std::string subS;
//          if (!std::getline( ss, subS, ',' )) break;
//          LineVector.push_back(subS);
//       }
//       for(unsigned int i=0; i<LineVector.size(); ++i)
//       {
//          if(LineVector[i]=="x" || LineVector[i] == "X")
//          {
//             columnX = i;
//          }
//          else if(LineVector[i]=="y" || LineVector[i] == "Y")
//          {
//             columnY = i;
//          }
//       }
//       if(columnX<0 || columnY<0)
//       {
//          std::cout << "ERROR: missing X or Y column\n";
//          return EXIT_FAILURE;
//       }
//       outFile << labelsStr << "\n";
//    }
//    else
//    {
//       std::cerr << "ERROR: empty file\n";
//       return EXIT_FAILURE;
//    }
//    LineVector.clear();

//    while(std::getline(inFile,line))
//    {
//      std::istringstream ss( line );
//      while (ss)
//      {
//        std::string subS;
//        if (!std::getline( ss, subS, ',' )) break;
//        LineVector.push_back(subS);
//      }
//      m_lines.push_back(new Line(atof(LineVector[columnX].c_str()),
//                                 atof(LineVector[columnY].c_str()),
//                                 line));
//      LineVector.clear();
//    }

//    std::cout << "number of trees = " << m_lines.size()-1 << "\n";

//    // sort lines
//     unsigned int len = m_lines.size();
//      // allocate memory for temporarly saved values
//      std::vector<Line *> tempValues;
//      tempValues.resize(len);

//      // sort y axis
//      for(int step=2; step/2 < len; step*=2)
//      {
//         for(unsigned int i=0; i < len; i+=step)
//         {
//            int endOfT2 = i+step;
//            if(i+ step/2 >= len)
//            {
//               continue;
//            }
//            else if (i+step >= len)
//            {
//               endOfT2 = len;
//            }
//            // both sets have step/2 items.
//            // t1 points to the first set of values
//            int t1 = i;
//            // t2 points to the second set of values
//            int t2 = i+step/2;
//            // here we save all the values that have been overridden from the first set
//            unsigned int tempIndex=0;
//            while(t1 < i+step/2 && t2 < endOfT2)
//            {
//               if(m_lines[t1]->m_y>m_lines[t2]->m_y)
//               {
//                  tempValues[tempIndex]=m_lines[t1];
//                  t1++;
//               }
//               else
//               {
//                  tempValues[tempIndex]=m_lines[t2];
//                  t2++;
//               }
//               tempIndex++;
//            }
//            while(t1 < i+step/2)
//            {
//               tempValues[tempIndex]=m_lines[t1];
//               t1++;
//               tempIndex++;
//            }
//            // write values back to the array
//            for(unsigned int t=0; t < tempIndex; ++t)
//            {
//                m_lines[i+t]=tempValues[t];
//            }
//         }
//      }




//    for(unsigned int i=0; i<m_lines.size(); ++i)
//    {
//       delete m_lines[i];
//    }
//    inFile.close();
//    outFile.close();

    std::cout << "   ***   EXIT   ***\n";
    return EXIT_SUCCESS;
}
