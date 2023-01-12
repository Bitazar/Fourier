#include <MPGL/MPGL.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <complex>
#include <algorithm>
#include <math.h>
#include <numbers>

#include <FourierImages/Fourier.hpp>
#include <FourierImages/FourierDrawer.hpp>

using namespace std::literals;

class Canva : public mpgl::Drawable2D,
              public mpgl::Transformable2D,
              public mpgl::MouseMotionEvent,
              public mpgl::MousePressEvent,
              public mpgl::MouseReleaseEvent,
              public mpgl::KeyPressEvent,
              public mpgl::TickEvent
{
   public:
    Canva(void) noexcept
        : font{"Hack", "/usr/share/fonts/truetype/hack/"},
          drawer{}
    {
        texts.emplace("coeff", mpgl::Text2D{font, mpgl::Vector2f{100, 140}, "Liczba współczynników: 1",
               mpgl::TextOptions{.size = 16, .color=mpgl::colors::white}});
        texts.emplace("t1", mpgl::Text2D{font, mpgl::Vector2f{100, 120}, "Zwiększ liczbę współczynników - strzałka w górę",
               mpgl::TextOptions{.size = 16, .color=mpgl::colors::white}});
        texts.emplace("t2", mpgl::Text2D{font, mpgl::Vector2f{100, 100}, "Zmniejsz liczbę współczynników - strzałka w dół",
               mpgl::TextOptions{.size = 16, .color=mpgl::colors::white}});
        texts.emplace("t3", mpgl::Text2D{font, mpgl::Vector2f{100, 80}, "Maksymalna liczba współczynników: N",
               mpgl::TextOptions{.size = 16, .color=mpgl::colors::white}});
        texts.emplace("t4", mpgl::Text2D{font, mpgl::Vector2f{100, 60}, "Minimalna liczba współczynników: O",
               mpgl::TextOptions{.size = 16, .color=mpgl::colors::white}});
        texts.emplace("t5", mpgl::Text2D{font, mpgl::Vector2f{100, 40}, "Wyczyść płutno: C",
               mpgl::TextOptions{.size = 16, .color=mpgl::colors::white}});
        texts.emplace("t6", mpgl::Text2D{font, mpgl::Vector2f{100, 20}, "Wyczyść tylko rysowany kontur: J",
               mpgl::TextOptions{.size = 16, .color=mpgl::colors::white}});
        cross.emplace_back(mpgl::Vector2f{430, 300}, mpgl::Vector2f{470, 300},  mpgl::colors::gray);
        cross.emplace_back(mpgl::Vector2f{450, 280}, mpgl::Vector2f{450, 320},  mpgl::colors::gray);
    }

    void draw(void) const noexcept {
        points.draw();
        outline.draw();
        drawer.draw();
        for (auto const& [_, text] : texts)
            text.draw();
        cross.draw();
    }

    void transform(mpgl::Transformation2D const& transformator) noexcept {
        points.transform(transformator);
        outline.transform(transformator);
        drawer.transform(transformator);
        for (auto& [_, text] : texts)
            text.transform(transformator);
        cross.transform(transformator);
    }

    void onMouseMotion(mpgl::Vector2f const& position) noexcept {
        static constexpr mpgl::Vector2f center{450, 300};
        if (isClicked) {
            pointsShape.emplace_back(position);
            points = mpgl::LineStrip2D{};
            for (auto const& x : pointsShape)
                points.emplace(x, mpgl::colors::gray);

            auto [x, y] = position - center;
            seeds.emplace_back(x, y);
        }
    }

    void onMousePress(mpgl::MouseButton const& button) noexcept {
        if (button == mpgl::MouseButton::Left) isClicked = true;
    }

    void onMouseRelease(mpgl::MouseButton const& button) noexcept {
        if (button == mpgl::MouseButton::Left) {
            isClicked = false;

            drawer.setPoints(fourier(seeds));

            drawers = drawer.size();
            texts.at("coeff") = "Liczba współczynników: " + std::to_string(drawers);

            isDrawing = true;
        }
    }

    void onKeyPress(mpgl::KeyboardKey const& key) noexcept {
        switch (key) {
            case mpgl::KeyboardKey::C: {
                seeds.clear();
                outline = mpgl::LineStrip2D{};
                outlineShape.clear();
                drawer.clear();
                isDrawing = false;
                isClicked = false;
                points = mpgl::LineStrip2D{};
                pointsShape.clear();
                return;
            }
            case mpgl::KeyboardKey::Up: {
                ++drawers;
                texts.at("coeff") = "Liczba współczynników: " + std::to_string(drawers);
                return;
            }
            case mpgl::KeyboardKey::Down: {
                --drawers;
                texts.at("coeff") = "Liczba współczynników: " + std::to_string(drawers);
                return;
            }
            case mpgl::KeyboardKey::J: {
                outline = mpgl::LineStrip2D{};
                outlineShape.clear();
                return;
            }
            case mpgl::KeyboardKey::O: {
                drawers = 1;
                texts.at("coeff") = "Liczba współczynników: " + std::to_string(drawers);
                return;
            }
            case mpgl::KeyboardKey::N: {
                drawers = drawer.size();
                texts.at("coeff") = "Liczba współczynników: " + std::to_string(drawers);
                return;
            }
            default:
                return;
        }
    }

    void onTick(std::chrono::milliseconds const& delta) noexcept {
        using namespace mpgl;

        (void) delta;
        if (not isDrawing)
            return;

        auto const& [x, y] = drawer.draw({450, 300}, drawers);

        outlineShape.emplace_back(x, y);
        outline = mpgl::LineStrip2D{};
        for (auto const& point : outlineShape)
            outline.emplace(point, mpgl::colors::red);
    }

   private:
    bool isClicked = false;
    bool isDrawing = false;

    std::vector<mpgl::Vector2f> seeds;

    mpgl::LineStrip2D outline;
    std::vector<mpgl::Vector2f> outlineShape;

    mpgl::LineStrip2D points;
    std::vector<mpgl::Vector2f> pointsShape;

    std::map<std::string, mpgl::Text2D> texts;

    mpgl::Font font;

    mpgl::DrawableCollection<mpgl::Line2D> cross;

    size_t drawers = 1;
    FourierDrawer drawer;
};

int main(void) {
    mpgl::Window window{{900, 600}, "Płutno Fouriera"};

    window.emplaceDrawable<Canva>();

    window.windowLoop(mpgl::Color{0.2f, 0.3f, 0.3f, 1.f});
    return 0;
}
