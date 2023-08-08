#include <numeric>
#include <cmath>
#include <algorithm>
#include "RecommenderSystem.h"

RecommenderSystem::RecommenderSystem ()
{
}

sp_movie
RecommenderSystem::add_movie (const std::string &name, int year,
                              const std::vector<double> &features)
{
  Movie movie (name, year);
  sp_movie p = std::make_shared<Movie> (movie);
  movie_tup tup = {year, name};
  database.insert ({tup, p});
  feats.insert ({p, features});
  return p;
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{

  auto check = database.find ({year, name});
  if (check == database.end ())
  {
    return nullptr;
  }
  sp_movie ret = check->second;
  return ret;
}

double inner_prod (const std::vector<double> &feats_a,
                   const std::vector<double> &
feats_b, size_t size)
{
  double ret = 0;
  for (size_t i = 0; i < size; i++)
  {
    ret += feats_a[i] * feats_b[i];
  }
  return ret;
}

double norm (size_t size, const std::vector<double> &vec)
{
  double ret = 0;
  for (size_t i = 0; i < size; i++)
  {
    ret += pow (vec[i], 2);
  }
  return sqrt (ret);
}

double RecommenderSystem::compute_similarity (const std::vector<double>
    &feats_a,
                                              const std::vector<double>
                                                  &feats_b)
{
  size_t size = feats_a.size ();
  double inner = inner_prod (feats_a, feats_b, size);
  double norm_mult = norm (size, feats_a) * norm (size, feats_b);
  return inner / norm_mult;
}

bool comp_func (std::pair<sp_movie, double> &a, std::pair<sp_movie, double> &b)
{
  return a.second > b.second;
}

double
RecommenderSystem::predict_movie_score (const RSUser &user, const sp_movie
&movie, int k)
{
  std::vector<std::pair<sp_movie, double>> rates;
  auto feats_a1 = feats.find (movie);
  auto feats_a = feats_a1->second;
  rank_map ranks = user.get_ranks ();
  for (auto &rank: ranks) //creating the similarity vector
  {
    if (rank.second == -INFINITY)
    {
      continue;
    }
    sp_movie cur_mov = rank.first;
    auto feats_b1 = feats.find (cur_mov);
    auto feats_b = feats_b1->second;
    std::pair<sp_movie, double> cur = {rank.first,
                                       compute_similarity
                                       (feats_a, feats_b)};
    rates.push_back (cur);
  }
  std::sort (rates.begin (), rates.end (), comp_func);
  double up = 0, down = 0;
  for (int i = 0; i < k; i++) //computing final match
  {
    double rank = ranks.find (rates[i].first)->second;
    up += rank * rates[i].second;
    down += rates[i].second;
  }
  return up / down;
}

sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  std::pair<sp_movie, double> max = {nullptr, -INFINITY};
  for (const auto &cur_rank: user.get_ranks ())
  {
    sp_movie cur = cur_rank.first;
    if (cur_rank.second > -INFINITY)
    {
      continue;
    }
    double sec = predict_movie_score (user, cur, k);
    if (sec > max.second)
    {
      max = {cur, sec};
    }
  }
  return max.first;
}

double RecommenderSystem::find_average_rank (const rank_map &ranks)
{
  double total = 0;
  int num = 0;
  for (auto rank: ranks)
  {
    if (rank.second > -INFINITY)
    {
      total += rank.second;
      num++;
    }
  }
  return total / num;
}

std::vector<std::pair<sp_movie, double>> RecommenderSystem::norm_vectorize
    (const rank_map &ranks)
{
  std::vector<std::pair<sp_movie, double>> ret;
  double avg = find_average_rank (ranks);
  for (auto rank: ranks)
  {
    if (rank.second > -INFINITY)
    {
      ret.push_back ({rank.first, rank.second - avg});
    }
    else
    {
      ret.push_back (rank);
    }
  }
  return ret;
}

std::vector<double> RecommenderSystem::vec_mult (std::vector<double> &
vec, double scalar)
{
  std::vector<double> ret;
  for (size_t i = 0; i < vec.size (); i++)
  {
    ret.push_back (vec[i] * scalar);
  }
  return ret;
}

std::vector<double>& RecommenderSystem::vec_add (std::vector<double> &a,
                                                const std::vector<double> &b)
{
  for (size_t i = 0; i < a.size (); i++)
  {
    a[i] = a[i] + b[i];
  }
  return a;
}

std::vector<double> RecommenderSystem::pref_vectorize
    (std::vector<std::pair<sp_movie,
        double>> &norm_vec)
{
  size_t size = feats.find(norm_vec[0].first)->second.size();
  std::vector<double> ret;
  for (size_t i = 0; i < size; i++)
  {
    ret.push_back (0);
  }
  for (auto cur: norm_vec)
  {
    if (cur.second > -INFINITY)
    {
      auto feat = feats[cur.first];
      auto cur_feats = vec_mult (feat, cur.second);
      //finding the feats vector and multiplying it by the user
          // normalized ranks
      vec_add (ret, cur_feats);
    }
  }
  return ret;
}

sp_movie RecommenderSystem::get_recommendation_by_content (const RSUser &user)
{
  std::pair<sp_movie, double> max = {nullptr, -INFINITY};
  auto norm = norm_vectorize (user.get_ranks ());
  auto pref = pref_vectorize (norm);
  norm.clear();
  for (const auto& rank : user.get_ranks())
  {
    if (rank.second == -INFINITY)
    {
      double cur = compute_similarity
          (pref,feats.find(rank.first)->second);
      if (cur > max.second)
      {
        max = {rank.first, cur};
      }
    }
  }
  pref.clear();
  return max.first;
}

std::ostream & operator<< (std::ostream& os, const RecommenderSystem& rs)
{
  for (const auto & iter : rs.database)
  {
    os << *(iter.second);
  }
  return os;
}
