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
          text{font, mpgl::Vector2f{100, 100}, "Liczba współczynników: 1",
               mpgl::TextOptions{.size = 16, .color=mpgl::colors::white}},
          drawer{5} {}

    void draw(void) const noexcept {
        points.draw();
        outline.draw();
        drawer.draw();
        text.draw();
        // lines.draw();
    }

    void transform(mpgl::Transformation2D const& transformator) noexcept {
        points.transform(transformator);
        outline.transform(transformator);
        drawer.transform(transformator);
        text.transform(transformator);
        //lines.transform(transformator);
    }

    void onMouseMotion(mpgl::Vector2f const& position) noexcept {
        static constexpr mpgl::Vector2f center{450, 300};
        if (isClicked) {
            points.emplace(position, mpgl::colors::white);

            auto [x, y] = position - center;
            seeds.emplace_back(x, y);
        }
    }

    void onMousePress(mpgl::MouseButton const& button) noexcept {
        if (button == mpgl::MouseButton::Right) isClicked = true;
    }

    void onMouseRelease(mpgl::MouseButton const& button) noexcept {
        if (button == mpgl::MouseButton::Right) {
            isClicked = false;

            drawer.setPoints(fourier(seeds));

            isDrawing = true;
            // points = mpgl::Points2D{};
        }
    }

    void onKeyPress(mpgl::KeyboardKey const& key) noexcept {
        if (key == mpgl::KeyboardKey::C) {
            seeds.clear();

            outline.clear();
            drawer.clear();
            isDrawing = false;
            isClicked = false;
            points = mpgl::Points2D{};
        } else if (key == mpgl::KeyboardKey::V) {
            ++drawers;
            text = "Liczba współczynników: " + std::to_string(drawers);
        } else if (key == mpgl::KeyboardKey::B) {
            --drawers;
            text = "Liczba współczynników: " + std::to_string(drawers);
        } else if (key == mpgl::KeyboardKey::J) {
            outline.clear();
        }
    }

    void onTick(std::chrono::milliseconds const& delta) noexcept {
        using namespace mpgl;

        (void) delta;
        if (not isDrawing)
            return;

        auto const& [x, y] = drawer.draw({450, 300}, drawers);

        outline.emplace_back(mpgl::colors::red, mpgl::Vector2f{x, y});

        // lines.clear();

        // double lastX = 450, lastY = 300;

        // double freq = elapsedTime.count() / 5'000.;

        // for (size_t i = 0; i < 12; ++i) {
        //     auto const& [radius, phase] = driver[i];
        //     // freq * time = i * time
        //     double const phi = phase + freq * i;

        //     double const x = lastX + radius * std::cos(phi);
        //     double const y = lastY + radius * std::sin(phi);

        //     lines.emplace_back(mpgl::Vector2f{lastX, lastY}, mpgl::Vector2f{x, y});
        //     lastX = x;
        //     lastY = y;
        // }

        // outline.emplace_back(mpgl::colors::white, mpgl::Vector2f{lastX, lastY});

        // elapsedTime += delta;
    }

   private:
    bool isClicked = false;
    bool isDrawing = false;

    std::chrono::milliseconds elapsedTime = 0ms;

    std::vector<mpgl::Vector2f> seeds;

    mpgl::DrawableCollection<mpgl::Points2D> outline;

    mpgl::Points2D points;

    mpgl::Font font;
    mpgl::Text2D text;


    size_t drawers = 1;

    std::chrono::milliseconds const Period = 5s;

    FourierDrawer drawer;
};

int main(void) {
    mpgl::Window window{{900, 600}, "fourier"};

    window.emplaceDrawable<Canva>();

    window.windowLoop();
    return 0;
}
