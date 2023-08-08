
#ifndef RECOMMENDERSYSTEMLOADER_H
#define RECOMMENDERSYSTEMLOADER_H

#define MAX_ENTRY 10
#define MIN_ENTRY 1
#include "RecommenderSystem.h"


class RecommenderSystemLoader
{

private:
  static string read_from_file (const string& file_path);

 public:
    RecommenderSystemLoader() = delete;
    /**
	 * loads movies by the given format for movies with their feature's score
	 * @param movies_file_path a path to the file of the movies
	 * @return shared pointer to a RecommenderSystem which was created with
     * those movies
	 */
    static std::unique_ptr<RecommenderSystem>
    create_rs_from_movies_file(const std::string&
    movies_file_path) noexcept(false);
};


#endif //RECOMMENDERSYSTEMLOADER_H
