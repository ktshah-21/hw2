#include "mydatastore.h"
#include <iostream>

/**
 * MyDataStore Interface needed for parsing and instantiating products and users
 *
 * A derived version of the MyDataStore can provide other services as well but
 * must support those below
 *
 * DO NOT EDIT
 */

MyDataStore::~MyDataStore(){
    for(size_t i=0; i<users_.size(); i++){
      delete users_[i];
    }
    for(size_t i=0; i<products_.size(); i++){
      delete products_[i];
    }
    searchMapping.clear();
}


void MyDataStore::addProduct(Product* p){
  products_.push_back(p);
  std::set<std::string>::iterator it;
  std::set<std::string> keyWordData = p->keywords();
  for(it=keyWordData.begin(); it != keyWordData.end(); ++it){
    std::map<std::string, std::set<Product*>>::iterator search = searchMapping.find(*it);
    if(search!=searchMapping.end()){
      search->second.insert(p);
    }else{
      std::set<Product*> productMapping;
      productMapping.insert(p);
      searchMapping.insert(std::make_pair(*it, productMapping));
    }
  }
}

/**
  * Adds a user to the data store
  */
void MyDataStore::addUser(User* u){
  users_.push_back(u);
}

/**
  * Performs a search of products whose keywords match the given "terms"
  *  type 0 = AND search (intersection of results for each term) while
  *  type 1 = OR search (union of results for each term)
  */
std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type){
  if(type==0){
    int beg = 0;
    std::set<Product*> productSet;
    for(int i=0; i<terms.size(); i++){
      std::map<std::string, std::set<Product*>>::iterator search = searchMapping.find(terms[i]);
      if(search!=searchMapping.end()){
        if(beg==0){
          productSet = (search->second);
          beg=1;
        }else{
          productSet = setIntersection(productSet, (search->second));
        }
      }
    }
    std::vector<Product*> retVal;
    for(std::set<Product*>::iterator it = productSet.begin(); it!=productSet.end(); ++it){
      retVal.push_back(*it);
    }
    return retVal;
  }else{
    int beg = 0;
    std::set<Product*> productSet;
    for(int i=0; i<terms.size(); i++){
      std::map<std::string, std::set<Product*>>::iterator search = searchMapping.find(terms[i]);
      if(search!=searchMapping.end()){
        if(beg==0){
          productSet = (search->second);
          beg=1;
        }else{
          productSet = setUnion(productSet, (search->second));
        }
      }
    }
    std::vector<Product*> retVal;
    for(std::set<Product*>::iterator it = productSet.begin(); it!=productSet.end(); ++it){
      retVal.push_back(*it);
    }
    return retVal;
  }
}

/**
  * Reproduce the database file from the current Products and User values
  */
void MyDataStore::dump(std::ostream& ofile){
  ofile<<"<products>"<<std::endl;
  if(products_.size()>0){
    
    for(unsigned int i=0; i<products_.size(); i++){
      products_[i]->dump(ofile);
    }
   
  }
  ofile<<"</products>"<<std::endl;
  ofile<<"<users>"<<std::endl;
  if(users_.size()>0){
    
    for(unsigned int i=0; i<users_.size(); i++){
      users_[i]->dump(ofile);
    }
   
  }
  ofile<<"</users>"<<std::endl;
}

std::vector<User*> MyDataStore::users() const{
  return users_;
}

