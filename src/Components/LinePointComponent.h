#pragma once

#include "../Opal/EntityComponent/Component.h"

#include <glm/vec3.hpp>

class LinePointComponent : public Opal::Component
{
    public:
    LinePointComponent();
    LinePointComponent(glm::vec2 position, int id, std::shared_ptr<LinePointComponent> sibling = nullptr);
    ~LinePointComponent();

    virtual void OnAdded() override;
    virtual void OnStart() override;
    virtual void Update(float dTime) override;
    virtual void Render(std::shared_ptr<Opal::BatchRenderer2D> ctx) override;
    virtual void OnCollision(std::shared_ptr<Opal::Entity> other, glm::vec2 resolution, Opal::AABB otherAABB) override;
    virtual void Serialize() override;
    virtual void Deserialize() override;
    virtual std::shared_ptr<Component> Clone() override;

    void SetSibling(std::shared_ptr<LinePointComponent> sibling);

    glm::vec2 Position;
    int Id;
    int SiblingId;

    private:
    std::shared_ptr<LinePointComponent> mSibling = nullptr;
};
