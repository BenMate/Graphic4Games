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
