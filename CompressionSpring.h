#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

/// <summary>
/// This class simulates a compression spring. Accounts for position only, not rotation.
/// </summary>
class CompressionSpring
{
public:
    CompressionSpring(const sf::Vector2f& anchor, float length, float stiffness, float damping)
        : m_anchorPoint(anchor), m_restLength(length), m_stiffness(stiffness), m_damping(damping) {
        m_endPoint = m_anchorPoint + sf::Vector2f(0, length);
        m_velocity = sf::Vector2f(0, 0);
    }

    void update(float dt) {
        sf::Vector2f displacement = m_endPoint - m_anchorPoint;
        float currentLength = std::sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
        sf::Vector2f direction = displacement / currentLength;

        float stretch = currentLength - m_restLength;
        sf::Vector2f force = -m_stiffness * stretch * direction;

        // apply damping force
        force += -m_damping * m_velocity;

        // update velocity and position
        m_velocity += force * dt;
        m_endPoint += m_velocity * dt;
    }

    void setAnchorPoint(const sf::Vector2f& point) {
		m_anchorPoint = point;
	}

	sf::Vector2f getAnchorPoint() const {
		return m_anchorPoint;
	}

    sf::Vector2f getEndPoint() const {
		return m_endPoint;
	}
private:
    sf::Vector2f m_anchorPoint;
    sf::Vector2f m_endPoint;
    float m_restLength;
    float m_stiffness;
    float m_damping;
    sf::Vector2f m_velocity;
};