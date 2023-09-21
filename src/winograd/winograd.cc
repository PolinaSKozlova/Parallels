#include "winograd.h"
// #include "../matrix/matrix.h"

namespace Parallels {
Matrix Winograd::MultiplyMatrices(Matrix& a, Matrix& b) {
if(!CheckMatrices(a, b)) {
    throw std::invalid_argument("Matrices are not compatible!");
} 
    int d = a.GetCols()/2;
    std::vector<double> rowFactor = FindRowFactors(a, d);
    std::vector<double> colFactor = FindColFactors(b, d);
    Matrix result = FindMatrix(a, b, rowFactor, colFactor, d);
    
    // if (result.GetRows() % 2 != 0 && result.GetCols() % 2 != 0) {
    if((a.GetCols()/2)*2 != a.GetCols()) {
        std::cout << "odd matrix" << std::endl;
        AddMembers(a, b, result);
    }
    return result;
}

 bool Winograd::CheckMatrices(Matrix& a, Matrix& b) {
    if(a.GetCols() != b.GetRows() ) return false;
    return true;
 }

        std::vector<double> Winograd::FindRowFactors(Matrix& a, int d){
       
        std::vector<double> row_factors(a.GetRows());
        for(int i = 0; i < a.GetRows(); i++){
            row_factors[i] = a.GetMatrix()[i][0]*a.GetMatrix()[i][1];
            for(int j = 1; j < d; j++){
                row_factors[i] = row_factors[i] + a.GetMatrix()[i][2*j]*a.GetMatrix()[i][2*j+1];
            }
        }
        return row_factors;
    }

 

        std::vector<double> Winograd::FindColFactors(Matrix& b,int d){
         std::vector<double> col_factors(b.GetCols());
        for(int i = 0; i < b.GetCols(); i++){
            col_factors[i] = b.GetMatrix()[0][i]*b.GetMatrix()[1][i];
            for(int j = 1; j < d; j++){
                col_factors[i] = col_factors[i] + b.GetMatrix()[2*j][i]*b.GetMatrix()[2*j+1][i];
            }
        }
         return col_factors;
    }



            Matrix Winograd::FindMatrix(Matrix& a, Matrix& b, 
    std::vector<double> rowFactor, std::vector<double> colFactor,int d){
        Matrix result_matrix(a.GetRows(), b.GetCols());

        for(int i = 0; i < a.GetRows(); i++){
            for(int j = 0; j < b.GetCols(); j++){
                result_matrix.GetMatrix()[i][j] = -rowFactor[i]-colFactor[j];
                for(int k = 0; k < d; k++){
                    result_matrix.GetMatrix()[i][j] = result_matrix.GetMatrix()[i][j]
                    +(a.GetMatrix()[i][2*k]+b.GetMatrix()[2*k+1][j]) * (a.GetMatrix()[i][2*k+1]+b.GetMatrix()[2*k][j]);    
                }
            }
        }
        
        return result_matrix;
        }

        void Winograd::AddMembers(Matrix& a, Matrix& b, Parallels::Matrix& result){
        for(int i = 0; i < a.GetRows(); i++){
            for(int j = 0; j < b.GetCols(); j++){
                result.GetMatrix()[i][j] = result.GetMatrix()[i][j] 
                + a.GetMatrix()[i][a.GetCols()-1]*b.GetMatrix()[a.GetCols()-1][j];
            }
        }
    }



    
}; // namespace Parallels