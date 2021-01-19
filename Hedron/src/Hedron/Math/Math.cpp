#include "hdrpch.h"
#include "Hedron/Math/Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Hedron::Math
{
	bool decompose_transform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale)
	{
		// From glm::decompose in matrix_decompose.inl
		using namespace glm;
		using T = float;

		mat4 localMatrix(transform);


		if (epsilonEqual(localMatrix[3][3], static_cast<T>(0), epsilon<T>()))
			return false;

		if (
			epsilonNotEqual(localMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(localMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(localMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{
			// Clear the perspective partition
			localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<T>(0);
			localMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		outTranslation = vec3(localMatrix[3]);
		localMatrix[3] = vec4(0, 0, 0, localMatrix[3].w);

		vec3 row[3]; // , pdum3;

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				row[i][j] = localMatrix[i][j];

		// Compute X scale factor and normalize first row.
		outScale.x = length(row[0]);
		row[0] = detail::scale(row[0], static_cast<T>(1));
		outScale.y = length(row[1]);
		row[1] = detail::scale(row[1], static_cast<T>(1));
		outScale.z = length(row[2]);
		row[2] = detail::scale(row[2], static_cast<T>(1));

#if 0
		pdum3 = cross(row[1], row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(row[0], pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				outScale[i] *= static_cast<T>(-1);
				row[i] *= static_cast<T>(-1);
			}
		}
#endif

		outRotation.y = asin(-row[0][2]);
		if (cos(outRotation.y) != 0)
		{
			outRotation.x = atan2(row[1][2], row[2][2]);
			outRotation.z = atan2(row[0][1], row[0][0]);
		}
		else
		{
			outRotation.x = atan2(-row[2][0], row[1][1]);
			outRotation.z = 0;
		}

		return true;
	}
}