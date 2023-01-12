#include <FourierImages/Fourier.hpp>

extern "C" {
    #include <fftw3.h>
}

#include <iostream>

[[nodiscard]] Fourier::Points Fourier::interpolate(Points const& data) const {
    Points result;
    for (size_t i = 1; i < data.size(); ++i) {
        auto const& [x, y] = data[i - 1];
        auto const& [xe, ye] = data[i];
        result.emplace_back(x, y);
        for (float delta = 0.1; delta < 1; delta += 0.5)
            result.emplace_back(std::lerp(x, xe, delta), std::lerp(y, ye, delta));
    }
    result.emplace_back(data.front());
    return result;
}

[[nodiscard]] Fourier::Complex Fourier::preprocessing(Points const& data) const {
    Complex result;
    size_t const limit = 1 << size_t(std::ceil(std::log2(data.size())));
    size_t const diff = limit - data.size();
    if (not diff) {
        for (auto const& [x, y] : data)
            result.emplace_back(x, y);
        return result;
    }
    size_t counter = 0;
    for (auto const& [x, y] : data) {
        if (counter++ < diff)
            result.emplace_back(x, y);
        result.emplace_back(x, y);
    }
    return result;
}

[[nodiscard]] Fourier::Points Fourier::postprocessing(Complex const& data) const {
    Points result;
    for (auto const& number : data)
        result.push_back(mpgl::cartesianToPolar(mpgl::Vector2f{number.real(), -number.imag()} / float(data.size())));
    return result;
}

[[nodiscard]] Fourier::Points Fourier::operator()(Points data) const {
    return postprocessing(fft(preprocessing(interpolate(data))));
}

static void insert(std::vector<std::complex<double>> const& data, fftw_complex* target) {
    for (auto const& complex : data) {
        (*target)[0] = complex.real();
        (*target++)[1] = complex.imag();
    }
}

static void extract(std::vector<std::complex<double>>& data, fftw_complex* target, size_t size) {
    data.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        std::complex<double> complex{};
        complex.real((*target)[0]);
        complex.imag((*target++)[1]);
        data.push_back(complex);
    }
}

std::vector<std::complex<double>> fft(std::vector<std::complex<double>> const& data) {
    fftw_complex *in, *out;
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * data.size());
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * data.size());

    insert(data, in);

    auto* plan = fftw_plan_dft_1d(data.size(), in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    std::vector<std::complex<double>> result;
    extract(result, out, data.size());

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
    return result;
}
