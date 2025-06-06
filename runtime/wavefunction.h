#ifndef QPP_WAVEFUNCTION_H
#define QPP_WAVEFUNCTION_H

#include <complex>
#include <array>

namespace qpp {
class Wavefunction {
public:
    Wavefunction();
    void apply_h();
    void apply_x();
    int measure() const;
    std::array<std::complex<double>, 2> state;
};
}

#endif // QPP_WAVEFUNCTION_H
