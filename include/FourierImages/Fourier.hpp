#pragma once

#include <MPGL/MPGL.hpp>

class Fourier {
public:
    constexpr Fourier(void) noexcept = default;

    using Points = std::vector<mpgl::Vector2f>;

    [[nodiscard]] Points operator()(Points data) const;

private:
    using Complex = std::vector<std::complex<double>>;

    [[nodiscard]] Points interpolate(Points const& data) const;

    [[nodiscard]] Complex preprocessing(Points const& data) const;

    [[nodiscard]] Points postprocessing(Complex const& data) const;
};

inline constexpr Fourier fourier{};

std::vector<std::complex<double>> fft(std::vector<std::complex<double>> const& data);

