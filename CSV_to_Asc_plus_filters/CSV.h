#ifndef CSV_H
#define CSV_H

//-----------------------------------------------------------------------------
/// @file class CSV (CSV.h)
/// @author Dr Milto Miltiadou
/// @version 1.0
/// @date 16/04/19
/// @brief this class interpretates a CSV file
//-----------------------------------------------------------------------------

#include <vector>
#include "ASC.h"
#include <string>

class CSV
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    CSV(
            const std::string &i_csvFileName,
            const unsigned short int i_xCol,
            const unsigned short int i_yCol,
            const unsigned short int i_pCol
            );
    //-------------------------------------------------------------------------
    /// @brief method that dynamically allocates memory for an ASC. It uses
    /// the x,y values to find the positions of the pixels and the p values
    /// are assigned to the corresponding pixel values
    /// @note Memory needs to be freed outside this class
    //-------------------------------------------------------------------------
    ASC *getASC()const;
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~CSV();

private:
    //-------------------------------------------------------------------------
    /// @brief vector with all the x values
    //-------------------------------------------------------------------------
    std::vector<double> m_xValues;
    //-------------------------------------------------------------------------
    /// @brief vector with all the y values
    //-------------------------------------------------------------------------
    std::vector<double> m_yValues;
    //-------------------------------------------------------------------------
    /// @brief vector with all the pixel values
    //-------------------------------------------------------------------------
    std::vector<double> m_pValues;


};

#endif // CSV_H
