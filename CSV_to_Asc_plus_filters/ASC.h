#ifndef ASC_H
#define ASC_H

//-----------------------------------------------------------------------------
/// @file class CSV (CSV.h)
/// @author Dr Milto Miltiadou
/// @version 1.0
/// @date 16/05/19
/// @brief this class interpretates an ASC file
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
#include <stack>

//   std::cout << "ncols "        << ncols << "\n"
//             << "nrows "        << nrows << "\n"
//             << "xllcorner "    << xllcorner << "\n"
//             << "yllcorner "    << yllcorner << "\n"
//             << "cellsize "     << cellsize << "\n"
//             << "NODATA_value " << NODATA_value << "\n";

/// order of .asc file:
/// row1: col1, col2, ... colm
/// row2: col1, col2, ... colm
/// ...
/// rown: col1, col2, ... colm
///
///
/// order of data been interpreted
/// rown: col1, col2, ... colm
/// ...
/// row2: col1, col2, ... colm
/// row1: col1, col2, ... colm
///
///
///

class ASC
{
public:
    typedef struct Geolocation{
       double m_x;
       double m_y;
    }Geolocation;

    typedef struct PredictedLocation{
       double m_x;
       double m_y;
       short int m_noOfPixels;
    }PredictedLocation;

    typedef struct Pixel{
       unsigned short int m_c;
       unsigned short int m_r;
       short int m_segmentID;
    }Pixel;
//-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    ASC(unsigned short int i_ncolsX,
            unsigned short int i_nrowsY,
            double i_xllxcorner,
            double i_yllcorner,
            double i_cellsize,
            double i_noDataValue
            );
    //-------------------------------------------------------------------------
    /// @brief method that adds a value to the asc file
    //-------------------------------------------------------------------------
    void addValue(double i_x, double i_y, double value);
    //-------------------------------------------------------------------------
    /// @brief method that applies median filtering for salt and pepper noise
    /// reduction, when a no value exist it ignores it.
    //-------------------------------------------------------------------------
    void saltNpepper();
    //-------------------------------------------------------------------------
    /// @brief method that applies median filtering for salt and pepper noise
    /// reduction and NoValue is a value. If median is noValue then noValue is
    /// given
    //-------------------------------------------------------------------------
    void saltNpepperWithNoValue();
    //-------------------------------------------------------------------------
    /// @brief method that returns a pointer to the ascs values
    //-------------------------------------------------------------------------
    std::vector<double> *getValuesPointer(){return &m_values;}
    //-------------------------------------------------------------------------
    /// @brief method that applies a seed growth algorithm for identifying
    /// @brief different segments and then exports the average geolocation of
    /// @brief each segment into a .csv file
    /// @param[in] i_file csv file to be exported
    //-------------------------------------------------------------------------
    void exportMidOfSegments(
            const std::string &i_ofile,
            const std::string &i_padFile
            );
    //-------------------------------------------------------------------------
    /// @brief method that thresholds: assigned no value to any pixel value
    /// below the given thresshold
    /// @param[in] i_thres the threshold
    //-------------------------------------------------------------------------
    void threshold(
            const double i_thres
            );
    //-------------------------------------------------------------------------
    /// @brief method that thresholds using another values array: assigned no
    /// value to any pixel value withing this.m_values that its corresponding
    /// value in the i_values array is below the given thresshold
    /// @param[in] i_thres the threshold
    /// @param[in] i_values the values of the array that will be used for
    /// thresholding (e.g. assign zero to any pixel of the KNN results whose
    /// corresponding height is less than 20)
    //-------------------------------------------------------------------------
    void threshold(
            const double i_thres,
            const std::vector<double> *i_valuesH)
    ;
    //-------------------------------------------------------------------------
    /// @brief method that applies averaging filtering
    //-------------------------------------------------------------------------
    void smooth();
    //-------------------------------------------------------------------------
    /// @brief method tha exports current ASC file int i_fileName
    /// @param[in] i_fileName: name of file to be exported
    //-------------------------------------------------------------------------
    void exportASCTo(const std::string &i_fileName)const;
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~ASC();

private:
    //-------------------------------------------------------------------------
    /// @brief method that takes as input the (x,y) indices of a pixel and
    /// @brief returns their geolocations
    /// @param[in] i_c the index of the column
    /// @param[in] i_r the index of the row
    /// @note returned coordinates dynamically allocated => memory needs to
    /// @note be freed after the call of this function
    //-------------------------------------------------------------------------
    Geolocation *getGeolocation(
            unsigned short int i_c,
            unsigned short int i_r
            )const;
    //-------------------------------------------------------------------------
    /// @brief method that takes as input the number of coloumn and returns the
    /// @brief corresponding x coordinate
    /// @param[in] i_c the index of the column
    /// @returns the corresponding x coordinate of the column
    //-------------------------------------------------------------------------
    double getXLocation(
            unsigned short int i_c
            )const;
    //-------------------------------------------------------------------------
    /// @brief method that takes as input the number of row and returns the
    /// @brief corresponding y coordinate
    /// @param[in] i_r the index of the column
    /// @returns the corresponding y coordinate of the column
    //-------------------------------------------------------------------------
    double getYLocation(
            unsigned short int i_r
            )const;
    //-------------------------------------------------------------------------
    /// @brief method that gets as input the (x,y) geolocations and returns
    /// the index value of m_values of the corresponding pixel
    //-------------------------------------------------------------------------
    unsigned int getIndex(const double i_x, const double i_y)const;
    //-------------------------------------------------------------------------
    /// @brief method that gets as input the (c,r) indexes of rows and cols
    /// and returns the index value of m_values of the corresponding pixel
    //-------------------------------------------------------------------------
    unsigned int getIndexInt(
            const short int i_c,
            const short int i_r
            )const;
    //-------------------------------------------------------------------------
    /// @brief number of columns
    //-------------------------------------------------------------------------
    short int m_ncols;
    //-------------------------------------------------------------------------
    /// @brief number of rows
    //-------------------------------------------------------------------------
    short int m_nrows;
    //-------------------------------------------------------------------------
    /// @brief x value at lower left corner
    //-------------------------------------------------------------------------
    double m_xllcorner;
    //-------------------------------------------------------------------------
    /// @brief y value at lower left corner
    //-------------------------------------------------------------------------
    double m_yllcorner;
    //-------------------------------------------------------------------------
    /// @brief cell size
    //-------------------------------------------------------------------------
    double m_cellsize;
    //-------------------------------------------------------------------------
    /// @brief value indicating no data
    //-------------------------------------------------------------------------
    double m_NODATA_value;
    //-------------------------------------------------------------------------
    /// @brief vector containing pixel values
    //-------------------------------------------------------------------------
    std::vector<double> m_values;


    //-------------------------------------------------------------------------
    /// @brief stack used to temporarly store pixels during seed growth
    //-------------------------------------------------------------------------
    std::stack<Pixel  *> m_pixelStack;
    std::vector<Pixel *> m_pixels;
    //-------------------------------------------------------------------------
    /// @brief method that returns pointer to pixel from pixels array given
    /// column and row within image - used in seed growth only. It returns NULL
    /// if pixel does not exists (which implies, it contains no data
    /// @param[in] i_c column of pixel
    /// @param[in] i_r row of pixel
    /// @returns pointer to the pixel
    //-------------------------------------------------------------------------
    ASC::Pixel *getPixel(
            unsigned short int i_c,
            unsigned short int i_r
            );
    //-------------------------------------------------------------------------
    void addToStack(
            Pixel *i_currentPixel,
            unsigned short i_currentSegmentID
            );




};


#endif // ASC_H
