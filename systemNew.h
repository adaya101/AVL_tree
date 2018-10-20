
#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include "treeNew.h"
#include "library1.h"
#include "AvlTreeNew.h"
#include <stdlib.h>
#include <iostream>
/*
class switchData {
	int index;
	Fruit** data;
public:
	switchData(Fruit** dataArray) :
			index(0), data(dataArray) {
	}
	;
	void operator()(AvlTree<Free, int, compareFruitID>::Node* node) {
		node->updateKey((data[index])->getRipeRate());
		Fruit* newFruit = new Fruit;
		newFruit->updateFruitID((data[index])->getID());
		newFruit->updateRipeRate((data[index])->getRipeRate());
        newFruit->updateRow((data[index])->getRow());
		newFruit->updateCol((data[index])->getCol());
		node->updtaeValue(newFruit, true);
		index++;
	}
};

*/
class compareTree{
  public:
    int operator()(int i1,int i2,Tree* value1,Tree* value2)const{
      if(i1>i2)
      {
        return 1;
      }else if(i1<i2)
      {
        return -1;
      }else
      {
        if(value1->getCol()>value2->getCol()){
          return 1;
        }else if(value1->getCol()<value2->getCol()){
          return -1;
        }
      }
      return 0;
    }
};


class CopyFruitsToArray {
	int index1; //the fruits that complete the if
	int index2;//the fruits that don't complete the if
	Fruit **fruitsArray1;
	Fruit **fruitsArray2;
	int rottenBase;


public:
	CopyFruitsToArray(Fruit **fruits1,Fruit **fruits2,int rottenBase) :
			index1(0),index2(0), fruitsArray1(fruits1),fruitsArray2(fruits2),rottenBase(rottenBase) {
	}
	void operator()(AvlTree<Fruit, int, compareFruitRipe>::Node* node) {
	    //int node->get
	    if(node->getValue()->getID()%rottenBase == 0){
          Fruit* fruitToAdd = node->getValue();
		  fruitsArray1[index1++] = fruitToAdd;
	    }else{
          Fruit* fruitToAdd = node->getValue();
		  fruitsArray2[index2++] = fruitToAdd;
	    }
	}
};

class CopyFruitsIdToArrays {
	int index;
	int* fruits;

public:
	CopyFruitsIdToArrays(int* fruits) :
			index(0), fruits(fruits) {
	}
	;
	void operator()(AvlTree<Fruit, int, compareFruitRipe>::Node* node) {
		Fruit* fruitToAdd = node->getValue();
		int value = fruitToAdd->getID();
		fruits[index++] = value;
	}
};

class fruitLess {
public:
	bool operator()(Fruit a, Fruit b) {
		return a.getRipeRate() < b.getRipeRate()|| (a.getRipeRate() == b.getRipeRate()
						&& a.getID() > b.getID());
	}
};


void merge(Fruit **array_1, int size_1, Fruit **array_2, int size_2,
		Fruit** mergedArray, int size_merged) {
	int index_1 = 0;
	int index_2 = 0;
	for (int i = 0; i < size_merged; i++) {
		if (index_1 == size_1) {
			mergedArray[i] = array_2[index_2++];
			continue;
		} else if (index_2 == size_2) {
			mergedArray[i] = array_1[index_1++];
			continue;
		} else {
			fruitLess compare;
			Fruit* app =
					compare(*array_1[index_1], *array_2[index_2]) ?
							array_1[index_1++] : array_2[index_2++];
			mergedArray[i] = app;
		}

	}
}


class System{
public:
  System(int N):numOfTrees(0),numOfFruits(0),edge(N)
  {
    compareTree compareT;
    compareFruitID compareID;
    Orchard = new AvlTree<Tree,int,compareTree>(compareT);
    fruitTree = new AvlTree<Fruit,int,compareFruitID>(compareID);
  }

  ~System(){
     delete Orchard;
     delete fruitTree;
  }

  StatusType PlantTree(int i, int j);

  void fruitAddToAll(Fruit*,Tree*);

  StatusType AddFruit(int i, int j, int fruitID, int ripeRate);
  StatusType PickFruit(int fruitID);
  StatusType RateFruit(int fruitID, int ripeRate);
  StatusType GetBestFruit(int i, int j, int *fruitID);
  StatusType GetAllFruitsByRate(int i, int j, int **fruits, int *numOfFruits);
  StatusType UpdateRottenFruits(int rottenBase, int rottenFactor);

  void UpdateRotten(int rottenBase, int rottenFactor,AvlTree<Fruit,int,compareFruitRipe>* theFruitTree);




private:
  int numOfTrees;
  int numOfFruits;
  int edge;
  AvlTree<Tree,int,compareTree>* Orchard;
  AvlTree<Fruit,int,compareFruitID>* fruitTree;

};

//////////////////////////////////////////////////////////////



StatusType System::PlantTree(int i, int j)
{
  if(i>=edge || i<0 || j>=edge || j<0){
    return INVALID_INPUT;
  }
  Tree* newTree = new Tree(i,j);
  bool wasFound = false;
  Orchard->find(i,newTree,&wasFound);
  if(wasFound){
    delete newTree;
    return FAILURE;
  }
  Orchard->insert(i,newTree);
  return SUCCESS;
}


void System::fruitAddToAll(Fruit* fruit,Tree* tree){

  tree->getFruitByID()->insert(fruit->getID(),fruit);
  tree->updateFruitsNum(1);
  tree->getFruitByRipe()->insert(fruit->getRipeRate(),fruit);
  fruitTree->insert(fruit->getID(),fruit);
}


StatusType System::AddFruit(int i, int j, int fruitID, int ripeRate)
{
    if(i<0 || i>edge-1 || j>edge-1 || j<0 || fruitID <=0 || ripeRate <=0)
    {
        return INVALID_INPUT;
    }
    bool wasFound = false;
    Tree newTree(i,j);
    AvlTree<Tree,int,compareTree>::Node* theFoundTree = Orchard->find(i,&newTree,&wasFound);
    //if there is no tree in the hole
    if(wasFound == false)
    {
//        delete newTree;
        return FAILURE;
    }
    Tree* tree = theFoundTree->getValue();
    Fruit* newFruit = new Fruit(fruitID,ripeRate,i,j);
    wasFound = false;
    //create a new node
    fruitTree->find(fruitID,newFruit,&wasFound);
    if(wasFound == true)
    {
        delete newFruit;
        return FAILURE;
    }
    fruitAddToAll(newFruit,tree);
    this->numOfFruits++;

    return SUCCESS;
}


StatusType System::PickFruit(int fruitID){
  if(fruitID<=0){
    return INVALID_INPUT;
  }
  bool wasFound = false;
  Fruit* newFruit = new Fruit(fruitID);
  AvlTree<Fruit,int,compareFruitID>::Node* theFoundFruit = fruitTree->find(fruitID,newFruit,&wasFound);
  Fruit* fruit = theFoundFruit->getValue();
  delete newFruit;
  if(wasFound==false){
    return FAILURE;
  }
  wasFound=false;
  Tree* tree = new Tree(fruit->getRow(),fruit->getCol());
  AvlTree<Tree,int,compareTree>::Node* theFoundTree = Orchard->find(fruit->getRow(),tree,&wasFound);
  fruitTree->remove(fruitID,fruit);
  Tree* t = theFoundTree->getValue();
  t->getFruitByID()->remove(fruitID,fruit);
  t->updateFruitsNum(-1);
  //getValue()->getFruitByID->remove(fruitID,fruit);
  t->getFruitByRipe()->remove(fruitID,fruit);
  delete tree;
  this->numOfFruits--;
  return SUCCESS;
}


StatusType System::RateFruit(int fruitID, int ripeRate)
{
    if(fruitID<=0 || ripeRate<=0)
    {
        return INVALID_INPUT;
    }

    bool wasFound = false;
    Fruit newFruit(fruitID);
    AvlTree<Fruit,int,compareFruitID>::Node* theFoundFruit = fruitTree->find(fruitID,&newFruit,&wasFound);
    if(wasFound==false)
    {
        return FAILURE;
    }

    Fruit* fruit = theFoundFruit->getValue();
    wasFound=false;
    Tree tree(fruit->getRow(),fruit->getCol());

    AvlTree<Tree,int,compareTree>::Node* theFoundTree = Orchard->find(fruit->getRow(),&tree,&wasFound);
    if(wasFound)
    {
        Tree* t = theFoundTree->getValue();
        t->getFruitByRipe()->remove(fruit->getRipeRate(),fruit);
        t->getFruitByID()->remove(fruitID,fruit);


        fruit->updateRipeRate(ripeRate);

        wasFound=false;
        t->getFruitByRipe()->insert(ripeRate,fruit);
        t->getFruitByID()->insert(fruitID,fruit);

  }
//    delete tree;

    return SUCCESS;
}

StatusType System::GetBestFruit(int i, int j, int *fruitID){
  if(fruitID==NULL || i<0 || i>edge-1 || j>edge-1 || j<0){
    return INVALID_INPUT;
  }
  bool wasFound = false;
  Tree* newTree = new Tree(i,j);
  AvlTree<Tree,int,compareTree>::Node* theFoundTree = Orchard->find(i,newTree,&wasFound);

  if(wasFound==false){
    return FAILURE;
  }
  Tree* tree = theFoundTree->getValue();
  if(tree->getNumOfFruits()==0){
    *fruitID = -1;
    return SUCCESS;
  }

  AvlTree<Fruit,int,compareFruitRipe>* fruits = tree->getFruitByRipe();
  *fruitID = fruits->getBiggestNode()->getValue()->getID();
  delete newTree;

  return SUCCESS;
}

StatusType System::GetAllFruitsByRate(int i, int j, int **fruits, int *numOfFruits){
  if(fruits==NULL|| numOfFruits==NULL || i<0 || i>edge-1 || j>edge-1 || j<0){
    return INVALID_INPUT;
  }
  bool wasFound = false;
  Tree* newTree = new Tree(i,j);
  AvlTree<Tree,int,compareTree>::Node* theFoundTree = Orchard->find(i,newTree,&wasFound);
  delete newTree;
  if(wasFound==false){
    return FAILURE;
  }
  *numOfFruits = theFoundTree->getValue()->getNumOfFruits();
  if(*numOfFruits==0){
    *fruits = NULL;
    return SUCCESS;
  }
  *fruits = (int*)malloc(sizeof(int)*(*numOfFruits));
  CopyFruitsIdToArrays copy(*fruits);
  reverseInOrder<Fruit,int,compareFruitRipe,CopyFruitsIdToArrays> tour;
  Tree* t = theFoundTree->getValue();
  AvlTree<Fruit,int,compareFruitRipe>* fruitss = t->getFruitByRipe();
  tour(fruitss->getRoot(),copy);
  return SUCCESS;
}
/*
void System::UpdateRotten(int rottenBase, int rottenFactor,Tree* fruitTree1){
  int n = fruitTree1->getNumOfFruits();
  Fruit** fruits1 = new Fruit*[n];
  Fruit** fruits2 = new Fruit*[n];
  CopyFruitsToArray copy(fruits1,fruits2,rottenBase);
  reverseInOrder<Fruit,int,compareFruitRipe,CopyFruitsToArrays> tour;
  tour(fruitTree1->getFruitByRipe()->getRoot(),copy);
  //now we will multiply the fruits that completed the rotten base condition
  for(int i=0;i<n;i++){
    fruits1[i]->updateRipeRate(fruits1[i]->getRipeRate()*rottenFactor);
  }

  //creating a merged Array of the total new updated fruits
  Fruit** mergedArray = new Fruit*[n];
  merge(fruits1,n,fruits2,n,mergedArray,n);
  switchData fillTree(mergedArray);
  inOrder<Fruit,int,compareFruitRipe,switchData> tourFill;
  compareFruitRipe compare1;
  AvlTree<Fruit,int,compareFruitRipe>* theFullTree = new AvlTree<Fruit,int,compareFruitRipe>(compare1);



  return SUCCESS;
};

*/
StatusType System::UpdateRottenFruits(int rottenBase, int rottenFactor){
  if(rottenBase < 1 || rottenFactor < 1){
    return INVALID_INPUT;
  }

  return SUCCESS;
};

#endif // SYSTEM_H_INCLUDED
