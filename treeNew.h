#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include "AvlTreeNew.h"
#include "fruitNew.h"
typedef class Fruit Fruit;

class compareFruitID{
  public:
	int operator()(int i1,int i2,Fruit* value1,Fruit* value2)const{
	if(i1>i2)
	{
		return 1;
	}else if(i1<i2)
	{
		return -1;
	}
	return 0;
	}
};

class compareFruitRipe{
  public:
	int operator()(int i1,int i2,Fruit* value1,Fruit* value2)const{
	if(i1<i2)
	{
		return 1;
	}else if(i1>i2){
		return -1;
	}else {
		if(value1->getID()<value2->getID()){
			return 1;
		}else if(value1->getID()>value2->getID()){
			return -1;
		}
	}
	return 0;
	}
};

class Tree{

public:
	Tree(int i, int j):row(i),col(j),numOfFruits(0)
	{
		compareFruitID compareID;
		fruitsByID = new AvlTree<Fruit,int,compareFruitID>(compareID);
		compareFruitRipe compareRipe;
		fruitsByRipe = new AvlTree<Fruit,int,compareFruitRipe>(compareRipe);
	};
	~Tree(){
    delete[] fruitsByID;
    delete[] fruitsByRipe;
	};

	AvlTree<Fruit,int,compareFruitID>* getFruitByID()const{return fruitsByID;};
	AvlTree<Fruit,int,compareFruitRipe>* getFruitByRipe()const{return fruitsByRipe;};

	int getRow()const{return row;};
	int getCol()const{return col;};
	int getNumOfFruits()const{return numOfFruits;};
	void updateFruitsNum(int n){numOfFruits+=n;};

  private:
	int row;
	int col;
	int numOfFruits;
	AvlTree<Fruit,int,compareFruitID>* fruitsByID;
	AvlTree<Fruit,int,compareFruitRipe>* fruitsByRipe;
};

#endif // TREE_H_INCLUDED