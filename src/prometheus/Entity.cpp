#include "Component.h"
#include "Transform.h"
#include "Entity.h"

namespace prometheus
{
	std::shared_ptr<Core> Entity::GetCore()
	{
		return core.lock();
	}

	Entity::Entity()
	{
		transform = addComponent<Transform>();
	}

	Entity::Entity(std::string _entityName)
	{
		transform = addComponent<Transform>();
		entityName = _entityName;
	}

	void Entity::SetPosition(glm::vec3 _position)
	{
		GetTransform()->SetLocalPosition(_position);
	}

	void Entity::SetRotation(glm::vec3 _rotation)
	{
		GetTransform()->SetLocalRotation(_rotation);
	}

	std::shared_ptr<Transform> Entity::GetTransform()
	{
		return transform.lock();
	}

	std::string Entity::GetEntityName()
	{
		return entityName;
	}

	void Entity::OnTick()
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end(); ++it)
		{
			(*it)->OnTick();
		}
	}

	void Entity::OnDisplay()
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end(); ++it)
		{
			(*it)->OnDisplay();
		}
	}
	void Entity::OnInit()
	{
		for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end(); ++it)
		{
			(*it)->OnInit();
		}
	}
}
