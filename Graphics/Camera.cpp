#include "Camera.h"
#include "Input.h"


glm::mat4 Camera::GetViewMatrix()
{
    float thetaR = glm::radians(m_theta);
    float phir = glm::radians(m_phi);
    glm::vec3 forward(cos(phir) * cos(thetaR), sin(phir),
        cos(phir) * sin(thetaR));

    return glm::lookAt(m_position, m_position + forward, 
        glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h)
{
    //this can be calculated from the fov, screen width and hieght
    //for aspect ratio, near and far plane values.
    return glm::perspective(glm::pi<float>() * 0.25f,
        w / h,
        0.1f, 1000.0f);
}

void Camera::Update(float deltaTime)
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
    if (input->isKeyDown(aie::INPUT_KEY_Z))
        m_position += up * deltaTime;

    if (input->isKeyDown(aie::INPUT_KEY_X))
        m_position -= up * deltaTime;

    if (input->isKeyDown(aie::INPUT_KEY_D))
        m_position += right * deltaTime;

    if (input->isKeyDown(aie::INPUT_KEY_A))
        m_position -= right * deltaTime;

    if (input->isKeyDown(aie::INPUT_KEY_W))
        m_position += forward * deltaTime;

    if (input->isKeyDown(aie::INPUT_KEY_S))
        m_position -= forward * deltaTime;

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

void Camera::SetPerspective(float fov, float aspectRatio, float near, float far)
{
    m_projectionTransform = glm::perspective(glm::pi<float>() * fov, aspectRatio, near, far);
}

void Camera::SetLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
    m_viewTransform = glm::lookAt(from, to, up);
}

void Camera::UpdateProjectionViewTransform()
{
    m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}
