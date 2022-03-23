#include "FlyCamera.h"

void FlyCamera::Update(float deltaTime)
{
    aie::Input* input = aie::Input::getInstance();
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);

    //calculate the forwards, right and up axis for the camera
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR),
        cos(phiR) * sin(thetaR));

    glm::vec3 right(-sin(thetaR), 0, cos(thetaR));

    glm::vec3 up(0, 1, 0);

    //use wasd, zx keys to move camera around
    if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
        m_position -= up * deltaTime * m_speed;

    if (input->isKeyDown(aie::INPUT_KEY_SPACE))
        m_position += up * deltaTime * m_speed;

    if (input->isKeyDown(aie::INPUT_KEY_D))
        m_position += right * deltaTime * m_speed;

    if (input->isKeyDown(aie::INPUT_KEY_A))
        m_position -= right * deltaTime * m_speed;

    if (input->isKeyDown(aie::INPUT_KEY_W))
        m_position += forward * deltaTime * m_speed;

    if (input->isKeyDown(aie::INPUT_KEY_S))
        m_position -= forward * deltaTime * m_speed;

    //get the current mouse coordinates
    float mx = input->getMouseX();
    float my = input->getMouseY();

    //if the right button is down, increment theta and phi
    if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
    {
        m_theta += m_turnspeed * (mx - m_lastMouseX);
        m_phi -= m_turnspeed * (my - m_lastMouseY);
    }
    //store this frames values for next frame
    m_lastMouseX = mx;
    m_lastMouseY = my;

    //clamping to prevent gimbal lock
    glm::clamp(m_phi, -70.0f, 70.0f);

}
