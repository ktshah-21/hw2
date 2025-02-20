#include <sstream>
#include <iomanip>
#include "movie.h"

using namespace std;

Movie::Movie(const std::string category, const std::string name, double price, int qty, const std::string genre, const std::string rating) :
    Product(category, name, price, qty)
{
  genre_ = genre;
  rating_ = rating;
  keywords_ = parseStringToWords(name);
  keywords_.insert(convToLower(genre_));
}

Movie::~Movie()
{
  keywords_.clear();

}

std::set<std::string> Movie::keywords() const{
  return keywords_;
}

std::string Movie::displayString() const{
  std::string retVal = name_ + "\nGenre: " + genre_ + " Rating: " + rating_ + "\n" + std::to_string(price_) + " " + std::to_string(qty_) + " left.";
  return retVal;
}


/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Movie::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}

void Movie::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << genre_  << "\n" << rating_<< endl;
}



