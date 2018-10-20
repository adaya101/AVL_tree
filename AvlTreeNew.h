#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdlib.h>
#include <iostream>



static const int EQUALS = 0;
static const int IS_BIGGER = 1;


using namespace std;

template<typename T, typename K, typename C>

class AvlTree
{
    public:
        //*********************************************************
        //***************************NODE CLASS********************
        //*********************************************************
        class Node{
          private:
            K key;
            T* value;
            Node* parent;
            Node* right;
            Node* left;
            int BF;
            int height;
            int rank;

          public:
            //constructor for Node class
            Node(const K& key, T* value):
                key(key), value(new T(*value)), parent(NULL),right(NULL),left(NULL),BF(0),height(0),rank(1){};
            //destructor for Node class
            ~Node(){
              delete value;
            };

            ///////////////////update functions/////////////
            void updateBF(int n);
            void updateHeight(int n);
            void updateRank(int n);
            void updateParent(Node* n);
            void updateLeft(Node* n);
            void updateRight(Node* n);
            void updateValue(T* t, bool deleteYn);
            void updateKey(const K& k);

            ///////////////////get functions/////////////
            K getKey() const{return key;};
            T* getValue() const {return value;};
            Node* getLeft() const {return left;};
            Node* getRight() const {return right;};
            Node* getParent() const {return parent;};
            int getBF() const {return BF;};
            int getHeight() const {return height;};
            int getRank() const {return rank;};

            bool isLeaf()const;
        };
        //************************************************************************

      AvlTree(const C& compare):avlRoot(NULL),avlSize(0),avlBiggest(NULL),avlCompare(compare){};
      ~AvlTree() = default;

      ///////////////////update functions/////////////

      void updateRoot(Node*);
      void updateBiggest(Node*);
      void updateKeyNode(Node*,const K&);
      void updateNodeFather(Node* father, Node* son, Node* grandson);

      ///////////////////get functions/////////////
       Node* getRoot() const;
      // int getSize() const;
       Node* getBiggestNode() const;
       C getCompare() const;
       Node* getFollow(Node* node) const;

      ////////////////////Rotations////////////////

      void rotation_RR(Node*);
      void rotation_RL(Node*);
      void rotation_LL(Node*);
      void rotation_LR(Node*);

      //////////////////removes/////////////////////
      void remove_aux(const K& key, T* value ,AvlTree<T, K, C>::Node* result);
      void remove(const K&, T*);
      void removeLeaf(Node*);
      void removeOneSon(Node*);
      void removeTwoSons(Node*);
      void remove_node(const K& key, T* value, Node* result);

      Node* FindBiggest();

      void buildEmptyTee(int n);
      void buildFullTree(int n);

      /////////////////////////FIX/////////////////////////////

      void fixBalance(Node*);
      void fixHeight(Node*);
      void fixRank(Node*);
      void fixBF(Node*);

      int nodeHeight(Node* node) const;

      /////////////////////tree functions/////////////////////
      Node* recursive(const K& key, T* value, Node* node, int*) const;
      Node* find(const K& key, T* value, bool* wasFound) const;
      void insert(const K& key, T* value);

      Node* copy(Node* treeOriginal, Node* parent);

    private:
      Node* avlRoot;
      int avlSize;
      Node* avlBiggest;
      C avlCompare;

      //a function that deletes the AVL
      //void DeleteAvlNode(Node<T,K>* node);

};


//////////////////////////////////////////help functions////////////////////////////////////////////////////////////////

///////////////////////////////////////////implementation of AvlTreeNode///////////////////////////////////////////////////

////////////////update AvlNode/////////////
template<typename T, typename K, typename C>
void AvlTree<T,K,C>::Node::updateRank(int n){
  rank = n;
}

template<typename T, typename K, typename C>
void AvlTree<T,K,C>::Node::updateParent(Node* n){
  parent = n;
}

template<typename T, typename K, typename C>
void AvlTree<T,K,C>::Node::updateLeft(Node* n){
  left = n;
}

template<typename T, typename K, typename C>
void AvlTree<T,K,C>::Node::updateRight(Node* n){
  right = n;
}

template<typename T, typename K, typename C>
void AvlTree<T,K,C>::Node::updateValue(T* t, bool deleteYN){
  value = t;
}

template<typename T, typename K, typename C>
void AvlTree<T,K,C>::Node::updateKey(const K& k){
   key = k;
}

template<typename T, typename K, typename C>
void AvlTree<T,K,C>::Node::updateBF(int num){
   BF = num;
}

template<typename T, typename K, typename C>
void AvlTree<T,K,C>::Node::updateHeight(int num){
   height = num;
}

template<typename T, typename K, typename C>
bool AvlTree<T,K,C>::Node::isLeaf()const{
   if(left==NULL & right == NULL)
    return true;
   else
    return false;
}

///////////////////////////////////////////implementation of AvlTree///////////////////////////////////////////////////


///////////////////Get///////////////////

template<typename T, typename K, typename C>
typename AvlTree<T, K, C>::Node* AvlTree<T, K, C>::getRoot() const {
	return avlRoot;
}

template<typename T, typename K, typename C>
typename AvlTree<T, K, C>::Node* AvlTree<T, K, C>::getFollow(Node* node) const {
	if (node->getRight() != NULL) {
		node = node->getRight();
		while (node->getLeft() != NULL) {
			node = node->getLeft();
		}
		return node;
	}
	if (avlRoot == node)
		return NULL;
	AvlTree<T, K, C>::Node* father = node->getParent();
	while (father->getLeft() != node) {
		node = node->getParent();
		father = node->getParent();
		if (father == NULL)
			return NULL;
	}
	return father;
}

/////////////////update AVL////////////

template<typename T, typename K, typename C>
void AvlTree<T,K,C>::updateNodeFather(Node* father, Node* son, Node* grandson){
  AvlTree<T,K,C>::Node* leftSon = father->getLeft();
  if(leftSon == NULL){
    father->updateRight(grandson);
  } else {
    int nodeToRemove = avlCompare(son->getKey(),leftSon->getKey(),son->getValue(),leftSon->getValue());
    if(nodeToRemove == 0){
     father->updateLeft(grandson);
    }else{
      father->updateRight(grandson);
    }
  }

}

///////////////////FIX/////////////////

template<typename T, typename K, typename C>
void AvlTree<T,K,C>::fixBalance(Node* node){
    if(node == NULL || node->getHeight() == 0){
        return;
    }
    AvlTree<T,K,C>::Node* OGroot= avlRoot;
    while(1){
      fixHeight(node);
      fixBF(node);
      fixRank(node);
      AvlTree<T,K,C>::Node* rightNode = node->getRight();
      AvlTree<T,K,C>::Node* leftNode = node->getLeft();
      if(node->getBF() == 2){
        if(leftNode->getBF()>=0){
          rotation_LL(node);
        }else{
          rotation_LR(node);
        }
      }else if(node->getBF()==-2){
        if(rightNode->getBF()>=0){
          rotation_RR(node);
        }else{
          rotation_RL(node);
        }
       }
       if(node == OGroot){
        fixHeight(node);
        fixBF(node);
        fixRank(node);
        break;
       }
       node = node->getParent();
    }
}


template<typename T, typename K, typename C>
void AvlTree<T,K,C>::fixHeight(Node* node){
  if(node == NULL){
    return;
  }
  node->updateHeight(nodeHeight(node)); // calculate the node height
  fixHeight(node->getParent());
}

template<typename T, typename K, typename C>
void AvlTree<T,K,C>::fixBF(Node* node){
  if(node == NULL){
    return;
  }
  node->updateBF(nodeHeight(node->getLeft())-nodeHeight(node->getRight()));
  fixBF(node->getParent());
}

template<typename T, typename K, typename C>
void AvlTree<T,K,C>::fixRank(Node* node){
  if(node == NULL){
    return ;
  }
  AvlTree<T,K,C>::Node* rightNode = node->getRight();
  AvlTree<T,K,C>::Node* leftNode = node->getLeft();
  int rightRank, leftRank;
  if(rightNode ==  NULL){
    rightRank =0;
  }else{
    rightRank = rightNode->getRank();
  }
  if(leftNode ==  NULL){
    leftRank =0;
  }else{
    leftRank = leftNode->getRank();
  }
  node->updateRank(rightRank+leftRank+1);
  fixRank(node->getParent());
}

template<typename T, typename K, typename C>
int AvlTree<T,K,C>::nodeHeight(Node* node) const{
  if(node==NULL){
    return -1;
  }
  int heightR, heightL, maxHeight;
  //calculate height
  if(node->getRight()==NULL){
    heightR = -1;
  }else{
    heightR = node->getRight()->getHeight();
  }
  if(node->getLeft()==NULL){
    heightL = -1;
  }else{
    heightL = node->getLeft()->getHeight();
  }
  if(heightL>=heightR){
    maxHeight=heightL;
  }else{
    maxHeight = heightR;
  }
  return ++maxHeight;
}

////////////////////rotations//////////

template<typename T, typename K, typename C>
void AvlTree<T,K,C>::rotation_RR(Node* node){
  	AvlTree<T, K, C>::Node* rightNode = node->getRight();
	AvlTree<T, K, C>::Node* rightNodeLeft = rightNode->getLeft();
	if (rightNodeLeft != NULL) {
		rightNodeLeft->updateParent(node);
	}
	node->updateRight(rightNodeLeft);
	rightNode->updateLeft(node);
	if (node->getParent() == NULL) {
		avlRoot = rightNode;
	} else {
		updateNodeFather(node->getParent(), node, rightNode);
	}
	rightNode->updateParent(node->getParent());
	node->updateParent(rightNode);
	fixHeight(node);
	fixBF(node);
	fixRank(node);
	fixHeight(rightNode);
	fixBF(rightNode);
}

template<typename T, typename K, typename C>
void AvlTree<T, K, C>::rotation_LL(Node* node) {
	AvlTree<T, K, C>::Node* leftNode = node->getLeft();
	AvlTree<T, K, C>::Node* leftNodeRight = leftNode->getRight();
	if (leftNodeRight != NULL) {
		leftNodeRight->updateParent(node);
	}
	node->updateLeft(leftNodeRight);
	leftNode->updateRight(node);
	if (node->getParent() == NULL) {
		avlRoot = leftNode;
	} else {
		updateNodeFather(node->getParent(), node, leftNode);
	}
	leftNode->updateParent(node->getParent());
	node->updateParent(leftNode);
	fixHeight(node);
	fixBF(node);
	fixRank(node);
	fixHeight(leftNode);
	fixBF(leftNode);
}


template<typename T, typename K, typename C>
void AvlTree<T, K, C>::rotation_RL(Node* node) {
	if (node == NULL) {
		return;
	}
	rotation_LL(node->getRight());
	rotation_RR(node);
}

template<typename T, typename K, typename C>
void AvlTree<T, K, C>::rotation_LR(Node* node) {
	if (node == NULL) {
		return;
	}
	rotation_RR(node->getLeft());
	rotation_LL(node);
}

///////////////////////////////////////////////////////////////////////////

template<typename T, typename K, typename C>
typename AvlTree<T, K, C>::Node* AvlTree<T, K, C>::recursive(const K& key, T* value, Node* node, int* index) const {
	K rootKey = node->getKey();
	T* rootValue = node->getValue();
	int compare = avlCompare(rootKey, key, rootValue, value);
	if (compare == EQUALS) {
		if (index != NULL) {
			AvlTree<T, K, C>::Node* leftSon = node->getLeft();
			int rankLeft;
			if(leftSon==NULL){
              rankLeft = 0;
			}else{
			  rankLeft = leftSon->getRank();
			}
			*index += rankLeft + 1;
		}
		return node;
	} else if (compare == IS_BIGGER) {
		      if(node->getLeft()==NULL){
                return node;
		      }else{
		        return recursive(key,value,node->getLeft(),index);
		      }
	} else {
		if (index == NULL) {
            if(node->getRight()==NULL){
              return node;
            }else{
              return recursive(key,value,node->getRight(),index);
            }
		} else {  //finding Rank
			if (node->getRight() == NULL) {
				return node;
			} else {
				AvlTree<T, K, C>::Node* leftSon = node->getLeft();
				int rankLeft;
				if(leftSon==NULL){
                  rankLeft = 0;
				}else{
				  rankLeft = leftSon->getRank();
				}
				*index += rankLeft + 1;
				return recursive(key, value, node->getRight(), index);
			}
		}
	}
}

template<typename T, typename K, typename C>
typename AvlTree<T, K, C>::Node* AvlTree<T, K, C>::find(const K& key, T* value, bool* wasFound) const {
	if (avlRoot == NULL) {
		return NULL;
	}
	AvlTree<T, K, C>::Node* result = recursive(key, value, avlRoot, NULL);
	int compare = avlCompare(result->getKey(), key, result->getValue(), value);
	if(compare == EQUALS){
      *wasFound = true;
	}else{
	  *wasFound = false;
	}
	return result;
}

template<typename T, typename K, typename C>
void AvlTree<T, K, C>::insert(const K& key, T* value) {
	bool wasFound;
	AvlTree<T, K, C>::Node* result = find(key, value, &wasFound);
	//if find function return NULL it means that the node is the AvlRoot
	if (result == NULL) {
		AvlTree<T, K, C>::Node* newNode = new Node(key, value);
		avlRoot = newNode;
		avlBiggest = newNode;
		return;
	}
	//we need to make sure the the node is equal
	int compare = avlCompare(result->getKey(), key, result->getValue(), value);
	if (compare == EQUALS) {
		return;
	}

	AvlTree<T, K, C>::Node* newNode = new Node(key, value);
	if(compare == IS_BIGGER){
      result->updateLeft(newNode);
	}else{
	  result->updateRight(newNode);
	}
	newNode->updateParent(result);
	fixBalance(newNode);
	AvlTree<T, K, C>::Node* biggestNode = getBiggestNode();
	compare = avlCompare(key, biggestNode->getKey(), value,biggestNode->getValue());
	if (compare == IS_BIGGER) {
		avlBiggest = newNode;
	}
}

template<typename T, typename K, typename C>
typename AvlTree<T, K, C>::Node* AvlTree<T, K, C>::getBiggestNode() const {
	return avlBiggest;
}

////////////////////////////////////////////removes////////////////////////////

template<typename T, typename K, typename C>
void AvlTree<T, K, C>::removeLeaf(Node* result) {
	AvlTree<T, K, C>::Node* father = result->getParent();
	if (father == NULL) {
		avlRoot = NULL;
		delete result;
		return;
	}
	updateNodeFather(father, result, NULL);
	delete result;
	fixBalance(father);
	return;
}


template<typename T, typename K, typename C>
void AvlTree<T, K, C>::removeOneSon(Node* result) {
	AvlTree<T, K, C>::Node* father = result->getParent();
	AvlTree<T, K, C>::Node* resultLeft = result->getLeft();
	AvlTree<T, K, C>::Node* resultRight = result->getRight();
	AvlTree<T, K, C>::Node* resultOnlySon;
	if(resultLeft==NULL){
      resultOnlySon = resultRight;
	}else{
	  resultOnlySon = resultLeft;
	}
	if (father == NULL) {
		avlRoot = resultOnlySon;
		resultOnlySon->updateParent(NULL);
	} else {
		updateNodeFather(father, result, resultOnlySon);
		resultOnlySon->updateParent(father);
		fixBalance(resultOnlySon);
	}
	delete result;
	return;
}


template<typename T, typename K, typename C>
void AvlTree<T, K, C>::removeTwoSons(Node* result) {
	AvlTree<T, K, C>::Node* resultFollowing = getFollow(result);
	K tempKey = result->getKey();
	T* tempValue = result->getValue();
	if (resultFollowing == avlBiggest) {
		avlBiggest = result;
	}
	result->updateKey(resultFollowing->getKey());
	result->updateValue(resultFollowing->getValue(), false);
	resultFollowing->updateKey(tempKey);
	resultFollowing->updateValue(tempValue, false);
	AvlTree<T, K, C>::Node* removedFather = resultFollowing->getParent();
	result = resultFollowing;
	remove_aux(result->getKey(), result->getValue(), result);
	fixBalance(removedFather);
}

template<typename T, typename K, typename C>
void AvlTree<T, K, C>::remove_aux(const K& key, T* value ,AvlTree<T, K, C>::Node* result) {
	AvlTree<T, K, C>::Node* resultLeft = result->getLeft();
	AvlTree<T, K, C>::Node* resultRight = result->getRight();
	if (result->isLeaf()) {
		removeLeaf(result);
	} else if (!(resultLeft != NULL && resultRight != NULL)) {
		removeOneSon(result);
	} else {
		removeTwoSons(result);
	}
}
template<typename T, typename K, typename C>
void AvlTree<T, K, C>::remove(const K& key, T* data) {
	bool wasFound;
	AvlTree<T, K, C>::Node* result = find(key, data, &wasFound);
	if (result == NULL) {
		return;
	}
	int compare = avlCompare(result->getKey(), key, result->getValue(), data);
	if (compare != EQUALS) {
		return;
	}

	if (avlCompare(key, avlBiggest->getKey(), data, avlBiggest->getValue())
			== EQUALS) {
		if (result != avlRoot) {
			avlBiggest =
					result->getLeft() == NULL ?
							result->getParent() : result->getLeft();
		} else {
			avlBiggest = result->getLeft() == NULL ? NULL : result->getLeft();
		}
	}
	remove_aux(key, data, result);
}

template<typename T, typename K, typename C>
typename AvlTree<T, K, C>::Node* AvlTree<T, K, C>::copy(
		AvlTree<T, K, C>::Node* treeOriginal,
		AvlTree<T, K, C>::Node* parent) {
	if (treeOriginal == NULL) {
		return NULL;
	}
	Node* treeNew = new AvlTree<T, K, C>::Node(treeOriginal->getKey(),
			treeOriginal->getValue());
	treeNew->updateBF(treeOriginal->getBF());
	treeNew->updateHeight(treeOriginal->getHeight());
	treeNew->updateParent(parent);
	treeNew->updateLeft(copy(treeOriginal->getLeft(), treeNew));
	treeNew->updateRight(copy(treeOriginal->getRight(), treeNew));
	return treeNew;
}

template<typename T, typename K, typename C, typename F>
class reverseInOrder {
public:
	void operator()(typename AvlTree<T, K, C>::Node* root, F& func) {
		if (root == NULL) {
			return;
		}
		this->operator()(root->getRight(), func);
		//bool check =
		func(root);
		//if (check != false) {
		this->operator()(root->getLeft(), func);
		//}
	}

};

template<typename T, typename K, typename C, typename F>
class inOrder {
public:
	void operator()(typename AvlTree<T, K, C>::Node* root, F& func) {
		if (root == NULL) {
			return;
		}
		this->operator()(root->getLeft(), func);
		func(root);
		this->operator()(root->getRight(), func);
	}

};

/*
template<typename T, typename K, typename C>
typename AvlTree<T, K, C>::Node* AvlTree<T, K, C>::buildFullTree(int height) {
	if (height == -1) {
		return NULL;
	}
	T* newData = new T;
	Node* right = buildFullTree(height - 1);
	Node* left = buildFullTree(height - 1);
	Node* father = new Node(0, newData);
	father->updateRight(right);
	father->updateLeft(left);
	if (left != NULL)
		left->updateParent(father);
	if (right != NULL)
		right->updateParent(father);
	delete newData;
	return father;
}

template<typename T, typename K, typename C>
void AvlTree<T, K, C>::buildEmptyTree(int n) {
	int height = 0;
	int sum = 1;
	while (sum < n + 1) {
		height++;
		sum *= 2;
	}
	height--;
	root = buildFullTree(height);
	int leavesToRemove = sum - 1 - n;
	removeLeaves<T, K, C> remove(leavesToRemove);
	reversedInOrder2<T, K, C, removeLeaves<T, K, C> > tour;
	tour(root, remove);
	updateBiggestJunction(findBiggestJunction());
	updateEmptyTreeHeight<T, K, C> fixHeight;
	postOrder<T, K, C, updateEmptyTreeHeight<T, K, C> > fix;
	fix(root, fixHeight);

	updateEmptyTreeBalance<T, K, C> fixBalance;
	postOrder<T, K, C, updateEmptyTreeBalance<T, K, C> > fixBF;
	fixBF(root, fixBalance);

	updateEmptyTreeRank<T, K, C> fixRank;
	postOrder<T, K, C, updateEmptyTreeRank<T, K, C> > fixR;
	fixR(root, fixRank);
}

*/



#endif // AVLTREE_H
