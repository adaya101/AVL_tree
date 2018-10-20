#include <stdlib.h>
#include <iostream>
#include <exception>
#include "systemNew.h"

using std::cout;
using std::endl;


void* Init(int N){
  if(N<=0){
    return NULL;
  }
  System* DS;
  try{
      DS  = new System(N);
  }catch (std::bad_alloc& e){
    return NULL;
  }
  return (void*)DS;
}

StatusType PlantTree(void *DS, int i, int j){
 if(DS == NULL){
    return INVALID_INPUT;
 }
 StatusType result;
 try{
   result = ((System*)DS)->PlantTree(i,j);
 }catch(std::bad_alloc& e){
   return ALLOCATION_ERROR;
 }
 return result;
}

StatusType AddFruit(void *DS, int i, int j, int fruitID, int ripeRate){
  if(DS ==NULL){
   return INVALID_INPUT;
  }
   StatusType result;

  try{
   result = ((System*)DS)->AddFruit(i,j,fruitID,ripeRate);
  }catch(std::bad_alloc& e){
   return ALLOCATION_ERROR;
 }
 return result;
}


StatusType PickFruit(void *DS, int fruitID){
  if(DS==NULL){
        return INVALID_INPUT;
     }
  StatusType result;
   try{
     result = ((System*)DS)->PickFruit(fruitID);
   }catch(std::bad_alloc& e){
     return ALLOCATION_ERROR;
   }
   return result;
}

StatusType RateFruit(void *DS, int fruitID, int ripeRate){
  if(DS==NULL){
        return INVALID_INPUT;
     }
  StatusType result;
   try{
     result = ((System*)DS)->RateFruit(fruitID,ripeRate);
   }catch(std::bad_alloc& e){
     return ALLOCATION_ERROR;
   }
   return result;
}

StatusType GetBestFruit(void *DS, int i, int j, int *fruitID){
  if(DS==NULL){
        return INVALID_INPUT;
     }
  StatusType result;
   try{
     result = ((System*)DS)->GetBestFruit(i,j,fruitID);
   }catch(std::bad_alloc& e){
     return ALLOCATION_ERROR;
   }
   return result;
}

StatusType GetAllFruitsByRate(void *DS, int i, int j, int **fruits, int *numOfFruits){
  if(DS==NULL||fruits == NULL|| numOfFruits ==NULL ){
        return INVALID_INPUT;
     }
  StatusType result;
   try{
     result = ((System*)DS)->GetAllFruitsByRate(i,j,fruits,numOfFruits);
   }catch(std::bad_alloc& e){
     return ALLOCATION_ERROR;
   }
   return result;

}

StatusType UpdateRottenFruits(void *DS, int rottenBase, int rottenFactor){
  if(DS==NULL||rottenBase<1|| rottenFactor<1 ){
        return INVALID_INPUT;
     }
  StatusType result;
   try{
     result = ((System*)DS)->UpdateRottenFruits(rottenBase,rottenFactor);
   }catch(std::bad_alloc& e){
     return ALLOCATION_ERROR;
   }
   return result;

}

void Quit(void** DS){
  if(*DS){
    delete ((System*)*DS);
    (*DS) = NULL;
  }
}
