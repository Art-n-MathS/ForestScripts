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
/// order reverse during exportation
///

class ASC
{
public: //-------------------------------------------------------------------------
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
    /// reduction
    //-------------------------------------------------------------------------
    void saltNpepper();
    //-------------------------------------------------------------------------
    /// @brief method that applies averaging filtering
    //-------------------------------------------------------------------------
    void smooth();
    //-------------------------------------------------------------------------
    /// @brief method tha exports current ASC file int i_fileName
    /// @param[in] i_fileName: name of file to be exported
    //-------------------------------------------------------------------------
    void exportTo(const std::string &i_fileName)const;
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~ASC();

private:
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
            const unsigned short int i_c,
            const unsigned short int i_r
            )const;
    //-------------------------------------------------------------------------
    /// @brief number of columns
    //-------------------------------------------------------------------------
    unsigned short int m_ncols;
    //-------------------------------------------------------------------------
    /// @brief number of rows
    //-------------------------------------------------------------------------
    unsigned short int m_nrows;
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




};


#endif // ASC_H
