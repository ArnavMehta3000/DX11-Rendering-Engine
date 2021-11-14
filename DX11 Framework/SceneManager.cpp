#include "SceneManager.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
    if (instance == nullptr)
        instance = new SceneManager();

    return instance;
}

SceneManager::SceneManager()
{
    activeScene = nullptr;
}

void SceneManager::AddScene(Scene* scene)
{
    sceneList.push_back(scene);
}

int SceneManager::GetSceneCount()
{
    return sceneList.size();
}

bool SceneManager::SetScene(std::string sceneName)
{
    Scene* prevScene = activeScene;
    for (auto nextScene : sceneList)
    {
        if (nextScene->name == sceneName)  // If current scene is same as the next scene, then don't change
        {
            if (activeScene != nullptr && activeScene->Equals(nextScene))
            {
                OutputDebugString(L"Scene already loaded");
                return false;
            }

            if (prevScene != nullptr)
                prevScene->OnExit();

            activeScene = nextScene;
            activeScene->OnEnter();
            return true;

        }
    }
    OutputDebugString(L"Failed to find scene");
    return false;
}

void SceneManager::Update()
{
    if (activeScene == nullptr)
    {
        std::cout << "No active scene" << std::endl;
        exit(-1);
    }

    activeScene->Update();
}

void SceneManager::Render()
{
    activeScene->Render();
}
