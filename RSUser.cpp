
// don't change those includes
#include "RSUser.h"
#include "RecommenderSystem.h"

#define MAX_ENTRY 10
#define MIN_ENTRY 1
#define RANK_ERR "Rank is supposed to be between 1 and 10"


RSUser::RSUser (string& username, rank_map& ranks,
                std::shared_ptr<RecommenderSystem>& recs) :
    username (username), ranks (ranks), recs (recs)
{
}

const string& RSUser::get_name () const
{
  return username;
}

rank_map RSUser::get_ranks () const
{
  return ranks;
}

void
RSUser::add_movie_to_rs (const std::string &name, int year,
                         const std::vector<double> &features, double rate)
{
  if (rate > MAX_ENTRY || rate < MIN_ENTRY)
  {throw std::range_error(RANK_ERR);}
  sp_movie moviep = recs->add_movie(name, year, features);
  ranks.insert({moviep, rate});
}

double
RSUser::get_prediction_score_for_movie (const std::string &name,
                                        int year, int k) const
{
  sp_movie movie = (*recs).get_movie (name, year);
  return (*recs).predict_movie_score (*this, movie, k);
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return (*recs).recommend_by_cf (*this, k);
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return (*recs).get_recommendation_by_content(*this);
}

std::ostream & operator<< (std::ostream& os, RSUser& user)
{
  os << "name: " << user.username << std::endl;
  os << *(user.recs);
  return os << std::endl;
}