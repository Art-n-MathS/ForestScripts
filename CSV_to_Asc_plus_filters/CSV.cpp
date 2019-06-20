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
#include <algorithm>

//-----------------------------------------------------------------------------
CSV::CSV(
        const std::string &i_csvFileName,
        const unsigned short int i_xCol,
        const unsigned short int i_yCol,
        const unsigned short int i_pCol
        )
{
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
ASC *CSV::getASC(double i_vl,double i_noDataValue)const
{
    double xllcorner(*std::min_element(m_xValues.begin(), m_xValues.end())),
           yllcorner(*std::min_element(m_yValues.begin(), m_yValues.end())),
           xmax(*std::max_element(m_xValues.begin(), m_xValues.end())),
           ymax(*std::max_element(m_yValues.begin(), m_yValues.end()));


    unsigned int ncolsX(ceil((xmax-xllcorner)/i_vl)),
                 ncolsY(ceil((xmax-xllcorner)/i_vl));
    xllcorner-=i_vl;
    yllcorner-=i_vl;
    ncolsX=(ceil((xmax-xllcorner)/i_vl))+2,
    ncolsY=(ceil((ymax-yllcorner)/i_vl))+2;

//    std::cout << "ncols "        << ncolsX << "\n"
//              << "nrows "        << ncolsY << "\n"
//              << "xllcorner "    << xllcorner << "\n"
//              << "yllcorner "    << yllcorner << "\n"
//              << "cellsize "     << i_vl << "\n"
//              << "NODATA_value " << i_noDataValue << "\n"
//              << "'''''''''''''''''''''''''''\n\n\n";

    ASC *currentASC = new ASC(ncolsX,ncolsY,xllcorner,yllcorner,i_vl,
                              i_noDataValue);

    for (unsigned int i=0; i<m_xValues.size();++i)
    {
       currentASC->addValue(m_xValues[i],m_yValues[i],m_pValues[i]);
    }
    std::cout << "ASC created\n";
    return currentASC;
}

//-----------------------------------------------------------------------------
CSV::~CSV()
{

}
