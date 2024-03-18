#include "Model3D.h"

Model3D::Model3D()
{
}

Model3D::~Model3D()
{
	if (IsChild)
	{
		//Children.clear();
		IsChild = false;
	}

	if (IsParent)
	{
		//Parents.clear();
		IsParent = false;
	}
}

bool Model3D::Initialize(Utilities* utilities)
{
	Entity::Initialize(utilities);

	ViewableArea = { WindowWidth, WindowHeight };

	return false;
}

void Model3D::LoadModel(Model &model, Texture2D &texture)
{
	if (IsTextureReady(texture))
	{
		TheModel = model;
		TheModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	}
}

bool Model3D::BeginRun()
{
	Entity::BeginRun();

	return false;
}

bool Model3D::SetCamera(Camera* camera)
{
	TheCamera = camera;

	return false;
}

void Model3D::Input()
{
}

void Model3D::Update(float deltaTime)
{
	Entity::Update(deltaTime);
}

void Model3D::Draw()
{
	if (Enabled)
	{
		Entity::Draw();

		if (TheCamera == nullptr)
		{
			TraceLog(LOG_INFO, "***************\n");
			TraceLog(LOG_ERROR,
			"*****************  Camera Reference is missing! ******************\n");

			return;
		}

		if (Cull)
		{
			if (IsChild)
			{
				Vector3 parentTest = Position;
				float radius = 0;

				for (auto parent : Parents)
				{
					parentTest = Vector3Add(parent->Position, parentTest);
					radius += parent->Radius;
				}

				if (TheCamera->position.x > parentTest.x + radius + Radius + ViewableArea.x
					|| TheCamera->position.x < parentTest.x + -radius + -Radius + -ViewableArea.x)
				{
					WasCulled = true;
					return;
				}

				if (TheCamera->position.y > parentTest.y + radius + Radius + ViewableArea.y ||
					TheCamera->position.y < parentTest.y + -radius + -Radius + -ViewableArea.y)
				{
					WasCulled = true;
					return;
				}
			}
			else
			{
				if (TheCamera->position.x > Position.x + Radius + ViewableArea.x
					|| TheCamera->position.x < Position.x + -Radius + -ViewableArea.x)
				{
					WasCulled = true;
					return;
				}

				if (TheCamera->position.y > Position.y + Radius + ViewableArea.y ||
					TheCamera->position.y < Position.y + -Radius + -ViewableArea.y)
				{
					WasCulled = true;
					return;
				}
			}
		}

		WasCulled = false;
		BeforeCalculate();

		CalculateWorldVectors();

		DrawModel(TheModel, ModelPosition, ModelScale, ModelColor);	// Draw 3D model

		AfterCalculate();
	}
}

//void Model3D::AddChild(Model3D* child)
//{
//	for (auto parent : Parents)
//	{
//		parent->AddChildren(child);
//	}
//
//	Children.push_back(child);
//	child->Parents.push_back(this);
//	child->IsChild = true;
//	IsParent = true;
//
//}

void Model3D::SetModel(Model &model, float scale)
{
	TheModel = model;
	ModelScale = scale;
}

void Model3D::SetModel(Model& model)
{
	SetModel(model, 1.0f);
}

void Model3D::SetModelCopy(Model model, float scale)
{
	TheModel = model;
	ModelScale = scale;
}

Model& Model3D::GetModel()
{
	return TheModel;
}

Camera* Model3D::GetCamera()
{
	return TheCamera;
}

void Model3D::Spawn(Vector3 pos, Vector3 vel)
{
}

void Model3D::Unload()
{
	UnloadModel(TheModel);
}

void Model3D::AddChildren(Model3D* child)
{
	child->Parents.push_back(this);
	IsParent = true;
	child->IsChild = true;
}
