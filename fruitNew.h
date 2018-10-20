#ifndef FRUIT_H_INCLUDED
#define FRUIT_H_INCLUDED

class Fruit{

public:
	Fruit(int ID, int ripe = 1, int i = 0 , int j = 0):fruitID(ID),ripeRate(ripe),treeRow(i),treeCol(j){};

//////////////////GET//////////////////////
	int getID()const{return fruitID;};
	int getRow()const{return treeRow;};
	int getCol()const{return treeCol;};
	int getRipeRate()const{return ripeRate;};

//////////////////////update///////////////////////
	void updateRipeRate(int rate){ripeRate=rate;};
	void updateID(int id){fruitID = id;};
	void updateRow(int i){treeRow = i;};
	void updateCol(int j){treeCol = j;};

private:
	int fruitID;
	int ripeRate;
	int treeRow;
	int treeCol;
};

#endif // FRUIT_H_INCLUDED
