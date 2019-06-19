#include "ASC.h"
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <fstream>

//-----------------------------------------------------------------------------
ASC::ASC(
        unsigned short i_ncolsX,
        unsigned short i_nrowsY,
        double i_xllxcorner,
        double i_yllcorner,
        double i_cellsize,
        double i_noDataValue):
    m_ncols(i_ncolsX),
    m_nrows(i_nrowsY),
    m_xllcorner(i_xllxcorner),
    m_yllcorner(i_yllcorner),
    m_cellsize(i_cellsize),
    m_NODATA_value(i_noDataValue),
    m_values(m_ncols*m_nrows,i_noDataValue)
{
    std::cout << "ncols "        << m_ncols << "\n"
              << "nrows "        << m_nrows << "\n"
              << "xllcorner "    << m_xllcorner << "\n"
              << "yllcorner "    << m_yllcorner << "\n"
              << "cellsize "     << m_cellsize << "\n"
              << "NODATA_value " << m_NODATA_value << "\n";
}


//-----------------------------------------------------------------------------
unsigned int ASC::getIndex(const double i_x, const double i_y) const
{
   unsigned short int col = ceil((i_x-m_xllcorner)/m_cellsize);
   unsigned short int row = ceil((i_y-m_yllcorner)/m_cellsize);

   assert(col<m_ncols);
   assert(row<m_nrows);
   return getIndexInt(col,row);
}

//-----------------------------------------------------------------------------
unsigned int ASC::getIndexInt(
        const unsigned short int i_c,
        const unsigned short int i_r
        ) const
{
   assert(i_c+i_r*m_ncols<m_values.size());
   return i_c+i_r*m_ncols;
}


//-----------------------------------------------------------------------------
void ASC::addValue(double i_x, double i_y, double value)
{
    m_values[getIndex(i_x,i_y)]=value;
}

//-----------------------------------------------------------------------------
void ASC::saltNpepper()
{
    std::vector<double> tmp_values(m_ncols*m_nrows);
    assert(tmp_values.size()==m_values.size());
   std::cout << "Applying Salt and Pepper filtering \n";

    std::vector<double> values;
    for(unsigned short int r=1;r<m_nrows-1; ++r)
    {
       for(unsigned short int c=1;c<m_ncols-1; ++c)
       {
           values.clear();
           double v1 = m_values[this->getIndexInt(c-1,r-1)];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              values.push_back(v1);
           }
           else
           {
           }
           v1 = m_values[this->getIndexInt(c  ,r-1)];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              values.push_back(v1);
           }
           v1 = m_values[this->getIndexInt(c+1,r-1)];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              values.push_back(v1);
           }
           v1 = m_values[this->getIndexInt(c-1,r  )];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              values.push_back(v1);
           }
           v1 = m_values[this->getIndexInt(c  ,r  )];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              values.push_back(v1);
           }
           v1 = m_values[this->getIndexInt(c+1,r  )];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              values.push_back(v1);
           }
           v1 = m_values[this->getIndexInt(c-1,r+1)];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              values.push_back(v1);
           }
           v1 = m_values[this->getIndexInt(c  ,r+1)];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              values.push_back(v1);
           }
           v1 = m_values[this->getIndexInt(c+1,r+1)];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              values.push_back(v1);
           }
           std::sort (values.begin(), values.end());
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              values.push_back(v1);
           }
           if(values.size()>0)
           {
              unsigned short int med = floor(double(values.size()/2.0));
              tmp_values[this->getIndexInt(c,r)]=values[med];
           }

       }
    }

    for(unsigned int i=0; i<tmp_values.size();++i)
    {
       m_values[i]=tmp_values[i];
    }
}

//-----------------------------------------------------------------------------
void ASC::smooth()
{
    std::vector<double> tmp_values(m_ncols*m_nrows);
    assert(tmp_values.size()==m_values.size());
        std::cout << "Applying smoothing filtering \n";


    for(unsigned short int r=1;r<m_nrows-1; ++r)
    {
       for(unsigned short int c=1;c<m_ncols-1; ++c)
       {
           double sum(0.0),no(0.0);
           double v1 = m_values[this->getIndexInt(c-1,r-1)];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              sum+=(v1); no+=1.0;
           }
           v1 = m_values[this->getIndexInt(c  ,r-1)];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              sum+=(v1); no+=1.0;
           }
           v1 = m_values[this->getIndexInt(c+1,r-1)];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              sum+=(v1); no+=1.0;
           }
           v1 = m_values[this->getIndexInt(c-1,r  )];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              sum+=(v1); no+=1.0;
           }
           v1 = m_values[this->getIndexInt(c  ,r  )];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              sum+=(v1); no+=1.0;
           }
           v1 = m_values[this->getIndexInt(c+1,r  )];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              sum+=(v1); no+=1.0;
           }
           v1 = m_values[this->getIndexInt(c-1,r+1)];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              sum+=(v1); no+=1.0;
           }
           v1 = m_values[this->getIndexInt(c  ,r+1)];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              sum+=(v1); no+=1.0;
           }
           v1 = m_values[this->getIndexInt(c+1,r+1)];
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              sum+=(v1); no+=1.0;
           }
           if(v1>m_NODATA_value+0.00001 || v1<m_NODATA_value-0.00001)
           {
              sum+=(v1); no+=1.0;
           }
           if(no>0.00001)
           {
              tmp_values[this->getIndexInt(c,r)]=sum/no;
           }
       }
    }

    for(unsigned int i=0; i<tmp_values.size();++i)
    {
       m_values[i]=tmp_values[i];
    }
}

//-----------------------------------------------------------------------------
void ASC::exportTo(const std::string &i_fileName) const
{
    std::cout << "Exporing ASC file : " << i_fileName << "\n";
    std::ofstream myfile;
    myfile.open (i_fileName);
    if(!myfile.is_open())
    {
       std::cout <<"ERROR: File \"" << i_fileName << "\" was not created"
                 << "\n";
       std::exit(EXIT_FAILURE);
    }
    myfile << "ncols "        << m_ncols << "\n"
           << "nrows "        << m_nrows << "\n"
           << "xllcorner "    << m_xllcorner << "\n"
           << "yllcorner "    << m_yllcorner << "\n"
           << "cellsize "     << m_cellsize << "\n"
           << "NODATA_value " << m_NODATA_value << "\n";

    for(short int r=m_nrows-1;r>=0; --r)
    {
       for(unsigned short int c=0;c<m_ncols; ++c)
       {
           assert(m_values.size()==m_ncols*m_nrows);
          assert(getIndexInt(c,r)<m_values.size());
          myfile << m_values[getIndexInt(c,r)] << " ";
       }
       myfile << "\n";
    }
    myfile.close();
    std::cout << "ASC file : " << i_fileName << " exported\n";
}


//-----------------------------------------------------------------------------
ASC::~ASC()
{

}

