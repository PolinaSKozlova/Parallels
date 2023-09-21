#ifndef PARALLELS_WINOGRAD_H
#define PARALLELS_WINOGRAD_H

#include <vector>
#include "../matrix/matrix.h"

namespace Parallels {
    class Winograd {

    public:
    Matrix MultiplyMatrices(Matrix& a, Matrix& b);

    private:
    bool CheckMatrices(Matrix& a, Matrix& b);
    std::vector<double> FindRowFactors(Matrix& a, int d);
    std::vector<double> FindColFactors(Matrix& b, int d);
    Matrix FindMatrix(Matrix& a, Matrix& b, std::vector<double> rowFactor, std::vector<double> colFactor, int d);
    void AddMembers(Matrix& a, Matrix& b, Matrix& result);

        
    };
}; // namespace Parallels


#endif  // PARALLELS_WINOGRAD_H