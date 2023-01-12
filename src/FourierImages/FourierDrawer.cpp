#include "FourierImages/FourierDrawer.hpp"

#include <numbers>

FourierDrawer::FourierDrawer(void)
    : elapsedTime{0}, counter{0} {}

void FourierDrawer::setPoints(Points points) {
    isDrawing = true;
    driver = points;
    lastStamp = std::chrono::steady_clock::now();
}

void FourierDrawer::clear(void) {
    isDrawing = false;
    counter = 0;
    lines.clear();
}

void FourierDrawer::transform(mpgl::Transformation2D const& transformator) noexcept {
    lines.transform(transformator);
}

static mpgl::Vector2f complexMul(mpgl::Vector2f const& left, mpgl::Vector2f const& right) {
    return {
        left[0] * right[0] - left[1] * right[1],
        left[0] * right[1] + left[1] * right[0]
    };
}

mpgl::Vector2f FourierDrawer::draw(mpgl::Vector2f const& start, size_t steps) {
    elapsedTime += std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - lastStamp);
    lastStamp = std::chrono::steady_clock::now();

    mpgl::Vector2f point{};

    lines.clear();

    for (size_t i = 0; i < steps; ++i) {
        double value = 2. * std::numbers::pi * i * counter / driver.size();
        auto current = complexMul(driver[i], { std::cos(value), std::sin(value) }) / float(driver.size());

        lines.emplace_back(start + point, start + point + current);
        point += current;
    }

    counter = (counter + 1) % driver.size();

    return point + start;
}

void FourierDrawer::draw(void) const noexcept {
    lines.draw();
}
