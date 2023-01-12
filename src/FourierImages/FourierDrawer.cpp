#include "FourierImages/FourierDrawer.hpp"

#include <numbers>

FourierDrawer::FourierDrawer(double period)
    : elapsedTime{0}, period{period / (2. * std::numbers::pi)} {}

void FourierDrawer::setPoints(Points points) {
    isDrawing = true;
    driver = points;
    lastStamp = std::chrono::steady_clock::now();
}

void FourierDrawer::clear(void) {
    isDrawing = false;
    lines.clear();
}

void FourierDrawer::transform(mpgl::Transformation2D const& transformator) noexcept {
    lines.transform(transformator);
}

mpgl::Vector2f FourierDrawer::draw(mpgl::Vector2f const& start, size_t steps, double shift) {
    elapsedTime += std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - lastStamp);
    lastStamp = std::chrono::steady_clock::now();

    double freq = elapsedTime.count() / (1000. * period);
    auto [lastX, lastY] = start;

    lines.clear();

    for (size_t i = 0; i < std::min(steps, driver.size()); ++i) {
        auto const& [radius, phase] = driver[i];
        double const phi = phase + freq * i + shift;

        double const x = lastX + radius * std::cos(phi);
        double const y = lastY - radius * std::sin(phi);

        lines.emplace_back(mpgl::Vector2f{lastX, lastY}, mpgl::Vector2f{x, y});
        lastX = x;
        lastY = y;
    }
    return {lastX, lastY};
}

void FourierDrawer::draw(void) const noexcept {
    lines.draw();
}
