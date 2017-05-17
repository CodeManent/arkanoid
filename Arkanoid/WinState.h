#ifndef Arkanoid_WinState_H
#define Arkanoid_WinState_H

#include "../State.h"
#include <vector>
#include <utility>//foir pair
#include "../point2.h"

namespace Arkanoid
{
	class WinState: public State
	{
	public:
		WinState();
		~WinState();

		virtual bool step();
		virtual void draw();
		virtual void input(const Input& input);

	private:
		struct TreeNode
		{
			point2f position;
			int angle;
			std::vector<TreeNode> following;
		} root;

		void addTreeNode();
		TreeNode* getLeaf(TreeNode &current, int &depth);
		static const int maxBranches;
		static const int maxDepth;
	};
}


#endif
