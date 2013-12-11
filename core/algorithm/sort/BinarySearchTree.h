/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BinarySearchTree.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>

namespace Etoile
{
	/*
	* Binary tree node data with left smaller, right biger two leafs.
	**/
	template <class K>
	class BinarySearchTreeNode
	{
	protected:
		BinarySearchTreeNode<K>* _pLeft;
		BinarySearchTreeNode<K>* _pRight;
		K _key;
	public:
		BinarySearchTreeNode(K key): _pLeft(NULL), _pRight(NULL), _key(key)
		{

		}

		void setLeftChildTreeNode(BinarySearchTreeNode<K>* left){_pLeft = left;}
		BinarySearchTreeNode<K>* getLeftChildTreeNode(){return _pLeft;}

		void setRightChildTreeNode(BinarySearchTreeNode<K>* right){_pRight = right;}
		BinarySearchTreeNode<K>* getRightChildTreeNode(){return _pRight;}

		K& getCurrentNodeKey(){return _key;}
		void setCurrentNodeKey(K key){_key = key;}
	};


	/*
	*	BinarySearchTree struct doing insert delete find, Traversal 
	**/
	template <class K>
	class BinarySearchTree
	{
		BinarySearchTreeNode<K>* _pRoot;
	public:
		BinarySearchTree(): _pRoot(NULL)
		{
		}

		void insertTreeNode(BinarySearchTreeNode<K>* node) 
		{
			if(_pRoot == NULL)
			{
				_pRoot = node;
			}
			else
			{
				insertTreeNode(_pRoot, node);
			}
		}

		BinarySearchTreeNode<K>* getTreeNodeByKey(const K& key)
		{
			if(_pRoot == NULL)
			{
				return NULL;
			}
			else
			{
				return getTreeNodeByKey(_pRoot, key);
			}
		}

		bool deleteTreeNodeByKey(const K& key)
		{
			if(_pRoot == NULL)
			{
				return false;
			}
			else
			{
				return deleteTreeNodeByKey(_pRoot, node);
			}
		}

		//only makes sense in binary trees ¨C the left node will be visited, then the root node, and then the right node.  this one can be used to do sorting
		std::vector<BinarySearchTreeNode<K>*> inOrderTraversals()
		{
			std::vector<BinarySearchTreeNode<K>*> values;
			if(_pRoot != NULL)
			{
				inOrderTraversals(values, _pRoot);
			}
			return values;
		}

		// the root node is visited, then the child nodes (left to right).
		void preOrderTraversals(BinarySearchTreeNode<K>* node)
		{
			if(node->getLeftChildTreeNode() != NULL)
			{
				preOrderTraversals(node->getLeftChildTreeNode());
			}
			if(node->getRightChildTreeNode() != NULL)
			{
				preOrderTraversals(node->getRightChildTreeNode());
			}
		}

		//only makes sense in binary trees ¨C the left node will be visited, then the root node, and then the right node.
		void inOrderTraversals(std::vector<BinarySearchTreeNode<K>*>& nodes, BinarySearchTreeNode<K>* node)
		{
			if(node->getLeftChildTreeNode() != NULL)
			{
				inOrderTraversals(nodes, node->getLeftChildTreeNode());
			}
			nodes.push_back(node);
			if(node->getRightChildTreeNode() != NULL)
			{
				inOrderTraversals(nodes, node->getRightChildTreeNode());
			}
		}

		//the child nodes will be visited before the root node (left to right).
		void postOrderTraversals(BinarySearchTreeNode<K>* node)
		{
			if(node->getLeftChildTreeNode() != NULL)
			{
				postOrderTraversals(node->getLeftChildTreeNode());
			}
			if(node->getRightChildTreeNode() != NULL)
			{
				postOrderTraversals(node->getRightChildTreeNode());
			}
		}


		//insert order left < right
		void insertTreeNode(BinarySearchTreeNode<K>* node, BinarySearchTreeNode<K>* newNode)
		{
			if(newNode->getCurrentNodeKey() < node->getCurrentNodeKey())
			{
				if(node->getLeftChildTreeNode() == NULL)
				{
					node->setLeftChildTreeNode(newNode);
				}
				else
				{
					insertTreeNode(node->getLeftChildTreeNode(),newNode);
				}
			}
			else
			{
				if(node->getRightChildTreeNode() == NULL)
				{
					node->setRightChildTreeNode(newNode);
				}
				else
				{
					insertTreeNode(node->getRightChildTreeNode(), newNode);
				}
			}
		}

		BinarySearchTreeNode<K>* getTreeNodeByKey(BinarySearchTreeNode<K>* node, const K& key)
		{
			if(node->getCurrentNodeKey() == key)
			{
				return this;
			}
			else if(key < node->getCurrentNodeKey())
			{
				if(node->getLeftChildTreeNode() == NULL) return NULL;
				return getTreeNodeByKey(node->getLeftChildTreeNode(), key);
			}
			else if(key > node->getCurrentNodeKey())
			{
				if(node->getRightChildTreeNode() == NULL) return NULL;
				return getTreeNodeByKey(node->getLeftChildTreeNode(), key);
			}
		}

		bool deleteTreeNodeByKey(BinarySearchTreeNode<K>* &node, const K& key)
		{
			if(key < node->getCurrentNodeKey())
			{
				if(node->getLeftChildTreeNode() == NULL) return false;
				return deleteTreeNodeByKey(node->getLeftChildTreeNode(), key);
			}
			else if(key > node->getCurrentNodeKey())
			{
				if(node->getRightChildTreeNode() == NULL) return false;
				return deleteTreeNodeByKey(node->getRightChildTreeNode(), key);
			}
			else
			{
				BinarySearchTreeNode<K>* temp;
				if(node->getLeftChildTreeNode() == NULL)
				{
					temp = node->getRightChildTreeNode();
					delete node;
					node = temp;
				}
				else if(node->getRightChildTreeNode() == NULL)
				{
					temp = node->getLeftChildTreeNode();
					delete node;
					node = temp;
				}
				else
				{
					temp = node->getRightChildTreeNode();
					while(temp->getLeftChildTreeNode() != NULL)
					{
						temp = temp->getLeftChildTreeNode();
					}
					node->setCurrentNodeKey(temp->getCurrentNodeKey());
					deleteTreeNodeByKey(node->getRightChildTreeNode(), temp->getCurrentNodeKey());
				}
			}

		}
	};

	typedef BinarySearchTree<double> BinarySearchTreed;
}