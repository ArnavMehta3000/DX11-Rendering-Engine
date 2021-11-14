#pragma once
#ifndef SCENE_H
#define SCENE_H
#include <string>
#include "Core.h"


class Scene
{
public:
    Scene(std::string name) : name(name) {}

    virtual void OnEnter() = 0;
    virtual void Init()    = 0;
    virtual void Update()  = 0;
    virtual void Render()  = 0;
    virtual void OnExit()  = 0;

    bool Equals(Scene* other)
    {
        if (!name.empty())
            return (other->name == name);

        exit(-1);
    }

    std::string name;

protected:
    bool m_initialised = false;
};
#endif // !SCENE_H