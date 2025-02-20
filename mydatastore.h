#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <string>
#include <set>
#include <map>
#include <vector>
#include <deque>
#include "product.h"
#include "book.h"
#include "clothing.h"
#include "movie.h"
#include "user.h"
#include "util.h"
#include "datastore.h"

/**
 * MyDataStore Interface needed for parsing and instantiating products and users
 *
 * A derived version of the MyDataStore can provide other services as well but
 * must support those below
 *
 * DO NOT EDIT
 */


class MyDataStore : public DataStore {
public:
    ~MyDataStore();

    /**
     * Adds a product to the data store
     */
    void addProduct(Product* p);

    /**
     * Adds a user to the data store
     */
    void addUser(User* u);

    std::vector<User*> users() const;

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
    std::vector<Product*> search(std::vector<std::string>& terms, int type);

    /**
     * Reproduce the database file from the current Products and User values
     */
    void dump(std::ostream& ofile);
private:
  std::vector<Product*> products_;
  std::vector<User*> users_;
  std::map<std::string, std::set<Product*>> searchMapping;
  std::map<std::string, std::deque<Product*>> cartMapping;
};

#endif
