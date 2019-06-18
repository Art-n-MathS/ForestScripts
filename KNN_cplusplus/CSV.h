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
    CSV(
            const std::string &i_name,
            const std::vector<unsigned short int> &i_cols,
            const std::vector<unsigned short int> &i_weights,
            unsigned short int i_heightCol,
            double i_heightThres
            );
    //-------------------------------------------------------------------------
    /// @brief method that returns the number of samples stored
    //-------------------------------------------------------------------------
    int getNoOfSamples()const;
    //-------------------------------------------------------------------------
    /// @brief method that sets m_noRowsOfPosSamples
    //-------------------------------------------------------------------------
    void setNoRowsOfPosSamples(int i_noOfPosSamples);
    //-------------------------------------------------------------------------
    /// @brief method that returns the k nearest values
    /// @param[in] i_line line of data to be interpreted
    /// @param[in] i_k number of nearests k values to be returned
    /// @param[out] a dynamically allocated list of KNN values
    //-------------------------------------------------------------------------
    double *getNearestValues(
            const std::string &i_line,
            const unsigned short int i_k
            )const;
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
    unsigned int m_noCols;
    //-------------------------------------------------------------------------
    /// @brief number of rows (without the labels row)
    //-------------------------------------------------------------------------
    unsigned int m_noRows;
    //-------------------------------------------------------------------------
    /// @brief number of rows of Positive Samples, since they are less in
    /// number and need to reduce the number of Neg Samples to reduce bias
    /// @note if this class contains pos samples then it takes the value "-1.0"
    //-------------------------------------------------------------------------
    int m_noRowsOfPosSamples;
    //-------------------------------------------------------------------------
    /// @brief Cols of features to be taken into consideration while performing
    /// the KNN algorithm
    //-------------------------------------------------------------------------
    std::vector<unsigned short int> m_cols;
    //-------------------------------------------------------------------------
    /// @brief weight of each feature (each weight corresponds to a column)
    //-------------------------------------------------------------------------
    std::vector<unsigned short int> m_weights;
    //-------------------------------------------------------------------------
    /// @brief number of column containing heights of middle column
    //-------------------------------------------------------------------------
    const unsigned int m_heightCol;
    //-------------------------------------------------------------------------
    /// @brief max height to non be considered as ground
    //-------------------------------------------------------------------------
    const double m_heightThres;
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
