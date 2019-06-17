#ifndef CSV_H
#define CSV_H

//-----------------------------------------------------------------------------
/// @file class CSV (CSV.h)
/// @author Dr Milto Miltiadou
/// @version 1.0
/// @date 16/04/19
/// @brief this class interpretates a CSV file
//-----------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <assert.h>

class CSV
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    /// @param[in] i_name: the name with directory of the CSV file to be loaded
    /// @param[in] i_isPositive indicates if this CSV files contains positive
    /// or negative samples: 0 for negative and 1 for positive
    //-------------------------------------------------------------------------
    CSV(const std::string &i_name);


    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~CSV();

private:
    //-------------------------------------------------------------------------
    /// @brief name of file with directory to be interpreted
    //-------------------------------------------------------------------------
    std::string m_csvFile;
    //-------------------------------------------------------------------------
    /// @brief number of columns
    //-------------------------------------------------------------------------
    unsigned int m_cols;
    //-------------------------------------------------------------------------
    /// @brief number of rows (without the labels row)
    //-------------------------------------------------------------------------
    unsigned int m_rows;
    //-------------------------------------------------------------------------
    /// @brief labels of the CSV file
    //-------------------------------------------------------------------------
    std::vector<std::string> m_labels;
    //-------------------------------------------------------------------------
    /// @brief lines of CSV file (without the labels) stored in 1D vector with
    /// len equal to : [m_labels.size()*(NoOfLines-1)] : -1 for the lables line
    //-------------------------------------------------------------------------
    std::vector<std::string> m_values;
    //-------------------------------------------------------------------------
    /// @brief indicates if this CSV files contains positive or negative samples
    /// @brief 0 for negative and 1 for positive
    //-------------------------------------------------------------------------
    bool m_isPositive;
};

#endif // CSV_H
