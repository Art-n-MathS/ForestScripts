#include "CSVManager.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include<sstream>


//-----------------------------------------------------------------------------
CSVManager::CSVManager(
        const std::string &i_ipos,
        const std::string &i_ineg,
        const std::string &i_iall,
        const std::string &i_oKNN,
        unsigned short int i_heightCol,
        const std::vector<unsigned short int> &i_cols,
        const std::vector<unsigned short int> &i_weights,
        const std::vector<unsigned short int> &i_keeps,
        const unsigned short int i_k
        ):
    m_posSamples(i_ipos,i_cols,i_weights,i_heightCol),
    m_negSamples(i_ineg,i_cols,i_weights,i_heightCol),
    m_iall(i_iall),
    m_oKNN(i_oKNN),
    m_keeps(i_keeps),
    m_k(i_k)
{
   std::cout << "Initialisation of CSVManager completed:";
   std::cout << "\nm_iall      = " << m_iall ;
   std::cout << "\nm_k         = " << m_k;

   std::cout << "\nm_keeps     = " ;
   for(unsigned int i=0; i<m_keeps.size(); ++i)
   {
      std::cout << m_keeps[i] << " ";
   }
   std::cout << "\n\n";
}

//-----------------------------------------------------------------------------
void CSVManager::interpret()
{
    std::ifstream inFile;
    std::string line;
    inFile.open(m_iall.c_str());
    if(!inFile)
    {
       std::cerr << "ERROR: File \"" << m_iall << "\" doesn't exist\n";
       std::exit(EXIT_FAILURE);
    }

    while(std::getline(inFile,line))
    {
      double *posKNN = m_posSamples.getNearestValues(line,m_k);
      double *negKNN = m_negSamples.getNearestValues(line,m_k);

//      std::istringstream ss( line );
//      while (ss)
//      {
//        std::string subS;
//        if (!std::getline( ss, subS, ',' )) break;
////        knnResults.push_back(subS);
//      }
      delete posKNN;
      delete negKNN;
    }

     inFile.close();
}


//-----------------------------------------------------------------------------
CSVManager::~CSVManager()
{
}
