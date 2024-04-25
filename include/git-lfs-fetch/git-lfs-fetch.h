#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <array>
#include <cstdlib>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

std::string exec_command(const std::string& cmd, const fs::path& working_dir) {
    std::cout << cmd << std::endl;
    std::string data;
    std::array<char, 256> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    if (result.back() == '\n') {
        result.pop_back();
    }
    return result;
}

nlohmann::json get_lfs_api_token(const std::string& host, const std::string& path) {
    std::string command = "ssh git@" + host + " git-lfs-authenticate " + path + " download";
    std::string output = exec_command(command, ".");
    nlohmann::json header_info;

    if (!output.empty()) {
        nlohmann::json query_resp = nlohmann::json::parse(output);
        header_info = query_resp["header"];
    }

    return header_info;
}

std::pair<std::string, std::string> get_lfs_endpoint_url(const std::string& git_repo, const std::string& checkout_dir) {
    std::string url;
    url = exec_command("git config --get remote.origin.url", git_repo);
    if (url.back() == '/') {
        url.pop_back();
    }
    if (url.find("/info/lfs") == std::string::npos) {
        url += (url.find(".git") != std::string::npos) ? "/info/lfs" : ".git/info/lfs";
    }

    std::string scheme, host, path;
    auto pos = url.find("://");
    if (pos != std::string::npos) {
        scheme = url.substr(0, pos);
        url = url.substr(pos + 3);
    }
    pos = url.find('/');
    if (pos != std::string::npos) {
        host = url.substr(0, pos);
        path = url.substr(pos);
    } else {
        host = url;
        path = "/";
    }

    if (scheme != "https") {
        if (scheme.empty()) {
            // SSH format: git@example.org:repo.git
            auto at_pos = host.find('@');
            auto colon_pos = host.find(':', at_pos);
            host = host.substr(at_pos + 1, colon_pos - at_pos - 1);
            path = host.substr(colon_pos + 1);
        }
        url = "https://" + host + path;
    }

    // Placeholder for actual authentication logic
    std::string auth_header = get_lfs_api_token(host, path);

    return {url, auth_header};
}

