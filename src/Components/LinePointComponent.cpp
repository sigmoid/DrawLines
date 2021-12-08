#include "LinePointComponent.h"

LinePointComponent::LinePointComponent()
{
    Position = glm::vec2(0,0);

    TypeName = "LinePointComponent";
    Parameters.AddVec2("Position", &Position);
}

LinePointComponent::LinePointComponent(glm::vec2 position, int id, std::shared_ptr<LinePointComponent> sibling)
{
    Position = position;
    Id = id;
    mSibling = sibling;

    if(mSibling != nullptr)
    {
        SiblingId = mSibling->Id;
    }

    TypeName = "LinePointComponent";
    Parameters.AddVec2("Position", &Position);
    Parameters.AddInt("Id", &Id);
    Parameters.AddInt("SiblingId", &SiblingId);
}

LinePointComponent::~LinePointComponent()
{

}

void LinePointComponent::OnAdded()
{

}

void LinePointComponent::OnStart()
{
    
}

void LinePointComponent::Update(float dTime)
{
    
}

void LinePointComponent::Render(std::shared_ptr<Opal::BatchRenderer2D> ctx)
{
    
}

void LinePointComponent::OnCollision(std::shared_ptr<Opal::Entity> other, glm::vec2 resolution, Opal::AABB otherAABB)
{
    
}

void LinePointComponent::Serialize()
{
    
}

void LinePointComponent::Deserialize()
{
    
}

std::shared_ptr<Opal::Component> LinePointComponent::Clone()
{
    return std::make_shared<LinePointComponent>();
}

void LinePointComponent::SetSibling(std::shared_ptr<LinePointComponent> sibling)
{
    mSibling = sibling;

    if(mSibling != nullptr)
    {
        SiblingId = mSibling->Id;
    }
}
