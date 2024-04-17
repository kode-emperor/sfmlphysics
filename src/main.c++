// Libraries
#include <SFML/Graphics.hpp>
#include <boost/program_options.hpp>
#include <filesystem>
#include <format>
#include <iostream>
#include <reactphysics3d/reactphysics3d.h>
#include <sstream>
#include <thread>

using namespace std::chrono_literals;

#define ARGV_PROJECT_ROOT 1
// ReactPhysics3D namespace
using namespace reactphysics3d;
namespace fs = std::filesystem;
// Main function

void parseProgramOptions(int argc, char **argv) {}
int main(int argc, char **argv) {
  const auto projectRootDir = fs::path(argv[ARGV_PROJECT_ROOT]);
  const auto resFolder = fs::path(projectRootDir.string() + "/src/res");
  const auto fileRobotoRegularTtf =
      fs::path(resFolder.string() + "/fonts/roboto/Roboto-Regular.ttf");
  // First you need to create the PhysicsCommon object.
  // This is a factory module that you can use to create physics
  // world and other objects. It is also responsible for
  // logging and memory management
  PhysicsCommon physicsCommon;

  // Create a physics world
  PhysicsWorld *world = physicsCommon.createPhysicsWorld();

  // Create a rigid body in the world
  Vector3 position(0, 20, 0);
  Quaternion orientation = Quaternion::identity();
  Transform transform(position, orientation);
  RigidBody *body = world->createRigidBody(transform);

  const decimal timeStep = 1.0f / 60.0f;

  // Step the simulation a few steps
  auto window = sf::RenderWindow{{1920u, 1080u}, "SFML-ReactPhysics"};

  sf::Font robotoFont;
  if (!robotoFont.loadFromFile(fileRobotoRegularTtf.string())) {
    std::cerr << "Error opening font file " << fileRobotoRegularTtf
              << std::endl;
    return -1;
  }
  sf::Text text;
  text.setFont(robotoFont);
  text.setString("Hello there");
  text.setFillColor(sf::Color::Green);
  text.setCharacterSize(30);

  // a circle to apply the physics to
  sf::CircleShape circle(50.f);
  circle.setFillColor(sf::Color::Blue);

  auto cx = 10.f;
  auto cy = 10.f;
  auto circleRadius = 50.f;
  circle.setPosition(sf::Vector2f{cx, cy});

  auto dx = 2.f;
  auto dy = -2.f;
  auto width = 1920.f;
  auto height = 1080.f;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    world->update(timeStep);
    if (cx + dx > width - circleRadius || cx + dx < circleRadius) {
      dx = -dx;
    }

    if (cy + dy > height - circleRadius || cy + dy < circleRadius) {
      dy = -dy;
    }
    cx += dx;
    cy += dy;

    // Get the updated position of the body
    const Transform &transform = body->getTransform();
    const Vector3 &position = transform.getPosition();

    // Display the position of the body
    std::stringstream ss;
    ss << "(" << position.x << ", " << position.y << ", " << position.z << ")";
    circle.setPosition(sf::Vector2f{cx, cy});
    // sf::sleep(sf::Time(std::chrono::microseconds(1s).count()));
    text.setString(ss.str());
    window.clear();
    window.draw(circle);
    window.draw(text);
    std::cout << "Body Position: " << ss.str() << std::endl;
    window.display();
  }

  std::cout << std::format("The current working directory is: {}\n",
                           fs::current_path().c_str());

  for (auto dir_entry : fs::recursive_directory_iterator{resFolder}) {
    std::cout << dir_entry << std::endl;
  }
  return 0;
}