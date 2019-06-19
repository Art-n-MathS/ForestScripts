#include "CSV.h"
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
#include <iomanip>


//-----------------------------------------------------------------------------
CSV::CSV(
        const std::string &i_csvFileName,
        const unsigned short int i_xCol,
        const unsigned short int i_yCol,
        const unsigned short int i_pCol
        )
{
    std::cout << "   ****   pCol, xCol, yCol : " <<i_pCol << ", " << i_xCol<< ", " <<i_yCol << "\n";

    // read file and separe it into the unordered multi-map
    std::ifstream input_file; input_file.open(i_csvFileName.c_str());
    if(!input_file.is_open())
    {
       std::cerr<<"ERROR: failed to open file file: " << i_csvFileName << "\n";
       std::exit(EXIT_FAILURE);
    }
    std::vector<std::string> lineValues;
    std::string line;
    std::getline(input_file,line); // discard labels

    unsigned int countLine(0);
    while(std::getline(input_file,line))
    {
        countLine++;
        lineValues.resize(0);

        std::istringstream ss( line );
        while (ss)
        {
           std::string subS;
           if (!std::getline( ss, subS, ',' )) break;
           subS.erase(std::remove(subS.begin(), subS.end(), '"'), subS.end());
           lineValues.push_back(subS);
        }

        assert(lineValues.size()>i_xCol && lineValues.size()>i_yCol &&
               lineValues.size()>i_pCol);
        std::string orbits ("365.24 29.53");
          std::string::size_type sz;     // alias of size_t
        m_xValues.push_back(std::stod(lineValues[i_xCol]));
        m_yValues.push_back(std::stod(lineValues[i_yCol]));
        m_pValues.push_back(std::stod(lineValues[i_pCol]));
     }
    std::cout << std::fixed << std::setprecision(3) ;
    assert(m_xValues.size()==m_yValues.size() &&
           m_yValues.size()==m_pValues.size());
}

//-----------------------------------------------------------------------------
ASC *CSV::getASC()const
{
    ASC *currentASC = new ASC();

    return currentASC;
}

//-----------------------------------------------------------------------------
CSV::~CSV()
{

}
