#include "RSUsersLoader.h"
#include "RecommenderSystem.h"
#include "Movie.h"
#include <fstream>
#include <sstream>
#include <cmath>

string RSUsersLoader::read_from (const string& file_path)
{
  std::ifstream t(file_path);
  t.seekg(0, std::ios::end);
  size_t size = t.tellg();
  string buffer(size, ' ');
  t.seekg(0);
  t.read(&buffer[0], size);
  return buffer;
}

std::vector<RSUser>
RSUsersLoader::create_users_from_file (const std::string &users_file_path,
       std::shared_ptr<RecommenderSystem> rs) noexcept (false)
{
  std::vector<RSUser> ret;
  string mystr, name, junk;
  int i = 0, year;
  sp_movie cur = nullptr, movie = nullptr;
  double rank = -INFINITY;
  string buf = read_from (users_file_path);
  std::istringstream file (buf);
  getline (file, mystr);
  std::istringstream ss (mystr);
  std::vector<sp_movie> movie_vec;
  while (getline (ss, name, '-')) {
    ss >> year;
    movie = rs->get_movie (name, year);
    movie_vec.push_back (movie);
    getline(ss, junk, ' ');
  }
  while (getline(file, mystr)) {
    rank_map map (0, sp_movie_hash, sp_movie_equal);
    std::istringstream iss (mystr);
    iss >> name;
    while (iss) {
      if (iss >> rank)
      {
        if (rank > MAX_ENTRY || rank < MIN_ENTRY)
        {throw std::range_error(RANK_ERR);}
        cur = movie_vec[i];
        map[cur] = rank;
        i++;
      }
      else
      {
        if (!iss.eof())
        {
          iss.clear ();
          iss >> junk;
          cur = movie_vec[i];
          map[cur] = -INFINITY;
          i++;
        }
      }
    }
    RSUser user(name, map, rs);
    ret.emplace_back(user);
    i = 0;
  }
  return ret;
}
