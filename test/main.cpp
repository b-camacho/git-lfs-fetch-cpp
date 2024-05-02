#include "git-lfs-fetch/git-lfs-fetch.h"
#include <iostream>

void usage(char *prog_name) {
  std::cerr << "usage: " << prog_name << " "
            << "<path-to-repo>" << std::endl;
}
int main(int argc, char **argv) {
  if (argc != 2) {
    usage(argv[0]);
    return 1;
  }
  const char *repo_path = argv[1];
  std::string repo_path_str = std::string(repo_path);

  git_repository *repo = nullptr;
  git_libgit2_init();

  std::cerr << "Open the current directory as a git repository" << std::endl;
  int error = git_repository_open(&repo, repo_path);
  if (error < 0) {
    const git_error *e = git_error_last();
    std::cerr << "Error opening repository: " << e->message << std::endl;
    return -1;
  }

  std::cerr << "Parse lfs files:" << std::endl;
  auto metadatas = parse_lfs_files(repo);
  for (const auto &[path, oid, size] : metadatas) {
    std::cerr << path << ": " << oid << ", " << size << " bytes" << std::endl;
  }

  std::cerr << "Get LFS URL (usually the origin remote url)" << std::endl;
  auto [url, host, path] = get_lfs_endpoint_url(repo);
  std::cerr << url << ", " << host << ", " << path << std::endl;

  std::cerr << "Get an auth token, we shell out to ssh here" << std::endl;
  nlohmann::json token = get_lfs_api_token(host, path);
  std::cerr << "token: " << token << std::endl;

  std::cerr << "Fetch the urls" << std::endl;
  auto urls = fetch_urls(url, token, metadatas);
  std::cerr << "Got urls! ";
  for (const auto &url : urls) {
    std::cerr << url << std::endl;
  }

  std::cerr << "Creating temp dir for downloads" << std::endl;
  char dir_template[] = "lfs-tmp-XXXXXX";
  const char *temp_dir_cstr = mkdtemp(dir_template);
  if (temp_dir_cstr == nullptr) {
    perror("Failed to create temp directory");
    return 1;
  }
  const std::string temp_dir = std::string(temp_dir_cstr);

  std::cerr << "Fetching actual data" << std::endl;
  download_files(urls, temp_dir);

  move_files(metadatas, temp_dir, repo_path);

  std::filesystem::remove(temp_dir);
}
