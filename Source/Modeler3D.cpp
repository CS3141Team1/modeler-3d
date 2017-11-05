#include "Modeler3D.h"

#include <cmath>
#include <iostream>

#include <boost/filesystem.hpp>
#include <GL/glew.h>

#include "GUI/AllWidgets.h"
#include "GUI/IAction.h"
#include "Math/VectorMath.h"

#include "FileIO.h"
#include "GuiRenderer.h"

using namespace std;
using namespace Core;
using namespace Core::Math;
using namespace Video;

std::string VertSource = ""
        "#version 120 \n"
        ""
        "attribute vec3 aPosition; \n"
        "attribute vec3 aNormal; \n"
        ""
        "varying vec3 vViewPosition; \n"
        "varying vec3 vNormal; \n"
        ""
        "uniform mat4 Projection; \n"
        "uniform mat4 View; \n"
        "uniform mat4 Model; \n"
        "uniform mat3 NormalMat; \n"
        ""
        "void main() \n"
        "{ \n"
        "   vNormal = normalize(NormalMat * aNormal); \n"
        "   gl_Position = View * Model * vec4(aPosition, 1.0); \n"
        "   vViewPosition = gl_Position.xyz / gl_Position.w; \n "
        "   gl_Position = Projection * gl_Position; \n"
        "} \n";

std::string FragSource = ""
        "#version 120 \n"
        ""
        "varying vec3 vViewPosition; \n"
        "varying vec3 vNormal; \n"
        ""
        "uniform vec3 LightDirection = vec3(-1, -0.5, -1); \n"
        ""
        "float Diffuse(vec3 normal, vec3 lightDir) \n"
        "{ \n"
        "   return clamp(dot(normal, -lightDir), 0.0, 1.0); \n"
        "} \n"
        ""
        "float Specular(vec3 normal, vec3 lightDir, vec3 cameraDir, float power) \n"
        "{ \n"
        "   vec3 halfVec = normalize(lightDir + cameraDir); \n"
        "   return pow(clamp(abs(dot(normal, -halfVec)), 0.0, 1.0), power); "
        "} \n"
        ""
        "void main() \n"
        "{ \n"
        "   vec3 normal = normalize(vNormal); \n"
        "   vec3 lightDir = normalize(LightDirection); \n"
        "   vec3 cameraDir = normalize(vViewPosition); \n"
        ""
        "   vec3 color = vec3(1.0); \n"
        "   float diffuse = Diffuse(normal, lightDir); \n"
        "   float specular = Specular(normal, lightDir, cameraDir, 100); \n"
        ""
        "   gl_FragColor = vec4(color * (diffuse * 0.4 + 0.4 + specular * 0.4), 1.0); \n"
        "} \n";

class LoadAction : public Gui::IAction
{
public:
    LoadAction(Modeler3D* modeler, std::string file) : mModeler(modeler), mFile(file) {}
    ~LoadAction() {}

    void OnActionPerformed(Gui::Widget* widget)
    {
        cout << "Loading file: " << mFile << endl;
        mModeler->LoadObj(mFile);
    }
private:
    Modeler3D* mModeler;
    std::string mFile;
};

class ZoomAction : public Gui::IAction
{
public:
	ZoomAction(Modeler3D* modeler, int32 zoom) : mModeler(modeler), mZoom(zoom) {}
    ~ZoomAction() {}

    void OnActionPerformed(Gui::Widget* widget)
    {
        cout << "Zoom set to: " << mZoom << endl;
        mModeler->SetZoom(mZoom);
    }
private:
    Modeler3D* mModeler;
    int32 mZoom;
};

namespace Core
{

Video::VertexFormat vboFormat = Video::VertexFormat()
        .AddElement(Video::Attribute::Position, 3)
        .AddElement(Video::Attribute::Normal, 3);

struct VertexPosition3Normal3
{
    Vector3f Position;
    Vector3f Normal;
};

Modeler3D::Modeler3D(IBackend* backend)
    : Application(backend),
      mEnv(nullptr),
      mGuiRenderer(nullptr),
      mShader(nullptr),
      mGeometry(nullptr),
      mVbo(nullptr),
      mAngle(0),
	  mMouse(backend->GetWindow()->GetMouse()),
	  mZoom(2)
{
}

Modeler3D::~Modeler3D()
{
}

void Modeler3D::LoadObj(const string& file)
{
    if (mVbo)
    {
        mGeometry->SetVertexBuffer(nullptr);
        mVbo->Release();
        delete mVbo;
    }

    boost::filesystem::path obj(file);
    FileIO objFile;
    objFile.LoadObj(obj);

    vector<VertexPosition3Normal3> vertices;
    vector<vector<double>> positions = objFile.GetGeometricVertices();
    vector<vector<vector<int>>> faces = objFile.GetFaceElements();

    for (uint i = 0; i < faces.size(); i++)
    {
        VertexPosition3Normal3 verts[3];
        for (uint j = 0; j < 3; j++)
        {
            vector<double> pos = positions[faces[i][0][j] - 1];
            for (uint k = 0; k < 3; k++)
            {
                verts[j].Position[k] = pos[k] * 10;
            }
        }
        Vector3f normal = Cross(Normalize( verts[1].Position -  verts[0].Position), Normalize( verts[2].Position -  verts[0].Position));
        verts[0].Normal = normal;
        verts[1].Normal = normal;
        verts[2].Normal = normal;

        vertices.push_back(verts[0]);
        vertices.push_back(verts[1]);
        vertices.push_back(verts[2]);
    }

    mVbo = Graphics->CreateVertexBuffer(vboFormat, vertices.size(), Video::BufferHint::Static);
    mVbo->SetData((float32*)(&vertices[0]), 0, vertices.size());
    mGeometry->SetVertexBuffer(mVbo);
}

void Modeler3D::OnInit()
{
    cout << "Initializing Modeler3D" << endl;

    mGeometry = Graphics->CreateGeometry();

    mEnv = Backend->GetWindow()->GetEnvironment();
    mGuiRenderer = new GuiRenderer(Graphics);
    mShader = Graphics->CreateShader(VertSource, FragSource);

    Gui::Button* elem1 = new Gui::Button(10, 10 + 50 * 0, 80, 40, new LoadAction(this, "Assets/bunny.obj"));
    Gui::Button* elem2 = new Gui::Button(10, 10 + 50 * 1, 80, 40, new LoadAction(this, "Assets/cube.obj"));
    Gui::Button* elem3 = new Gui::Button(10, 10 + 50 * 2, 80, 40, new LoadAction(this, "Assets/dragon.obj"));
    Gui::Button* elem4 = new Gui::Button(10, 10 + 50 * 3, 80, 40, new LoadAction(this, "Assets/pencil.obj"));

    Gui::Widget* elem5 = new Gui::Button(10, 10 + 50 * 0,80,40, new ZoomAction(this, 1));
    Gui::Widget* elem6 = new Gui::Button(10, 10 + 50 * 1,80,40, new ZoomAction(this, 100));
    Gui::Widget* elem7 = new Gui::Button(10, 10 + 50 * 2,80,40, new ZoomAction(this, 1000));
    Gui::Widget* elem8 = new Gui::Button(10, 10 + 50 * 3,80,40, new ZoomAction(this, 2500));

    elem1->SetAlignment(0, 1);
    elem2->SetAlignment(0, 1);
    elem3->SetAlignment(0, 1);
    elem4->SetAlignment(0, 1);

    elem5->SetAlignment(1, 1);
    elem6->SetAlignment(1, 1);
    elem7->SetAlignment(1, 1);
    elem8->SetAlignment(1, 1);

    mEnv->AddWidget(elem1);
    mEnv->AddWidget(elem2);
    mEnv->AddWidget(elem3);
    mEnv->AddWidget(elem4);

    mEnv->AddWidget(elem5);
    mEnv->AddWidget(elem6);
    mEnv->AddWidget(elem7);
    mEnv->AddWidget(elem8);
}

void Modeler3D::OnUpdate(float64 dt)
{
    mAngle += 1.0 * dt;

    mEnv->SetSize(Window->GetWidth(), Window->GetHeight());
    mEnv->Update(dt);
}

void Modeler3D::OnRender()
{
    Graphics->SetClearColor(0.3, 0.3, 0.3);
    Graphics->Clear();

    if (mVbo)
    {
    	int32 amt = mMouse->GetWheelScroll();
    	if(amt != 0)
    	{
    		if(amt > 0)
    		{
    			mZoom -= pow(2,amt);
    		}
    		else
    		{
    			mZoom += pow(2,abs(amt));
    		}

    		if(mZoom < 1) mZoom = 1;
    	}

        Matrix4f projection = Matrix4f::ToPerspective(Math::ToRadians(70.0f), Graphics->GetAspectRatio(), 0.1f, 3000.0f);
        Matrix4f view = Matrix4f::ToLookAt(Vector3f(0, 1, mZoom), Vector3f::Zero, Vector3f::Up);
        Matrix4f model = Matrix4f::ToYaw(mAngle) * Matrix4f::ToPitch(mAngle * 1.3) * Matrix4f::ToRoll(mAngle * 1.7) * Matrix4f::ToTranslation(Vector3f(0.2, -0.8, 0));
        Matrix3f normalMat(Inverse(Transpose(model)));

        mShader->SetMatrix4f("Projection", projection);
        mShader->SetMatrix4f("View", view);
        mShader->SetMatrix4f("Model", model);
        mShader->SetMatrix3f("NormalMat", normalMat);

        Graphics->SetShader(mShader);
        Graphics->SetGeometry(mGeometry);
        Graphics->Draw(Video::Primitive::TriangleList, 0, mVbo->GetLength() / 3);
    }

    mGuiRenderer->Reset();
    mEnv->Draw(mGuiRenderer);
}

void Modeler3D::SetZoom(int32 zoom) { mZoom = zoom; }

void Modeler3D::OnDestroy()
{
    cout << "Destroying Modeler3D" << endl;
    mGuiRenderer->Release();
    mShader->Release();
    mGeometry->SetVertexBuffer(nullptr);
    mGeometry->Release();
    mVbo->Release();
}

}
