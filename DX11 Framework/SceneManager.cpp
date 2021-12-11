#include "SceneManager.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
    if (instance == nullptr)
        instance = new SceneManager();

    return instance;
}

void SceneManager::UpdateScene()
{
    if (m_ActiveScene == nullptr)
        OutputDebugString(L"ACTIVE SCENE IS NULL\n");
    else
        m_ActiveScene->Update();
}

void SceneManager::RenderScene(ConstantBuffer* cb)
{
    if (m_ActiveScene == nullptr)
        OutputDebugString(L"ACTIVE SCENE IS NULL\n");
    else
        m_ActiveScene->Render(cb);
}

void SceneManager::AddScene(Scene* scene)
{
    m_SceneList.push_back(scene);
}

bool SceneManager::SetScene(std::string sceneName)
{
    Scene* prevScene = m_ActiveScene;  // Assuming scene changes, the previous scene is the current scene
    for (auto nextScene : m_SceneList)  // Cycle through all scens
    {
        if (nextScene->m_SceneName == sceneName)
        {
            // If previous scene is not nullptr (there is a current active scene
            if (prevScene != nullptr)  // Call the on exit function of that scene
                prevScene->Exit();

            // Set currently active scene to the next scene
            m_ActiveScene = nextScene;
            m_ActiveScene->Enter();
            
            return true;
        }
    }

    // Scene not found 
    return false;
}

CameraMats SceneManager::GetCamera()
{
    if (m_ActiveScene == nullptr)
        OutputDebugString(L"ACTIVE SCENE IS NULL\n");
    else
        return m_ActiveScene->GetActiveCamera();
}

SceneManager::SceneManager()
{
    m_ActiveScene = nullptr;
}
