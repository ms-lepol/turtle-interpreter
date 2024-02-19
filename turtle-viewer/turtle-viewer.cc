#include <cstdlib>
#include <cstring>

#include <iostream>
#include <string>

#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Curves.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Vector.h>
#include <gf/VectorOps.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

enum Command {
  Color,
  MoveTo,
  LineTo,
};

static constexpr const char *ColorKw = "Color";
static constexpr const char *MoveToKw = "MoveTo";
static constexpr const char *LineToKw = "LineTo";

int main() {
  std::vector<Command> commands;
  std::vector<gf::Vector2f> points;
  std::vector<gf::Color4f> colors;

  std::size_t movements = 0;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.find(ColorKw) != std::string::npos) {
      gf::Color4f color;

      char *endptr = &line[0] + std::strlen(ColorKw);
      color.r = std::strtod(endptr, &endptr);
      color.g = std::strtod(endptr, &endptr);
      color.b = std::strtod(endptr, &endptr);
      color.a = 1.0f;

      commands.push_back(Command::Color);
      colors.push_back(color);
    }

    if (line.find(MoveToKw) != std::string::npos) {
      gf::Vector2f point;

      char *endptr = &line[0] + std::strlen(MoveToKw);
      point.x = std::strtod(endptr, &endptr);
      point.y = std::strtod(endptr, &endptr);

      commands.push_back(Command::MoveTo);
      points.push_back(point);

      ++movements;
    }

    if (line.find(LineToKw) != std::string::npos) {
      gf::Vector2f point;

      char *endptr = &line[0] + std::strlen(LineToKw);
      point.x = std::strtod(endptr, &endptr);
      point.y = std::strtod(endptr, &endptr);

      commands.push_back(Command::LineTo);
      points.push_back(point);

      ++movements;
    }
  }

  static constexpr gf::Vector2u ScreenSize(1024, 576);
  static constexpr gf::Vector2f ViewSize(1000.0f, 1000.0f);
  static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f);

  // initialization

  gf::Window window("Turtle Viewer", ScreenSize);
  gf::RenderWindow renderer(window);

  // views

  gf::ViewContainer views;

  gf::ExtendView mainView(ViewCenter, ViewSize);
  views.addView(mainView);

  views.setInitialFramebufferSize(ScreenSize);

  // actions

  gf::ActionContainer actions;

  gf::Action closeWindowAction("Close window");
  closeWindowAction.addCloseControl();
  closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
  actions.addAction(closeWindowAction);

  gf::Action fullscreenAction("Fullscreen");
  fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  actions.addAction(fullscreenAction);

  gf::Action finishAction("Finish");
  finishAction.addKeycodeKeyControl(gf::Keycode::Space);
  actions.addAction(finishAction);

  gf::Action forwardAction("Forward");
  forwardAction.addKeycodeKeyControl(gf::Keycode::Right);
  actions.addAction(forwardAction);

  gf::Action backwardAction("Backward");
  backwardAction.addKeycodeKeyControl(gf::Keycode::Left);
  actions.addAction(backwardAction);


  // game loop

  renderer.clear(gf::Color::White);
  gf::Clock clock;

  static constexpr float Duration = 10.0f;
  static constexpr float Jump = 1.0f; // for forward and backward
  float elapsed = 0;

  while (window.isOpen()) {
    // 1. input

    gf::Event event;

    while (window.pollEvent(event)) {
      actions.processEvent(event);
      views.processEvent(event);
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

    if (finishAction.isActive()) {
      elapsed = Duration;
    }

    if (forwardAction.isActive()) {
      elapsed += Jump;
    }

    if (backwardAction.isActive()) {
      elapsed -= Jump;
    }

    // 2. update

    float dt = clock.restart().asSeconds();
    elapsed += dt;

    elapsed = gf::clamp(elapsed, 0.0f, Duration);

    // 3. draw

    renderer.clear();
    renderer.setView(mainView);

    if (!commands.empty()) {
      float steps = elapsed / Duration * movements;
      std::size_t maxStep = std::floor(steps);
      float inStep = std::fmod(steps, 1.0f);

      gf::Vector2f currPoint(0, 0);
      gf::Color4f currColor = gf::Color::Black;

      auto itPoint = points.begin();
      auto itColor = colors.begin();

      std::size_t currStep = 0;

      for (auto cmd : commands) {
        if (currStep > maxStep) {
          break;
        }

        switch (cmd) {
          case Command::Color:
            currColor = *itColor++;
            break;
          case Command::MoveTo:
            currPoint = *itPoint++;
            break;
          case Command::LineTo: {
            gf::Vector2f nextPoint = *itPoint++;

            if (currStep == maxStep) {
              nextPoint = gf::lerp(currPoint, nextPoint, inStep);
            }

            gf::Line line(currPoint, nextPoint);
            line.setColor(currColor);
            line.setWidth(3.0f);
            renderer.draw(line);

            currPoint = nextPoint;
            break;
          }
        }


        if (cmd != Command::Color) {
          ++currStep;
        }
      }

      gf::CircleShape turtle(5.0f);
      turtle.setPosition(currPoint);
      turtle.setColor(gf::Color::Chartreuse);
      turtle.setAnchor(gf::Anchor::Center);
      renderer.draw(turtle);
    }


    renderer.display();

    actions.reset();
  }

  return 0;
}
