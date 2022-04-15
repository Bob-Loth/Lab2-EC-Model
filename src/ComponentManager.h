#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Component.h"
#include "Movement.h"
#include "Collect.h"
#include "Transform.h"
#include "Collision.h"
#include "TextureRenderer.h"

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

class ComponentManager
{
    //a queue that is automatically sorted, with smaller elements at the top.
    //using typedef to localize this long definition.
    typedef priority_queue<size_t, vector<size_t>, std::greater<size_t>> OpenSlots;
public:
    void DeleteObject(GameObject obj);
    GameObject GetObject(string name);
    //anything the component manager needs to do BEFORE frame-by-frame operations
    void Init(std::string resourceDirectory);
    
    //update components
    void UpdateComponents(float frameTime, int width, int height);
    void AddGameObject(string name, vector<shared_ptr<Component>> comps);
    void RemoveGameObject(string name);
    //anything the component manager needs to do AFTER frame-by-frame operations
    void Cleanup();
    shared_ptr<Component> GetComponent(string, int);
    Camera& GetCamera() { return camera; } //direct access, camera isn't componentized yet.
private:
    //the objects
    map<string, GameObject> objects;
    
    //helper functions to differentiate parts of AddGameObject.
    pair<string, size_t> addToComponentList(const shared_ptr<Component>& comp);
    int getNextOpenSlot(OpenSlots& slots);
    template<typename T>
    void addHelper(shared_ptr<T> comp, vector<shared_ptr<T>>& compList, int& index);

    //the various components
    //one camera for now
    Camera& camera = Camera::GetInstance(vec3(0,1,0));

    vector<string> componentVectorNames{ "Movement", "Transform", "Collision", "Renderer", "Collect" }; //MTRC
    //Movement
    vector< shared_ptr<Movement>> movements; //TODO change type to movement component name
    OpenSlots movementSlots;
    //Transform
    vector< shared_ptr<Transform>> transforms; //TODO change type to transforms component name
    OpenSlots transformSlots;
    //Renderer
    vector<shared_ptr<Renderer>> renderers; //TODO change type to Renderer
    OpenSlots rendererSlots;
    //Collision
    vector<shared_ptr<Collision>> collisions; //TODO change type to collision, or bounding sphere/box.
    OpenSlots collisionSlots;
    //Collect
    vector<shared_ptr<Collect>> collects; //TODO change type to collision, or bounding sphere/box.
    OpenSlots collectSlots;
    
    //Spawner
    //TODO write singleton for spawner.
};

