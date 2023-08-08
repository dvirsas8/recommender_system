
#ifndef USERFACTORY_H
#define USERFACTORY_H
#define MAX_ENTRY 10
#define MIN_ENTRY 1
#define RANK_ERR "Rank is supposed to be between 1 and 10"
#include "RSUser.h"

class RSUsersLoader
{
private:
  static string read_from (const string& file_path);

public:
    RSUsersLoader() = delete;
    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file of the users and their
     * movie ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static std::vector<RSUser> create_users_from_file(const std::string&
    users_file_path, std::shared_ptr<RecommenderSystem> rs) noexcept(false);

};


#endif //USERFACTORY_H
