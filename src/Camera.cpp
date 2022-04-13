#include "Camera.h"

void Camera::Update()
{
    //TODO update based on input from mouse+keyboard
    vec3 target = normalize(vec3(
        cos(xRot) * cos(yRot),
        sin(yRot),
        sin(xRot) * cos(yRot)));
    vec3 up = vec3(0, 1, 0);
    mat4 lookAt = glm::lookAt(pos, pos + target, up); //first person camera. "looks at" the direction of target from the starting point of pos.
    view = lookAt; //something can watch view and do something based on that value.
}
//not sure how to handle update based on outside parameters yet. Might refactor with event manager.
void Camera::Update(double posX, double posY)
{
    if (firstMouseMovement) { //avoids first-frame jerky camera movement
        mousePrevX = posX;
        mousePrevY = posY;
        firstMouseMovement = false;
    }
    deltaMouseX = posX - mousePrevX;
    deltaMouseY = mousePrevY - posY; //y is inverted
    //adjust movement based on sensitivity
    xRot += sensitivityX * deltaMouseX;
    yRot += sensitivityY * deltaMouseY;
    //cap yRot to avoid gimbal lock.
    constexpr float cap = glm::radians<float>(80.0f); //in degrees
    if (yRot > cap) yRot = cap;
    if (yRot < -cap) yRot = -cap;
    //set the previous values
    mousePrevX = posX;
    mousePrevY = posY;
    //update gaze and cameraRight vector w and u using xRot and yRot.
    w = -normalize(vec3(
        cos(xRot) * cos(yRot),
        sin(yRot),
        sin(xRot) * cos(yRot)));
    u = cross(w, vec3(0, 1, 0)); //right is forward cross up.
}


