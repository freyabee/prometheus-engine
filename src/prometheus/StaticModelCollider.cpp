#include "StaticModelCollider.h"
#include "MeshRenderer.h"
#include "Entity.h"
#include "Model.h"
//#include "MaterialGroup.h"
//#include "Part.h"
#include "MeshRenderer.h"
#include <cmath>


namespace prometheus
{
	int triBoxOverlap(float boxcenter[3],float boxhalfsize[3], float triverts[3][3]);


	bool ColliderColumn::IsColliding(glm::vec3 position, glm::vec3 size)
	{
		for (std::vector<qsoft::Face>::iterator i = faces.begin(); i != faces.end(); i++)
		{
			float f[3][3] = { 0 };
			f[0][0] = i->a.position.x;
			f[0][1] = i->a.position.y;
			f[0][2] = i->a.position.z;
			f[1][0] = i->b.position.x;
			f[1][1] = i->b.position.y;
			f[1][2] = i->b.position.z;
			f[2][0] = i->c.position.x;
			f[2][1] = i->c.position.y;
			f[2][2] = i->c.position.z;

			float bc[3] = { 0 };
			bc[0] = position.x;
			bc[1] = position.y;
			bc[2] = position.z;
			float bhs[3] = { 0 };
			bhs[0] = size.x / 2.0f;
			bhs[1] = size.y / 2.0f;
			bhs[2] = size.z / 2.0f;

			if (triBoxOverlap(bc, bhs, f))
			{
				return true;
			}
		}

		return false;
	}

	void ColliderColumn::GetColliding(glm::vec3 position, glm::vec3 size, std::vector<qsoft::Face>& collisions)
	{
		for (std::vector<qsoft::Face>::iterator i = faces.begin(); i != faces.end(); i++)
		{
			float f[3][3] = { 0 };
			f[0][0] = i->a.position.x;
			f[0][1] = i->a.position.y;
			f[0][2] = i->a.position.z;
			f[1][0] = i->b.position.x;
			f[1][1] = i->b.position.y;
			f[1][2] = i->b.position.z;
			f[2][0] = i->c.position.x;
			f[2][1] = i->c.position.y;
			f[2][2] = i->c.position.z;

			float bc[3] = { 0 };
			bc[0] = position.x;
			bc[1] = position.y;
			bc[2] = position.z;
			float bhs[3] = { 0 };
			bhs[0] = size.x / 2.0f;
			bhs[1] = size.y / 2.0f;
			bhs[2] = size.z / 2.0f;

			if (triBoxOverlap(bc, bhs, f))
			{
				collisions.push_back(*i);
			}
		}
	}

	bool StaticModelCollider::IsColliding(qsoft::Face& face,
		glm::vec3 position, glm::vec3 size)
	{
		float f[3][3] = { 0 };
		f[0][0] = face.a.position.x;
		f[0][1] = face.a.position.y;
		f[0][2] = face.a.position.z;
		f[1][0] = face.b.position.x;
		f[1][1] = face.b.position.y;
		f[1][2] = face.b.position.z;
		f[2][0] = face.c.position.x;
		f[2][1] = face.c.position.y;
		f[2][2] = face.c.position.z;

		float bc[3] = { 0 };
		bc[0] = position.x;
		bc[1] = position.y;
		bc[2] = position.z;
		float bhs[3] = { 0 };
		bhs[0] = size.x / 2.0f;
		bhs[1] = size.y / 2.0f;
		bhs[2] = size.z / 2.0f;

		if (triBoxOverlap(bc, bhs, f))
		{
			return true;
		}

		return false;
	}

	bool StaticModelCollider::IsColliding(glm::vec3 position,
		glm::vec3 size)
	{
		for (std::vector<qsoft::Face>::iterator i = collisions.begin();
			i != collisions.end(); i++)
		{
			float f[3][3] = { 0 };
			f[0][0] = i->a.position.x;
			f[0][1] = i->a.position.y;
			f[0][2] = i->a.position.z;
			f[1][0] = i->b.position.x;
			f[1][1] = i->b.position.y;
			f[1][2] = i->b.position.z;
			f[2][0] = i->c.position.x;
			f[2][1] = i->c.position.y;
			f[2][2] = i->c.position.z;

			float bc[3] = { 0 };
			bc[0] = position.x;
			bc[1] = position.y;
			bc[2] = position.z;
			float bhs[3] = { 0 };
			bhs[0] = size.x / 2.0f;
			bhs[1] = size.y / 2.0f;
			bhs[2] = size.z / 2.0f;

			if (triBoxOverlap(bc, bhs, f))
			{
				return true;
			}
		}

		return false;
	}

	void StaticModelCollider::GetColliding(glm::vec3 position,
		glm::vec3 size)
	{
		glm::vec3 pos = position - extent.min;
		size_t x = (size_t)(pos.x / cols.at(0)->size.x);
		size_t y = (size_t)(pos.z / cols.at(0)->size.z);
		size_t idx = y * resolution + x;

		if (idx >= cols.size()) return;

		cols.at(idx)->GetColliding(position, size, collisions);
	}

	glm::vec3 StaticModelCollider::FaceNormal(qsoft::Face& face)
	{
		// Winding for CCW?
		//glm::vec3 N = glm::vec3::cross(
		//  glm::vec3(face.b.position) - glm::vec3(face.a.position),
		//  glm::vec3(face.c.position) - glm::vec3(face.a.position));

		glm::vec3 N = glm::vec3::cross(
			glm::vec3(face.b.position) - glm::vec3(face.c.position),
			glm::vec3(face.a.position) - glm::vec3(face.c.position));

		return N;
	}

	glm::vec3 StaticModelCollider::GetCollisionResponse(glm::vec3 position, glm::vec3 size, bool& solved)
	{
		glm::vec3 solve = position;
		solved = false;

		collisions.clear();
		GetColliding(solve, size);

		if (!IsColliding(solve, size))
		{
			solved = true;
			return solve;
		}

		// Favour Y faces first.
		for (std::vector<qsoft::Face>::iterator it = collisions.begin();
			it != collisions.end(); it++)
		{
			if (!IsColliding(*it, solve, size))
			{
				continue;
			}

			glm::vec3 n = FaceNormal(*it);
			n = glm::normalize(n);
			//std::cout << n.x << " " << n.y << " " << n.z << std::endl;
			if (n.y < fabs(n.x) + fabs(n.z)) continue;

			float amount = tryStep;

			while (true)
			{
				solve = solve + n * amount;

				if (!IsColliding(*it, solve, size))
				{
					break;
				}

				solve = solve - n * amount;
				amount += tryStep;

				if (amount > maxStep)
				{
					break;
				}
			}
		}

		if (!IsColliding(solve, size))
		{
			solved = true;
			return solve;
		}

		float amount = tryInc;

		while (true)
		{
			glm::vec3 total;

			// Try to uncollide using face normals
			for (std::vector<qsoft::Face>::iterator it = collisions.begin();
				it != collisions.end(); it++)
			{
				glm::vec3 n = FaceNormal(*it);
				n = glm::normalize(n);
				total = total + n;
				solve = solve + n * amount;

				if (!IsColliding(solve, size))
				{
					solved = true;
					return solve;
				}

				solve = solve - n * amount;
			}

			// Try to uncollide using averaged face normals
			total = total.normalize();
			solve = solve + total * amount;

			if (!IsColliding(solve, size))
			{
				solved = true;
				return solve;
			}

			solve = solve - total * amount;
			amount += tryInc;

			if (amount > maxInc)
			{
				break;
			}
		}

		solved = false;
		return position;
	}

	void StaticModelCollider::GenerateExtent()
	{
		std::vector<glm::vec3> positions;
		std::sr1::shared_ptr<ModelRenderer> mr = getEntity()->getComponent<ModelRenderer>();
		std::sr1::shared_ptr<Model> model = mr->getModel();

		for (size_t f = 0; f < model->faces.size(); f++)
		{
			qsoft::Face face = model->faces.at(f);
			positions.push_back(face.a.position);
			positions.push_back(face.b.position);
			positions.push_back(face.c.position);
		}

		extent.max = positions.at(0);
		extent.min = positions.at(0);

		for (size_t i = 1; i < positions.size(); i++)
		{
			if (positions.at(i).x > extent.max.x) extent.max.x = positions.at(i).x;
			if (positions.at(i).y > extent.max.y) extent.max.y = positions.at(i).y;
			if (positions.at(i).z > extent.max.z) extent.max.z = positions.at(i).z;
			if (positions.at(i).x < extent.min.x) extent.min.x = positions.at(i).x;
			if (positions.at(i).y < extent.min.y) extent.min.y = positions.at(i).y;
			if (positions.at(i).z < extent.min.z) extent.min.z = positions.at(i).z;
		}

		extent.min = extent.min - 1;
		extent.max = extent.max + 1;

		/*
		  std::cout << "World Bounds: " << path << std::endl;
		  std::cout << "Size: " << extent.max.x - extent.min.x;
		  std::cout << ", " << extent.max.y - extent.min.y;
		  std::cout << ", " << extent.max.z - extent.min.z << std::endl;
		  std::cout << "Min: " << extent.min.x;
		  std::cout << ", " << extent.min.y;
		  std::cout << ", " << extent.min.z << std::endl;
		  std::cout << "Max: " << extent.max.x;
		  std::cout << ", " << extent.max.y;
		  std::cout << ", " << extent.max.z << std::endl;
		*/

	}

	void StaticModelCollider::onInit()
	{
		resolution = 10;
		tryStep = 0.001f;
		maxStep = 1.0f;
		tryInc = 0.01f;
		maxInc = 0.5f;

		std::sr1::shared_ptr<ModelRenderer> mr = getEntity()->getComponent<ModelRenderer>();
		std::sr1::shared_ptr<Model> model = mr->getModel();
		GenerateExtent();

		// Create collision columns
		glm::vec3 size = extent.max - extent.min;
		glm::vec3 colSize = size / resolution;
		colSize.y = size.y;

		for (size_t y = 0; y < resolution; y++)
		{
			glm::vec3 pos = extent.min + colSize / 2.0f;
			pos.z += (float)y * colSize.z;

			for (size_t x = 0; x < resolution; x++)
			{
				std::sr1::shared_ptr<ColliderColumn> cc = std::sr1::make_shared<ColliderColumn>();
				cc->size = colSize;

				// Overlap columns for sub column collision
				//cc->size.x += 1.0f;
				//cc->size.z += 1.0f;
				// Conflicts with x / y index generation when matching column to collide with.
				// Done when adding face instead.

				cc->position = pos;
				cols.push_back(cc);
				pos.x += colSize.x;
			}
		}

		for (size_t f = 0; f < model->faces.size(); f++)
		{
			qsoft::Face face = model->faces.at(f);
			AddFace(face);
		}
	}

	Extent StaticModelCollider::GetExtent()
	{
		return extent;
	}

	void StaticModelCollider::AddFace(qsoft::Face face)
	{
		float f[3][3] = { 0 };
		f[0][0] = face.a.position.x;
		f[0][1] = face.a.position.y;
		f[0][2] = face.a.position.z;
		f[1][0] = face.b.position.x;
		f[1][1] = face.b.position.y;
		f[1][2] = face.b.position.z;
		f[2][0] = face.c.position.x;
		f[2][1] = face.c.position.y;
		f[2][2] = face.c.position.z;

		bool found = false;

		for (size_t i = 0; i < cols.size(); i++)
		{
			float bc[3] = { 0 };
			bc[0] = cols.at(i)->position.x;
			bc[1] = cols.at(i)->position.y;
			bc[2] = cols.at(i)->position.z;

			// Overlap columns for sub column collision
			glm::vec3 s = cols.at(i)->size;
			s.x += 1;
			s.z += 1;

			float bhs[3] = { 0 };
			bhs[0] = s.x / 2.0f;
			bhs[1] = s.y / 2.0f;
			bhs[2] = s.z / 2.0f;

			if (triBoxOverlap(bc, bhs, f))
			{
				cols.at(i)->faces.push_back(face);
				//std::cout << "Pushing face into " << i << std::endl;
				found = true;
			}
		}

		/*
		if (!found)
		{
			throw Exception("Face not assigned spatial partition");
		}
		*/
		/*
		  if(!found)
		  {
			std::cout << "Assertion failed: Face not in spacial partition" << std::endl;
			std::cout << f[0][0] << ", " << f[0][1] << ", " << f[0][2] << std::endl;
			std::cout << f[1][0] << ", " << f[1][1] << ", " << f[1][2] << std::endl;
			std::cout << f[2][0] << ", " << f[2][1] << ", " << f[2][2] << std::endl;
			std::cout << "Expect collision errors" << std::endl;
			//abort();
		  }
		*/
	}

}
