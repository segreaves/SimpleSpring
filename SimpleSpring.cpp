#include <SFML/Graphics.hpp>
#include "CompressionSpring.h"
#include "TorqueSpring.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// function to calculate the angle between two vectors
float getAngle(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    // calculate dot product
    float dotProduct = v1.x * v2.x + v1.y * v2.y;

    // calculate magnitudes
    float mag1 = std::sqrt(v1.x * v1.x + v1.y * v1.y);
    float mag2 = std::sqrt(v2.x * v2.x + v2.y * v2.y);

    // calculate cosine of the angle
    float cosTheta = dotProduct / (mag1 * mag2);
    // clamp cosTheta to [-1, 1] to avoid domain errors with acos
    cosTheta = std::max(-1.0f, std::min(1.0f, cosTheta));
    // calculate and return the angle in radians
    return std::acos(cosTheta);
}

// function to calculate the signed angle from v1 to v2
float getSignedAngle(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    float angle = getAngle(v1, v2);
    // calculate the cross product's z-component to determine the sign
    float crossProduct = v1.x * v2.y - v1.y * v2.x;
    // if cross product is negative, the angle is clockwise
    angle = crossProduct < 0 ? -angle : angle;
    return angle;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Interactive Spring Simulation");
    window.setFramerateLimit(60);

    // compression spring
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    CompressionSpring spring(sf::Vector2f(mousePos.x, mousePos.y), 25.f, 25.f, 5.f);
    // torque spring
    TorqueSpring torqueSpring(10, 10);
    // offset angle of the torque spring
    sf::Vector2f preOffset = spring.getAnchorPoint() - spring.getEndPoint();

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        // get current mouse position
        mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosFloat(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        // update spring anchor to mouse position
        spring.setAnchorPoint(mousePosFloat);

        // offset angle of the torque spring
        const float angle = atan2(spring.getEndPoint().y - spring.getAnchorPoint().y, spring.getEndPoint().x - spring.getAnchorPoint().x);
        // get current offset
        const sf::Vector2f offset = spring.getAnchorPoint() - spring.getEndPoint();
        // get angle (in radians) between previous offset and current offset
        const float delta = getSignedAngle(preOffset, offset);
        // update torque spring angle
        torqueSpring.rotate(-delta);
        // update previous offset
        preOffset = offset;

        // update spring    
        spring.update(dt);
        torqueSpring.update(dt);

        // Clear the window
        window.clear(sf::Color::Black);

        // draw the spring anchor
        sf::CircleShape anchor(5);
        anchor.setFillColor(sf::Color::Red);
        anchor.setOrigin(anchor.getRadius(), anchor.getRadius());
        anchor.setPosition(spring.getAnchorPoint());
        window.draw(anchor);
        // draw end point
        sf::RectangleShape end(sf::Vector2f(35, 25));
        end.setFillColor(sf::Color::White);
        end.setOrigin(end.getSize().x / 2, end.getSize().y / 2);
        // set end position to end point + offset
		end.setPosition(spring.getEndPoint());
        // rotate offset by torque spring angle
		end.setPosition(spring.getEndPoint());
        // set end rotation to torque spring angle in degrees
        end.setRotation(90 + (angle + torqueSpring.getAngle()) * 180 / M_PI);
        window.draw(end);
        // draw attach point
        sf::CircleShape attach(2);
        attach.setFillColor(sf::Color::Red);
        attach.setOrigin(attach.getRadius(), attach.getRadius());
        attach.setPosition(spring.getEndPoint());
        window.draw(attach);
        // draw line
        sf::Vertex line[] = {
        sf::Vertex(spring.getAnchorPoint()),
        sf::Vertex(spring.getEndPoint())
        };
        window.draw(line, 2, sf::Lines);

        // display the contents of the window
        window.display();
    }
}
