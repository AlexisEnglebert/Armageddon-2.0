#include "Joint.h"

void DECL Skeleton::UpdateJoint(std::shared_ptr<Joint>& Parentjoint, std::shared_ptr<Joint>& currentJoint)
{
	/*if (Parentjoint)
		currentJoint->m_inverseBlindPose = */
	for (size_t i = 0; i < currentJoint->m_child.size(); i++)
	{
		UpdateJoint(currentJoint, currentJoint->m_child[i]);
	}
}