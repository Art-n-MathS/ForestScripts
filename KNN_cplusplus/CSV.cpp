#include "CSV.h"

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include<sstream>

//-----------------------------------------------------------------------------
CSV::CSV(const std::string &i_name):
    m_csvFile(i_name)
{
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
    m_cols = m_labels.size();
    m_rows = m_values.size()/m_labels.size();
    unsigned int modVL = m_values.size()%m_labels.size();
    std::cout << "No of features: " << m_cols << "  -  No of samples: "
              << m_rows << "  -  modVL(=0): " << modVL << "\n\n";
    assert(m_values.size()%m_labels.size()==0);
}






//-----------------------------------------------------------------------------
CSV::~CSV()
{

}
