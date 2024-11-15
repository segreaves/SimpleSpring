#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

/// <summary>
/// This class simulates a torque spring. Accounts for rotation only, not position.
/// </summary>
class TorqueSpring
{
public:
    TorqueSpring(float springStiffness, float springDamping) :
        m_currentAngle(0),
        m_stiffness(springStiffness),
        m_damping(springDamping),
        m_angularVelocity(0)
    {
    }

    void update(float dt)
    {
        float torque = -m_stiffness * m_currentAngle;

        // apply damping torque
        torque += -m_damping * m_angularVelocity;

        // update angular velocity and angle
        m_angularVelocity += torque * dt;
        rotate(m_angularVelocity * dt);
    }

    void setAngle(float angle)
    {
        m_currentAngle = angle;
    }
    float getAngle() const { return m_currentAngle; }

    void rotate(float angle) { m_currentAngle += angle; }
private:
    float m_currentAngle;
    float m_stiffness;
    float m_damping;
    float m_angularVelocity;
};