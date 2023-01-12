#pragma once

#include <MPGL/MPGL.hpp>

class FourierDrawer : public mpgl::Transformable2D,
    public mpgl::Drawable2D
{
public:
    using Points = std::vector<mpgl::Vector2f>;

    explicit FourierDrawer(double period);

    void setPoints(Points points);

    void start(void)
        { isDrawing = true; }

    void clear(void);

    mpgl::Vector2f draw(mpgl::Vector2f const& start, size_t steps, double shift = 0);

    void draw(void) const noexcept;

    void transform(mpgl::Transformation2D const& transformator) noexcept;
private:
    bool isDrawing = false;

    mpgl::DrawableCollection<mpgl::Line2D> lines;
    std::chrono::steady_clock::time_point lastStamp;
    std::chrono::milliseconds elapsedTime;
    Points driver;
    double const period;
};
