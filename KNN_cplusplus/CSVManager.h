#ifndef CSVMANAGER_H
#define CSVMANAGER_H


//-----------------------------------------------------------------------------
/// @file class CSVManager (CSVManager.h)
/// @author Dr Milto Miltiadou
/// @version 1.0
/// @date 16/04/19
/// @brief this class manages pos and neg samples and performs KNN on a new
/// @briefgiven .csv file while reading it line by line
//-----------------------------------------------------------------------------
#include <iostream>
#include "CSV.h"

class CSVManager
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    CSVManager(
            const std::string &i_ipos,
            const std::string &i_ineg,
            const std::string &i_iall,
            const std::string &i_oKNN,
            unsigned short int i_heightCol,
            const std::vector<unsigned short int> &i_cols,
            const std::vector<unsigned short int> &i_weights,
            const std::vector<unsigned short int> &i_keeps,
            const unsigned short int i_k,
            double i_heightThres
            );
    //-------------------------------------------------------------------------
    /// @brief method that interprets data and exports KNN results into .csv
    //-------------------------------------------------------------------------
    void interpret();
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~CSVManager();

private:
    //-------------------------------------------------------------------------
    /// @brief positive samples
    //-------------------------------------------------------------------------
    CSV m_posSamples;
    //-------------------------------------------------------------------------
    /// @brief negative samples
    //-------------------------------------------------------------------------
    CSV m_negSamples;
    //-------------------------------------------------------------------------
    /// @brief name of file to be classified with KNN
    //-------------------------------------------------------------------------
    const std::string m_iall;
    //-------------------------------------------------------------------------
    /// @brief file to be exported with KNN results
    //-------------------------------------------------------------------------
    const std::string m_oKNN;
    //-------------------------------------------------------------------------
    /// @brief Columns that will be exported with the results of the KNN
    //-------------------------------------------------------------------------
    const std::vector<unsigned short int> m_keeps;
    //-------------------------------------------------------------------------
    /// @brief the number of k in the KNN algorithm
    //-------------------------------------------------------------------------
    const unsigned short int m_k;

};

#endif // CSVMANAGER_H
