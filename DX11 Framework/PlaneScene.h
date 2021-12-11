#pragma once
#ifndef PLANESCENE_H
#define PLANESCENE_H
#include "Scene.h"
#include "GameObject.h"

class PlaneScene : public Scene
{
public:
	PlaneScene(std::string name);
	~PlaneScene();

	virtual void Init(SceneInitData data) override;
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Render(ConstantBuffer* cb) override;
	virtual void Exit() override;
	virtual CameraMats GetActiveCamera() override;

private:
	GameObject* m_HerculesPlane;
};
#endif // !PLANESCENE_H