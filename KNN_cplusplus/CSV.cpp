#include "CSV.h"

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include<sstream>
#include <math.h>

//-----------------------------------------------------------------------------
CSV::CSV(
        const std::string &i_name,
        const std::vector<unsigned short int> &i_cols,
        const std::vector<unsigned short int> &i_weights,
        unsigned short int i_heightCol
        ):
    m_csvFile(i_name),
    m_cols(i_cols),
    m_weights(i_weights),
    m_heightCol(i_heightCol),
    m_heightThres(20.0)
{
    std::cout << "Start reading file: " << m_csvFile << "\n";
    std::ifstream inFile;
    inFile.open(m_csvFile.c_str());
    if(!inFile)
    {
       std::cerr << "ERROR: File \"" << m_csvFile << "\" doesn't exist\n";
       std::exit(EXIT_FAILURE);
    }

    std::string line;
    if(std::getline(inFile,line))
    {
      std::istringstream ss( line );
      while (ss)
      {
        std::string subS;
        if (!std::getline( ss, subS, ',' )) break;
        m_labels.push_back(subS);
      }
    }
    while(std::getline(inFile,line))
    {
      std::istringstream ss( line );
      while (ss)
      {
        std::string subS;
        if (!std::getline( ss, subS, ',' )) break;
        m_values.push_back(subS);
      }
    }
    inFile.close();
    std::cout << "Labels: ";
    for(unsigned int i=0; i<m_labels.size(); ++i)
    {
       std::cout << m_labels[i] << " ";
    }
    std::cout << "\n";
    m_noCols = m_labels.size();
    m_noRows = m_values.size()/m_labels.size();
    unsigned int modVL = m_values.size()%m_labels.size();
    std::cout << "No of features: " << m_noCols << "  -  No of samples: "
              << m_noRows << "  -  modVL(=0): " << modVL << "\nm_cols      = ";
    for(unsigned int i=0; i<m_cols.size(); ++i)
    {
       std::cout << m_cols[i] << " ";
    }
    std::cout << "\nm_weights   = " ;

    for(unsigned int i=0; i<m_weights.size(); ++i)
    {
       std::cout << m_weights[i] << " ";
    }

    std::cout << "\nm_heightCol = " << m_heightCol << "\n\n";
    assert(m_values.size()%m_labels.size()==0);
}

//-----------------------------------------------------------------------------
double *CSV::getNearestValues(
        const std::string &i_line,
        const unsigned short i_k
        ) const
{
   std::vector<double> knnResults;
   double *highestKNN = new double[i_k];
   double sum(0.0),tmp, height;
   std::vector<double> lineSelectedColValues;
   std::vector<std::string> lineValuesStrVector;
   std::istringstream ss( i_line );
   while (ss)
   {
     std::string subS;
     if (!std::getline( ss, subS, ',' )) break;
     lineValuesStrVector.push_back(subS);
   }
   assert(lineValuesStrVector.size()==m_noCols);

   height = atof(lineValuesStrVector[m_heightCol].c_str());
   if(m_heightThres>height) // pixel is ground, KNN is 0 skipped
   {
      for(unsigned int k=0; k<i_k; ++k)
      {
         highestKNN[k]=0;
      }
      return highestKNN;
   }

   for(unsigned int c=0; c<m_cols.size(); ++c)
   {
      lineSelectedColValues.push_back(atof(lineValuesStrVector[m_cols[c]].c_str()));
   }

   assert(lineSelectedColValues.size()==m_cols.size());
   for(unsigned int r=0; r<m_noRows; ++r)
   {
      sum=0;

      for(unsigned int c=0; c<m_cols.size();++c)
      {
         tmp=atof(m_values[m_cols[c]+m_noCols*r].c_str())-lineSelectedColValues[c];
         sum+=(tmp*tmp*double(m_weights[c]));
      }
      knnResults.push_back(sqrt(sum));
   }
   std::exit(EXIT_SUCCESS);
   return highestKNN;
}



//-----------------------------------------------------------------------------
CSV::~CSV()
{

}
