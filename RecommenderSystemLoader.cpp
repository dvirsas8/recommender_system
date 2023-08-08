#include "RecommenderSystemLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define FEAT_ERR "Feature is supposed to be between 1 and 10"

string RecommenderSystemLoader::read_from_file (const string& file_path)
{
  std::ifstream file(file_path);
  file.seekg(0, std::ios::end);
  size_t size = file.tellg();
  string buffer(size, ' ');
  file.seekg(0);
  file.read(&buffer[0], size);
  return buffer;
}

std::unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file (const
std::string &movies_file_path) noexcept (false)
{
  string mystr, name;
  double vec_ent;
  RecommenderSystem rec;
  std::vector<double> feats;
  string buf = read_from_file (movies_file_path);
  std::istringstream file (buf);
  while (getline (file, mystr))
  {
    std::istringstream ss (mystr);
    getline (ss, name, '-');
    int year;
    ss >> year;
    while (ss >> vec_ent)
    {
      if (vec_ent > MAX_ENTRY || vec_ent < MIN_ENTRY)
      {
        throw std::range_error(FEAT_ERR);
      }
      feats.push_back (vec_ent);
    }
    rec.add_movie (name, year, feats);
    feats.clear();
  }
  return std::make_unique<RecommenderSystem>(rec);
}

