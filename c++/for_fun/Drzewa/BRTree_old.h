#ifndef RTSHADOW_BRTREE
#define RTSHADOW_BRTREE

#include "BST.h"

template <class T>
class BRTree : public BST<T>
{
	private:
	static typename BST<T> :: Node* CreateBRTreeNode() { return new BRNode(); }
	
	protected:
	struct BRNode : public BST<T> :: Node
	{
		enum Color {BLACK, RED};
		Color color;
		
		BRNode() : color(RED) {}
	};
	
	public:
	BRTree() { NodeCreator = BRTree<T> :: CreateBRTreeNode; }
};

#endif
