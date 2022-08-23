#include <Application.h>
#include <Main.h>
#include <stdlib.h>  
#include <crtdbg.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Serializer.h>
#include "imgui.h"
class RunTimeApplication : public Armageddon::Application
{
public:
	RunTimeApplication()
	{
		Armageddon::Application::GetWindow()->SetMouseCallBack(std::bind(&RunTimeApplication::onMouseEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		Armageddon::Application::GetWindow()->setKeyBoardCallBack(std::bind(&RunTimeApplication::onKeyBoardEvent, this, std::placeholders::_1));

	};
	~RunTimeApplication()
	{

	};
	void OnUpdate() override;
	void OnRender() override;
	void ImGuiRender() override;
	void OnInit() override;
	void onMouseEvent(MouseEvent::MEventType e, float x, float y);
	void onKeyBoardEvent(const unsigned char keyCode);
private:
	Scene m_Scene;
	Serializer m_serializer = { &m_Scene };
};

Armageddon::Application* Armageddon::CreateApplication()
{
	return new RunTimeApplication();
}
static float cameraSpeed = 0.4f;

void RunTimeApplication::OnUpdate()
{
	m_Scene.UpdateScene();
	if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_S))
	{
		//AG_GET_RENDERER()m_camera.AdjustPosition(AG_GET_RENDERER()m_camera.GetUpVec() * cameraSpeed);
		AG_GET_RENDERER().m_camera.AdjustPosition(AG_GET_RENDERER().m_camera.GetBackwardVec() * cameraSpeed);

	}
	if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_Space))
	{
		//AG_GET_RENDERER()m_camera.AdjustPosition(AG_GET_RENDERER()m_camera.GetUpVec() * cameraSpeed);
		AG_GET_RENDERER().m_camera.AdjustPosition(AG_GET_RENDERER().m_camera.GetUpVec() * cameraSpeed);

	}

	if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_Q))
	{
		AG_GET_RENDERER().m_camera.AdjustPosition(AG_GET_RENDERER().m_camera.GetLeftVec() * cameraSpeed);

	}
	if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_Z))
	{
		AG_GET_RENDERER().m_camera.AdjustPosition(AG_GET_RENDERER().m_camera.GetForwardVec() * cameraSpeed);
	}
	if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_D))
	{
		AG_GET_RENDERER().m_camera.AdjustPosition(AG_GET_RENDERER().m_camera.GetRightVec() * cameraSpeed);
	}
	if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_A))
	{
		AG_GET_RENDERER().m_camera.AdjustRotation(0.0, 0.1 * 0.4f, 0);
	}
}

void RunTimeApplication::OnRender()
{
	m_Scene.RenderScene();
}

void RunTimeApplication::ImGuiRender()
{

}

void RunTimeApplication::OnInit()
{
	ImGui::SetCurrentContext(AG_GET_RENDERER().GetImGuiContext());
	m_serializer.DeserializeScene("Assets/Scenes/TestScene.mat");
	m_Scene.InitScene(); //TODO MOVE IN LOAD SCENE !!!!
	m_Scene.m_SceneState = SceneState::Runtime;
	//TODO IN SCENE 
	AG_GET_RENDERER().m_camera.SetPosition(0.0f, 0.0f, -1.0f);
	AG_GET_RENDERER().m_camera.SetProjectionValues(90.0f, Armageddon::Application::GetWindow()->GetAspectRatio(), 0.1f, 100.0f);
	AG_GET_RENDERER().displayScene = true;
	
	//SHOULD BE CALL AFTER THE SCENE DESERIALIZATION !
	m_Scene.OnRuntimeStart();
}
static float CameraZoom = 1.0f;

void RunTimeApplication::onMouseEvent(MouseEvent::MEventType e, float x, float y)
{
	switch (e)
	{
	case  MouseEvent::MEventType::Wheeldown:
		CameraZoom -= 0.1f;
		Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustPosition(Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetBackwardVec() * cameraSpeed);

		break;
	case  MouseEvent::MEventType::Wheelup:
		CameraZoom += 0.1f;
		Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustPosition(Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetForwardVec() * cameraSpeed);


		break;

	case MouseEvent::MEventType::Move:

		Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustRotation(y * -0.01f, x * 0.01f, 0.0f);
		break;
	}
}

//TODO LA CAMERA PUTAIN CA DOIT PAS ETRE ICI
void RunTimeApplication::onKeyBoardEvent(const unsigned char keyCode)
{
}

//TODO PUTAIN ALEXIS TU ES CON OUBLIE PAS LES CALL BACK ! 
//TODO RIEN NE MARCHE QUAND IL EST PAS DANS LE BON WORKING DIRECTORY !!!!