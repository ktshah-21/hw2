#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "user.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "mydatastore.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product*>& hits);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;
    std::map<std::string, std::deque<Product*>> cartMapping;


    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  AND term term ...                  " << endl;
    cout << "  OR term term ...                   " << endl;
    cout << "  ADD username search_hit_number     " << endl;
    cout << "  VIEWCART username                  " << endl;
    cout << "  BUYCART username                   " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "====================================" << endl;

    vector<Product*> hits ={};
    bool done = false;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND") {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                displayProducts(hits);
            }
            else if ( cmd == "OR" ) {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                displayProducts(hits);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }else if(cmd == "VIEWCART"){
              string uname;
              if(ss>>uname){
                vector<User*> usersData = ds.users();
                int check=0;
                for(unsigned int i=0; i<usersData.size(); i++){
                  if(usersData[i]->getName()==uname){
                    std::map<std::string, std::deque<Product*>>::iterator it = cartMapping.find(uname);
                    deque<Product*> cartDetails;
                    if(it != cartMapping.end()){
                      cartDetails = it->second;
                    }
                    for(unsigned int i=0; i<cartDetails.size(); i++){
                      cout << "Hit " << setw(3) << (i+1) << endl;
                      cout<<cartDetails[i]->displayString() << endl; 
                    }
                    check=1;
                    break;
                  }
                }
                if(check==0){
                  cout<<"Invalid username"<<endl;
                }
              }
            }else if(cmd == "ADD"){
              string uname;
              if(ss>>uname){
                User* user_ = nullptr;
                vector<User*> usersData = ds.users();
                for(unsigned int i=0; i<usersData.size(); i++){
                  if(usersData[i]->getName()==uname){
                    user_ = usersData[i];
                    break;
                  }
                }
                if(user_ == nullptr){
                  cout<<"Invalid request"<<endl;
                }else{
                  unsigned int hitIndex =0;
                  if(ss>>hitIndex){
                    if(hitIndex<=hits.size()&&hitIndex>0){
                      std::map<std::string, std::deque<Product*>>::iterator search = cartMapping.find(uname);
                      if(search!=cartMapping.end()){
                        search->second.push_back(hits[hitIndex-1]);
                      }else{
                        std::deque<Product*> productMapping;
                        productMapping.push_back(hits[hitIndex-1]);
                        cartMapping.insert(std::make_pair(uname, productMapping));
                      }
                      //cout<<"SUCESSFULLY ADDED TO CART"<<endl;
                    }else{
                      cout<<"Invalid request"<<endl;
                    }
                  }else{
                    cout<<"Invalid request"<<endl;
                  }
                }
              }
            }else if(cmd=="BUYCART"){
              string uname;
              if(ss>>uname){
                User* user_ = nullptr;
                vector<User*> usersData = ds.users();
                for(unsigned int i=0; i<usersData.size(); i++){
                  if(usersData[i]->getName()==uname){
                    user_ = usersData[i];
                    break;
                  }
                }
                if(user_ == nullptr){
                  cout<<"Invalid Username"<<endl;
                }else{
                  //double totalCost = 0;
                  std::deque<Product*> workingCart;
                  std::map<std::string, std::deque<Product*>>::iterator it = cartMapping.find(uname);
                    //vector<Product*> cartDetails;
                  if(it != cartMapping.end()){
                    workingCart = it->second;
                  }
                  int size__ = workingCart.size();
                  int counter =0;
                  while(!workingCart.empty() && counter<size__){   
                    Product* pp = workingCart.front();
                    if(user_->getBalance() >= pp->getPrice()&&pp->getQty()>0){
                      user_->deductAmount(pp->getPrice());
                      pp->subtractQty(1);
                      workingCart.pop_front();
                      size__--;
                    }else{
                      workingCart.push_back(pp);
                      workingCart.pop_front(); 
                      counter++;
                      //cout<<workingCart.size()<<endl;
                    }
                  }
                  it->second = workingCart;
                }
                
              }
            }
	    /* Add support for other commands here */




            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    if (hits.begin() == hits.end()) {
    	cout << "No results found!" << endl;
    	return;
    }
    std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        //cout<<"SHITE"<<endl;
        cout << (*it)->displayString() << endl;
        //cout<<"The second SHITE"<<endl;
        cout << endl;
        resultNo++;
        //cout<<"hhs"<<endl;
    }
    // for(unsigned int i=0; i<hits.size(); i++) {
    //     cout << "Hit " << setw(3) << resultNo << endl;
    //     //cout<<"SHITE"<<endl;
    //     cout << hits[i]->displayString() << endl;
    //     //cout<<"The second SHITE"<<endl;
    //     cout << endl;
    //     resultNo++;
    //     //cout<<"hhs"<<endl;
    // }
}
