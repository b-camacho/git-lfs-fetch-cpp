#include "git-lfs-fetch/git-lfs-fetch.h"
#include <iostream>

int main() {
    const std::string repo = "/home/bmc/sources/test-lfs";
    const auto res = get_lfs_endpoint_url(repo, repo);
    std::cout << res.first << " " << res.second << std::endl;

}
