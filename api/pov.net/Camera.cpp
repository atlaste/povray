#include "Stdafx.h"
#include "Camera.h"

namespace povray
{
	void Camera::Render(Context^ context)
	{
		if (context->camera)
		{
			throw gcnew Exception("You can only have one camera in a scene.");
		}
		context->camera = new pov::Camera();

		auto cam = context->camera;
		cam->Location = Location.ToVector();
		cam->Look_At = LookAt.ToVector();
		// etc.

		// Parser.cpp:1759 also contains a lot of logic:

		auto Direction_Length = cam->Direction.length();
		auto Up_Length = cam->Up.length();
		auto Right_Length = cam->Right.length();
		auto tempv = cross(cam->Up, cam->Direction);
		auto Handedness = dot(tempv, cam->Right);

		cam->Direction = cam->Look_At - cam->Location;

		// Check for zero length direction vector.
		if (cam->Direction.lengthSqr() < EPSILON)
		{
			context->Error("Camera location and look_at point must be different.");
		}

		cam->Direction.normalize();

		// Save Right vector
		tempv = cam->Right;

		cam->Right = cross(cam->Sky, cam->Direction);

		// Avoid DOMAIN error (from Terry Kanakis)
		if ((fabs(cam->Right[0]) < EPSILON) &&
			(fabs(cam->Right[1]) < EPSILON) &&
			(fabs(cam->Right[2]) < EPSILON))
		{
			context->Warning("Camera location to look_at direction and sky direction should be different.\nUsing default/supplied right vector instead.");

			// Restore Right vector
			cam->Right = tempv;
		}

		cam->Right.normalize();
		cam->Up = cross(cam->Direction, cam->Right);
		cam->Direction *= Direction_Length;

		if (Handedness > 0.0)
		{
			cam->Right *= Right_Length;
		}
		else
		{
			cam->Right *= -Right_Length;
		}

		cam->Up *= Up_Length;
	}
}