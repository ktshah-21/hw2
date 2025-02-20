#include <sstream>
#include <iomanip>
#include "clothing.h"

using namespace std;

Clothing::Clothing(const std::string category, const std::string name, double price, int qty, const std::string size, const std::string brand) :
    Product(category, name, price, qty )
{
  size_ = size;
  brand_ = brand;
  keywords_ = parseStringToWords(name);
  std::set<string> brand_words = parseStringToWords(brand_);
  keywords_ = setUnion(keywords_, brand_words);
}

Clothing::~Clothing()
{
  keywords_.clear();

}

std::set<std::string> Clothing::keywords() const{
  return keywords_;
}

std::string Clothing::displayString() const{
  std::string retVal = name_ + "\nSize: " + size_ + " Brand: " + brand_ + "\n" + std::to_string(price_) + " " + std::to_string(qty_) + " left.";
  return retVal;
}


/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Clothing::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}

void Clothing::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << size_ << "\n" << brand_<<endl;
}



