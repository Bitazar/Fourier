#pragma once

#include <MPGL/MPGL.hpp>

class FourierDrawer : public mpgl::Transformable2D,
    public mpgl::Drawable2D
{
public:
    using Points = std::vector<mpgl::Vector2f>;

    explicit FourierDrawer(void);

    void setPoints(Points points);

    void start(void)
        { isDrawing = true; }

    void clear(void);

    mpgl::Vector2f draw(mpgl::Vector2f const& start, size_t steps);

    void draw(void) const noexcept;

    size_t size(void) const noexcept
        { return driver.size(); }

    void transform(mpgl::Transformation2D const& transformator) noexcept;
private:
    bool isDrawing = false;

    mpgl::DrawableCollection<mpgl::Line2D> lines;
    std::chrono::steady_clock::time_point lastStamp;
    std::chrono::milliseconds elapsedTime;
    Points driver;
    size_t counter;
};
