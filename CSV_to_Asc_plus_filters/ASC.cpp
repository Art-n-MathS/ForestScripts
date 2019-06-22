#include "ASC.h"
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <fstream>
#include <stack>
#include<iomanip>

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
   short int col = ceil((i_x-m_xllcorner)/m_cellsize);
   short int row = ceil((i_y-m_yllcorner)/m_cellsize);

   if(col<0)col=0;   if(row<0)row=0;
   if(col>=m_ncols)col=m_ncols-1;   if(row>=m_nrows)row=m_nrows-1;
//  std::cout << "  ++ " << col<< " " << m_ncols << "\n";
   assert(col<m_ncols);
//   std::cout << "  ++ " << row<< " " << m_nrows << "\n";
   assert(row<m_nrows);

   return getIndexInt(col,(m_nrows-row-1));
}

//-----------------------------------------------------------------------------
unsigned int ASC::getIndexInt(
        const short int i_c,
        const short int i_r
        ) const
{
   assert(i_c+i_r*m_ncols<int(m_values.size()));
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
void ASC::saltNpepperIncNoValue()
{
    std::vector<double> tmp_values(m_ncols*m_nrows,m_NODATA_value);
    assert(tmp_values.size()==m_values.size());
   std::cout << "Applying Salt and Pepper filtering \n";

    std::vector<double> values(9);
    for(unsigned short int r=1;r<m_nrows-1; ++r)
    {
       for(unsigned short int c=1;c<m_ncols-1; ++c)
       {
           values[0] = m_values[this->getIndexInt(c-1,r-1)];
           values[1] = m_values[this->getIndexInt(c  ,r-1)];
           values[2] = m_values[this->getIndexInt(c+1,r-1)];
           values[3] = m_values[this->getIndexInt(c-1,r  )];
           values[4] = m_values[this->getIndexInt(c  ,r  )];
           values[5] = m_values[this->getIndexInt(c+1,r  )];
           values[6] = m_values[this->getIndexInt(c-1,r+1)];
           values[7] = m_values[this->getIndexInt(c  ,r+1)];
           values[8] = m_values[this->getIndexInt(c+1,r+1)];
           std::sort (values.begin(), values.end());
           tmp_values[this->getIndexInt(c,r)]=values[4];

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
void ASC::threshold(
        const double i_thres
        )
{
    for(unsigned int i=0; i<m_values.size();++i)
    {
       if(m_values[i]<i_thres)
       {
          m_values[i]=m_NODATA_value;
       }
    }
}

//-----------------------------------------------------------------------------
void ASC::threshold(
        const double i_thres,
        const std::vector<double> *i_valuesH
        )
{
    assert(i_valuesH->size()==m_values.size());
    for(unsigned int i=0; i<m_values.size();++i)
    {
       if((*i_valuesH)[i]<i_thres)
       {
          m_values[i]=m_NODATA_value;
       }
    }
}

//-----------------------------------------------------------------------------
ASC::Geolocation *ASC::getGeolocation(
        unsigned short i_c,
        unsigned short i_r
        ) const
{
   ASC::Geolocation *coordinates = new ASC::Geolocation;
   coordinates->m_x = m_xllcorner + i_c*m_cellsize;
   coordinates->m_y = m_yllcorner + i_r*m_cellsize;
   return coordinates;
}

//-----------------------------------------------------------------------------
double ASC::getXLocation(unsigned short i_c) const
{
    return m_xllcorner + i_c*m_cellsize;
}

//-----------------------------------------------------------------------------
double ASC::getYLocation(unsigned short i_r) const
{
   return m_yllcorner + (m_nrows-i_r-1)*m_cellsize;
}

//-----------------------------------------------------------------------------
ASC::Pixel *ASC::getPixel(
        unsigned short int i_c,
        unsigned short int i_r
        )
{
    for(unsigned int i=0; i<m_pixels.size(); ++i)
    {
       if(m_pixels[i]->m_c==i_c && m_pixels[i]->m_r==i_r)
       {
          return m_pixels[i];
       }
    }
    return NULL;
}

//-----------------------------------------------------------------------------
void ASC::addToStack(
        ASC::Pixel *i_cPixel,
        unsigned short int i_currentSegmentID
        )
{
    i_cPixel->m_segmentID=i_currentSegmentID;
    ASC::Pixel *tmpPixel(NULL);
    tmpPixel=getPixel(i_cPixel->m_c-1,i_cPixel->m_r-1);
    if(tmpPixel!=NULL && tmpPixel->m_segmentID == -1)
    {
       tmpPixel->m_segmentID=i_currentSegmentID;
       m_pixelStack.push(tmpPixel);
    }

    tmpPixel=getPixel(i_cPixel->m_c-1,i_cPixel->m_r  );
    if(tmpPixel!=NULL && tmpPixel->m_segmentID == -1)
    {
       tmpPixel->m_segmentID=i_currentSegmentID;
       m_pixelStack.push(tmpPixel);
    }

    tmpPixel=getPixel(i_cPixel->m_c-1,i_cPixel->m_r+1);
    if(tmpPixel!=NULL && tmpPixel->m_segmentID == -1)
    {
       tmpPixel->m_segmentID=i_currentSegmentID;
       m_pixelStack.push(tmpPixel);
    }

    tmpPixel=getPixel(i_cPixel->m_c+1,i_cPixel->m_r-1);
    if(tmpPixel!=NULL && tmpPixel->m_segmentID == -1)
    {
       tmpPixel->m_segmentID=i_currentSegmentID;
       m_pixelStack.push(tmpPixel);
    }

    tmpPixel=getPixel(i_cPixel->m_c+1,i_cPixel->m_r  );
    if(tmpPixel!=NULL && tmpPixel->m_segmentID == -1)
    {
       tmpPixel->m_segmentID=i_currentSegmentID;
       m_pixelStack.push(tmpPixel);
    }
    tmpPixel=getPixel(i_cPixel->m_c+1,i_cPixel->m_r+1);
    if(tmpPixel!=NULL && tmpPixel->m_segmentID == -1)
    {
       tmpPixel->m_segmentID=i_currentSegmentID;
       m_pixelStack.push(tmpPixel);
    }

    tmpPixel=getPixel(i_cPixel->m_c  ,i_cPixel->m_r-1);
    if(tmpPixel!=NULL && tmpPixel->m_segmentID == -1)
    {
       tmpPixel->m_segmentID=i_currentSegmentID;
       m_pixelStack.push(tmpPixel);
    }

    tmpPixel=getPixel(i_cPixel->m_c  ,i_cPixel->m_r+1);
    if(tmpPixel!=NULL && tmpPixel->m_segmentID!= -1)
    {
       tmpPixel->m_segmentID=i_currentSegmentID;
       m_pixelStack.push(tmpPixel);
    }
}

//-----------------------------------------------------------------------------
void ASC::exportMidOfSegments(
        const std::string &i_ofile,
        const std::string &i_padFile
        )
{
   std::ofstream myfile;
   unsigned int currentSegmentID(0);

   // first find non-empty pixels
   for(unsigned short int c=1; c<m_ncols-1; ++c)
   {
      for(unsigned short int r=1; r<m_nrows-1; ++r)
      {
         if (m_values[getIndexInt(c,r)]<m_NODATA_value-0.0001 ||
                 m_values[getIndexInt(c,r)]>m_NODATA_value+0.0001)
         {
            m_pixels.push_back(new ASC::Pixel);
            m_pixels[m_pixels.size()-1]->m_c = c;
            m_pixels[m_pixels.size()-1]->m_r = r;
            m_pixels[m_pixels.size()-1]->m_segmentID = -1;
         }
      }
   }

   // seed growth - segmentation
   for(unsigned short int p=0; p<m_pixels.size(); ++p)
   {
       if(m_pixels[p]->m_segmentID!=-1)
       {
           continue;
       }
       m_pixels[p]->m_segmentID=currentSegmentID;

       addToStack(m_pixels[p],currentSegmentID);
       ASC::Pixel *tmp;
       while (!m_pixelStack.empty()) {
          tmp = m_pixelStack.top();
          m_pixelStack.pop();
          addToStack(tmp,currentSegmentID);
       }

       currentSegmentID++;
   }

   // calculate positions
   std::vector<PredictedLocation *> predicted_locations(currentSegmentID);
   for(unsigned int i=0; i<predicted_locations.size(); ++i)
   {
      predicted_locations[i]= new PredictedLocation;
      predicted_locations[i]->m_noOfPixels=0;
      predicted_locations[i]->m_x=0.0;
      predicted_locations[i]->m_y=0.0;
   }

   for(unsigned int i=0; i<m_pixels.size(); ++i)
   {
      assert(m_pixels[i]->m_segmentID != -1);
      assert(m_pixels[i]->m_segmentID < predicted_locations.size());
      predicted_locations[m_pixels[i]->m_segmentID]->m_noOfPixels++;
      predicted_locations[m_pixels[i]->m_segmentID]->m_y+=
                                     this->getYLocation(m_pixels[i]->m_r);
      predicted_locations[m_pixels[i]->m_segmentID]->m_x+=
                                     this->getXLocation(m_pixels[i]->m_c);
   }

   for(unsigned int i=0; i<predicted_locations.size(); ++i)
   {
      predicted_locations[i]->m_y/=predicted_locations[i]->m_noOfPixels;
      predicted_locations[i]->m_x/=predicted_locations[i]->m_noOfPixels;
   }



   // export data in file
   myfile.open (i_ofile);
   if(!myfile.is_open())
   {
      std::cout <<"ERROR: File \"" << i_ofile << "\" was not created ****"
                << "\n";
      std::exit(EXIT_FAILURE);
   }
   myfile <<std::fixed<<std::setprecision(4);
   myfile << "PAD,x_coordinate,y_coordinate,no_of_pixels\n";
   for(unsigned int i=0; i<predicted_locations.size(); ++i)
   {
       myfile << i_padFile << ","
              << predicted_locations[i]->m_x << ","
              << predicted_locations[i]->m_y << ","
              << predicted_locations[i]->m_noOfPixels << "\n";
   }

   myfile.close();
   for(unsigned int i=0; i<m_pixels.size(); ++i)
   {
      delete m_pixels[i];
   }
   for(unsigned int i=0; i<predicted_locations.size(); ++i)
   {
      delete predicted_locations[i];
   }
}


//-----------------------------------------------------------------------------
void ASC::exportASCTo(const std::string &i_fileName) const
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

    for(short int r=0;r<m_nrows; ++r)
    {
       unsigned short int c=0;
       for(;c<m_ncols-1; ++c)
       {
           assert(int(m_values.size())==m_ncols*m_nrows);
          assert(getIndexInt(c,r)<m_values.size());
          myfile << m_values[getIndexInt(c,r)] << " ";
       }
       assert(int(m_values.size())==m_ncols*m_nrows);
       assert(getIndexInt(c,r)<m_values.size());
       myfile << m_values[getIndexInt(c,r)] << "\n";
    }
    myfile.close();
    std::cout << "ASC file : " << i_fileName << " exported\n";
}


//-----------------------------------------------------------------------------
ASC::~ASC()
{

}

