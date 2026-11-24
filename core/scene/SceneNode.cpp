#include "SceneNode.h"

#include <core/property/IPropertyVisitor.h>
#include <core/camera/Camera.h>
#include <core/utils/Math.h>
#include "nlohmann/json.hpp"
#include <limits>
#include <imgui_internal.h>

namespace core
{

namespace // unnamed
{

constexpr const char * NAME_TAG       = "Name";
constexpr const char * SIZE_TAG       = "Size";
constexpr const char * SIZE_X_TAG     = "Width";
constexpr const char * SIZE_Y_TAG     = "Height";
constexpr const char * ORIGIN_TAG     = "Origin";
constexpr const char * POSITION_X_TAG = "PositionX";
constexpr const char * POSITION_Y_TAG = "PositionY";
constexpr const char * ROTATION_TAG   = "Rotation";
constexpr const char * SCALE_TAG      = "Scale";
constexpr const char * SCALE_X_TAG    = "ScaleX";
constexpr const char * SCALE_Y_TAG    = "ScaleY";
constexpr const char * COLOR_TAG      = "Color";
constexpr const char * COLOR_R_TAG    = "ColorR";
constexpr const char * COLOR_G_TAG    = "ColorG";
constexpr const char * COLOR_B_TAG    = "ColorB";
constexpr const char * COLOR_A_TAG    = "ColorA";

const std::string DEFAULT_NAME     = "SceneNode";

} // namespace unnamed

//
// Constructor/Destructor
//

CSceneNode::CSceneNode():
  m_Size(100.0f, 100.0f),
  m_Origin(0.0f, 0.0f),
  m_Scale(1.0f, 1.0f),
  m_Rotation(0.0f),
  m_Color(0.5f, 0.5f, 0.7f, 1.0f)
{
  // Empty
}

CSceneNode::~CSceneNode() = default;

//
// Interface
//

void CSceneNode::SetOrigin(const ImVec2 & _Position)
{
  m_Origin = _Position;
}

ImVec2 CSceneNode::GetOrigin() const
{
  return m_Origin;
}

void CSceneNode::SetRotation(float _Rotation)
{
  m_Rotation = _Rotation;
}

float CSceneNode::GetRotation() const
{
  return m_Rotation;
}

void CSceneNode::SetScale(const ImVec2 & _Scale)
{
  m_Scale = _Scale;
}

ImVec2 CSceneNode::GetScale() const
{
  return m_Scale;
}

ImVec2 CSceneNode::GetSize() const
{
  return m_Size;
}

void CSceneNode::SetSize(const ImVec2 & _Size)
{
  m_Size = _Size;
}

void CSceneNode::Render(const core::CCamera & _Camera)
{
  RenderContent(_Camera);
  
  for(const auto & Child : m_Children)
    std::static_pointer_cast<CSceneNode>(Child)->Render(_Camera);
}

void CSceneNode::Update(float _DelataTime)
{
  for(const auto & Child : m_Children)
    std::static_pointer_cast<CSceneNode>(Child)->Update(_DelataTime);
}

nlohmann::ordered_json CSceneNode::Serialize()
{
  nlohmann::ordered_json Data;

  Data[SIZE_X_TAG]     = m_Size.x;
  Data[SIZE_Y_TAG]     = m_Size.y;
  Data[POSITION_X_TAG] = m_Origin.x;
  Data[POSITION_Y_TAG] = m_Origin.y;
  Data[ROTATION_TAG]   = m_Rotation;
  Data[SCALE_X_TAG]    = m_Scale.x;
  Data[SCALE_Y_TAG]    = m_Scale.y;
  Data[COLOR_R_TAG]    = m_Color.x;
  Data[COLOR_G_TAG]    = m_Color.y;
  Data[COLOR_B_TAG]    = m_Color.z;
  Data[COLOR_A_TAG]    = m_Color.w;

  return Data;
}

void CSceneNode::Deserialize(const nlohmann::ordered_json & _Data)
{
  m_Size.x   = _Data.value(SIZE_X_TAG,     m_Size.x);
  m_Size.y   = _Data.value(SIZE_Y_TAG,     m_Size.y);
  m_Origin.x = _Data.value(POSITION_X_TAG, m_Origin.x);
  m_Origin.y = _Data.value(POSITION_Y_TAG, m_Origin.y);
  m_Rotation = _Data.value(ROTATION_TAG,   m_Rotation);  
  m_Scale.x  = _Data.value(SCALE_X_TAG,    m_Scale.x);
  m_Scale.y  = _Data.value(SCALE_Y_TAG,    m_Scale.y);
  m_Color.x  = _Data.value(COLOR_R_TAG,    m_Color.x);
  m_Color.y  = _Data.value(COLOR_G_TAG,    m_Color.y);
  m_Color.z  = _Data.value(COLOR_B_TAG,    m_Color.z);
  m_Color.w  = _Data.value(COLOR_A_TAG,    m_Color.w);
}

void CSceneNode::InspectProperties(const std::shared_ptr<IPropertyInspector> & _PropertyInspector)
{
  _PropertyInspector->BeginGroup("SceneNode");

  _PropertyInspector->Vec2(ORIGIN_TAG, m_Origin, std::numeric_limits<float>::max(),std::numeric_limits<float>::min());
  _PropertyInspector->Vec2(SIZE_TAG, m_Size, std::numeric_limits<float>::max(), 1.0f);
  _PropertyInspector->Vec2(SCALE_TAG, m_Scale, std::numeric_limits<float>::max(), 0.01f);
  _PropertyInspector->Angle(ROTATION_TAG, m_Rotation, 2.0f * IM_PI , 2.0f * -IM_PI);
  _PropertyInspector->Color(COLOR_TAG, m_Color);
}

void CSceneNode::GetCorners(TCorners & _Corners) const
{
  ImVec2 HalfSize = m_Size * 0.5f;

  TCorners LocalCorners =
  {
    ImVec2(-HalfSize.x, -HalfSize.y),
    ImVec2( HalfSize.x, -HalfSize.y),
    ImVec2( HalfSize.x,  HalfSize.y),
    ImVec2(-HalfSize.x,  HalfSize.y)
  };

  for (int i = 0; i < LocalCorners.size(); ++i)
    _Corners[i] = LocalToWorld(LocalCorners[i]);
}

ImVec2 CSceneNode::WorldToParent(const ImVec2 & _WorldPos) const
{
  const auto Parent = std::static_pointer_cast<CSceneNode>(m_Parent.lock());
  return Parent != nullptr ? Parent->WorldToLocal(_WorldPos) : _WorldPos;
}

ImVec2 CSceneNode::ParentToWorld(const ImVec2 & _ParentPos) const
{
  const auto Parent = std::static_pointer_cast<CSceneNode>(m_Parent.lock());
  return Parent != nullptr ? Parent->LocalToWorld(_ParentPos) : _ParentPos;
}

ImVec2 CSceneNode::WorldToLocal(const ImVec2 & _WorldPos) const
{
  ImVec2 Position = math::Rotate(WorldToParent(_WorldPos) - m_Origin, -m_Rotation);
  return ImVec2(m_Scale.x > 0.0f ? Position.x / m_Scale.x : 0.0f, m_Scale.y > 0.0f ? Position.y / m_Scale.y : 0.0f);
}

ImVec2 CSceneNode::LocalToWorld(const ImVec2 & _LocalPos) const
{
  return ParentToWorld(m_Origin + (math::Rotate(_LocalPos * m_Scale, m_Rotation)));
}

bool CSceneNode::HitTest(const ImVec2 & _Position) const
{
  ImVec2 LocalPos = WorldToLocal(_Position);
  ImVec2 HalfSize = m_Size * 0.5f;

  bool IsPointIn = (-HalfSize.x <= LocalPos.x) && (LocalPos.x < HalfSize.x) &&
                   (-HalfSize.y <= LocalPos.y) && (LocalPos.y < HalfSize.y);

  return IsPointIn;
}

//
// Service
//

void CSceneNode::RenderContent(const core::CCamera & _Camera)
{
  TCorners Corners;
  GetCorners(Corners);

  for (auto & Corner : Corners)
    Corner = _Camera.ToScreen(Corner);

  auto * DrawList = ImGui::GetWindowDrawList();
  DrawList->AddQuadFilled(Corners[0], Corners[1], Corners[2], Corners[3], ImGui::GetColorU32(m_Color));
}

} // namespace core
