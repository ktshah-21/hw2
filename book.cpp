#include <sstream>
#include <iomanip>
#include "book.h"

using namespace std;

Book::Book(const std::string category, const std::string name, double price, int qty, const std::string author, const std::string isbn) :
    Product(category, name, price, qty)
{
  author_ = author;
  isbn_ = isbn;
  keywords_ = parseStringToWords(name);
  std::set<string> author_words = parseStringToWords(author_);
  // std::set<string> isbn_data;
  // isbn_data.insert(convToLower(isbn_));
  keywords_ = setUnion(keywords_, author_words);
  keywords_.insert(isbn_);
}

Book::~Book()
{
  keywords_.clear();

}

std::set<std::string> Book::keywords() const{
  return keywords_;
}

std::string Book::displayString() const{
  std::string retVal = name_ + "\nAuthor: " + author_ + " ISBN: " + isbn_ + "\n" + std::to_string(price_) + " " + std::to_string(qty_) + " left.";
  return retVal;
}


/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Book::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}

void Book::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << isbn_ <<"\n" << author_ <<endl;
}



