
#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H
#include <map>
#include "RSUser.h"

typedef std::map<sp_movie, std::vector<double>> feature_map;
typedef std::pair<int, string> movie_tup;
typedef std::map<movie_tup, sp_movie> movie_map;

class RecommenderSystem
{
 public:

  explicit RecommenderSystem ();
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie
  add_movie (const std::string &name, int year, const std::vector<double>
      &features);

  /**
   * a function that calculates the movie with highest score based on movie
   * features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie get_recommendation_by_content (const RSUser &user);

  /**
   * a function that calculates the movie with highest predicted score based
   * on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);

  /**
   * Predict a user rating for a movie given argument using item cf procedure
   * with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;

  friend std::ostream &operator<< (std::ostream &os, const RecommenderSystem &
  rs);

 private:
  /**
   * Computes the angle between two vectors
   * @param feats_a first vector
   * @param feats_b second vector
   * @return angle between the vectors
   */
  double compute_similarity (const std::vector<double> &feats_a,
                             const std::vector<double> &feats_b);

  /**
   * Normalizes a given vector
   * @param ranks a rank_map object
   * @return normalized vector
   */
  std::vector<std::pair<sp_movie, double>> norm_vectorize (const rank_map &
  ranks);

  /**
   * Creates a preferences vector
   * @param norm_vec a normalized vector
   * @return a preferences vector
   */
  std::vector<double> pref_vectorize (std::vector<std::pair<sp_movie, double>>
                                      &norm_vec);

  /**
   * Multiplies a vector by a scalar
   * @param vec vector to scale
   * @param scalar scalar
   * @return multiplied vector
   */
  std::vector<double> vec_mult (std::vector<double> &vec, double
  scalar);

  /**
   * Adds two vector
   * @param a first vector
   * @param b second vector
   * @return product of both vectors
   */
  std::vector<double> &vec_add (std::vector<double> &a,
                                const std::vector<double> &b);

  /**
   * Finds the average rank of a user
   * @param ranks a rank_map object
   * @return average rank given by the user
   */
  double find_average_rank (const rank_map &ranks);
  movie_map database;
  feature_map feats;
};

#endif //RECOMMENDERSYSTEM_H
