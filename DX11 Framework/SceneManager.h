#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <vector>
#include "Scene.h"

class SceneManager
{
public:
    static SceneManager* GetInstance();

    void Update();
    void Render();

    void AddScene(Scene* scene = nullptr);
    bool SetScene(std::string sceneName = "");
    int GetSceneCount();

private:
    SceneManager();

    Scene* activeScene;
    std::vector<Scene*> sceneList;
    static SceneManager* instance;
};
#endif SCENEMANAGER_H