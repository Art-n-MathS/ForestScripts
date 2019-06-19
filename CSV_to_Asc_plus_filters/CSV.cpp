#include "CSV.h"


//-----------------------------------------------------------------------------
CSV::CSV(
        const std::string &i_csvFileName,
        const unsigned short int i_xCol,
        const unsigned short int i_yCol,
        const unsigned short int i_pCol
        )
{
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
