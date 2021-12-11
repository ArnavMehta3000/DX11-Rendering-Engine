#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <vector>
#include "Scene.h"


class SceneManager
{
public:
    static SceneManager* GetInstance();

    void UpdateScene();
    void RenderScene(ConstantBuffer* cb);

    void AddScene(Scene* scene = nullptr);
    bool SetScene(std::string sceneName = "");

    CameraMats GetCamera();

private:
    SceneManager();

    Scene* m_ActiveScene;
    std::vector<Scene*> m_SceneList;
    static SceneManager* instance;
};
#endif // SCENEMANAGER_H