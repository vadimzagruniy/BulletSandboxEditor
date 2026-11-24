#pragma once

#include "core/node/Node.h"

#include "imgui.h"
#include "nlohmann/json_fwd.hpp"
#include <array>

namespace core
{

class IPropertyInspector;
class CCamera;

class CSceneNode:
  public CNode
{
public: // Types

  using TCorners = std::array<ImVec2, 4>;

public: // Constructor/Destructor

  CSceneNode();

  ~CSceneNode();

public: // Interface

  void SetOrigin(const ImVec2 & _Position);

  ImVec2 GetOrigin() const;

  void SetRotation(float _Rotation);

  float GetRotation() const;

  void SetScale(const ImVec2 & _Scale);

  ImVec2 GetScale() const;

  ImVec2 GetSize() const;

  void SetSize(const ImVec2 & _Size);

  virtual void Render(const core::CCamera & _Camera);

  virtual void Update(float _DelataTime);

  virtual nlohmann::ordered_json Serialize();

  virtual void Deserialize(const nlohmann::ordered_json & _Data);

  virtual std::string_view GetTypeID() const = 0;

  virtual void InspectProperties(const std::shared_ptr<IPropertyInspector> & _PropertyInspector);

  void GetCorners(TCorners & _Corners) const;

  ImVec2 WorldToParent(const ImVec2 & _WorldPos) const;

  ImVec2 ParentToWorld(const ImVec2 & _ParentPos) const;

  ImVec2 WorldToLocal(const ImVec2 & _WorldPos) const;

  ImVec2 LocalToWorld(const ImVec2 & _LocalPos) const;

  bool HitTest(const ImVec2 & _Position) const;

protected: // Service

  virtual void RenderContent(const core::CCamera & _Camera);

protected: // Attr

  ImVec2 m_Size;
  ImVec2 m_Origin;
  ImVec2 m_Scale;
  float  m_Rotation;
  ImVec4 m_Color; 
};

} // namespace core
