#ifndef PARALLELS_WINOGRAD_H
#define PARALLELS_WINOGRAD_H

#include <vector>

namespace Parallels {
    class Winograd {

    public:
    Parallels::Matrix MultiplyMatrices(Parallels::Matrix& a, Parallels::Matrix& b);

    private:
    bool CheckMatrices(Parallels::Matrix& a, Parallels::Matrix& b);
    std::vector<double> FindRowFactors(Parallels::Matrix& a, int d);
    std::vector<double> FindColFactors(Parallels::Matrix& b, int d);
    Parallels::Matrix FindMatrix(Parallels::Matrix& a, Parallels::Matrix& b, double rowFactor, double colFactor, int d);
    void AddMembers(Parallels::Matrix& a, Parallels::Matrix& b, Parallels::Matrix& result);

        
    }
}; // namespace Parallels


#endif  // PARALLELS_WINOGRAD_H