#include "CSVManager.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include<sstream>


//-----------------------------------------------------------------------------
CSVManager::CSVManager(
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
        ):
    m_posSamples(i_ipos,i_cols,i_weights,i_heightCol,i_heightThres),
    m_negSamples(i_ineg,i_cols,i_weights,i_heightCol,i_heightThres),
    m_iall(i_iall),
    m_oKNN(i_oKNN),
    m_keeps(i_keeps),
    m_k(i_k)
{
   std::cout << "Initialisation of CSVManager completed:";
   std::cout << "\nm_iall      = " << m_iall ;
   std::cout << "\nm_k         = " << m_k;

   std::cout << "\nm_keeps     = " ;
   for(unsigned int i=0; i<m_keeps.size(); ++i)
   {
      std::cout << m_keeps[i] << " ";
   }
   std::cout << "\n\n";
}

//-----------------------------------------------------------------------------
void CSVManager::interpret()
{
   std::ifstream inFile;
   std::string line;
   inFile.open(m_iall.c_str());
   if(!inFile)
   {
      std::cerr << "ERROR: File \"" << m_iall << "\" doesn't exist\n";
      std::exit(EXIT_FAILURE);
   }
   std::ofstream myfile;
   myfile.open(m_oKNN);
   if (!myfile)
   {
      std::cerr << "ERROR: Could not create \"" << m_oKNN << "\" file\n";
      std::exit(EXIT_FAILURE);
   }

   myfile << "Hello World";
   std::vector<std::string> labels;
   std::cout << "   ***  Starting KNN  ***\n";
   if (std::getline(inFile,line)) // skip labels
   {
      std::istringstream ss( line );
      while (ss)
      {
         std::string subS;
         if (!std::getline( ss, subS, ',' )) break;
         labels.push_back(subS);
      }
      for(unsigned int i=0; i<m_keeps.size(); ++i)
      {
         myfile << labels[m_keeps[i]] << ",";
      }
      myfile << "KNN\n";
      /// TO DO: export keeps and m_cols to output KNN file
   }

   while(std::getline(inFile,line) )
   {
     if (line.c_str()[0]==',')continue;
     labels.clear();
     std::istringstream ss( line );
     while (ss)
     {
        std::string subS;
        if (!std::getline( ss, subS, ',' )) break;
        labels.push_back(subS);
     }
     for(unsigned int i=0; i<m_keeps.size(); ++i)
     {
        myfile << labels[m_keeps[i]] << ",";
     }
     double *posKNN = m_posSamples.getNearestValues(line,m_k);
     double sumpos(0.0);
     for(unsigned int k=0; k<m_k; ++k)
     {
        sumpos+=posKNN[k];
     }
     if(sumpos<0.00001)
     {
        myfile << 0 << "\n";
        delete posKNN;
        continue;
     }

     double *negKNN = m_negSamples.getNearestValues(line,m_k);

     std::vector<double> posKNNSelected, negKNNSelected;
     unsigned short int countPos(0),countNeg(0);
     double sumPos(0.0),sumNeg(0.0);
     while (posKNNSelected.size()+negKNNSelected.size()<m_k)
     {
         if(posKNN[countPos]<=negKNN[countNeg])
         {
            posKNNSelected.push_back(100.0/posKNN[countPos]);
            sumPos+=100.0/posKNN[countPos];
            countPos++;
         }
         else
         {
            negKNNSelected.push_back(100.0/negKNN[countNeg]);
            sumNeg+=100.0/negKNN[countNeg];
            countNeg++;
         }
     }


     myfile << 100.0*sumPos/(sumNeg+sumPos) << "\n";


     delete posKNN;
     delete negKNN;
   }

    myfile.close();
    inFile.close();
}


//-----------------------------------------------------------------------------
CSVManager::~CSVManager()
{
}
