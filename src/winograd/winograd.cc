#include "winograd.h"
// #include "../matrix/matrix.h"

namespace Parallels {
Matrix Winograd::MultiplyMatrices(Matrix& a, Matrix& b) {
if(!CheckMatrices(a, b)) {
    throw std::invalid_argument("Matrices are not compatible!");
} 
    int d = a.GetCols()/2;
    std::vector<double> rowFactor = FindRowFactors(a, d);
    std::cout << rowFactor[1] << std::endl;
    std::vector<double> colFactor = FindColFactors(b, d);
    Matrix result = FindMatrix(a, b, rowFactor, colFactor, d);
    result.PrintMatrix();

    if (result.GetRows() % 2 != 0 && result.GetCols() % 2 != 0) {
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
        for(int i = 1; i < a.GetRows(); i++){
            row_factors[i] = a.GetMatrix()[i][1]*a.GetMatrix()[i][2];
            for(int j = 2; j < d; j++){
                row_factors[i] = row_factors[i] + a.GetMatrix()[i][2*j-1]*a.GetMatrix()[i][2*j];
            }
        }
        return row_factors;
    }

    std::vector<double> Winograd::FindColFactors(Matrix& b,int d){
         std::vector<double> col_factors(b.GetCols());
        for(int i = 1; i < b.GetCols(); i++){
            col_factors[i] = b.GetMatrix()[1][i]*b.GetMatrix()[2][i];
            for(int j = 2; j < d; j++){
                col_factors[i] = col_factors[i] + b.GetMatrix()[2*j-1][i]*b.GetMatrix()[2*j][i];
            }
        }
         return col_factors;
    }


    Matrix Winograd::FindMatrix(Matrix& a, Matrix& b, 
    std::vector<double> rowFactor, std::vector<double> colFactor,int d){
        Matrix result_matrix(a.GetRows(), b.GetCols());

        for(int i = 1; i < a.GetRows(); i++){
            for(int j = 1; j < b.GetCols(); j++){
                result_matrix.GetMatrix()[i][j] = -rowFactor[i]-colFactor[j];
                for(int k = 1; k < d; k++){
                    result_matrix.GetMatrix()[i][j] = result_matrix.GetMatrix()[i][j]
                    +(a.GetMatrix()[i][2*k-1]+b.GetMatrix()[2*k][j]) * (a.GetMatrix()[i][2*k]+b.GetMatrix()[2*k-1][j]);    
                }
            }
        }
        
        std::cout << "Result matrix:" << std::endl;
        result_matrix.PrintMatrix();
        
        return result_matrix;
        }
    void Winograd::AddMembers(Matrix& a, Matrix& b, Parallels::Matrix& result){
        for(int i = 1; i < a.GetRows(); i++){
            for(int j = 1; j < b.GetCols(); j++){
                result.GetMatrix()[i][j] = result.GetMatrix()[i][j] 
                + a.GetMatrix()[i][a.GetCols()]*b.GetMatrix()[a.GetCols()][j];
            }
        }
    }



    
}; // namespace Parallels