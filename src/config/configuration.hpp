#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct BackupConfig {
    std::string root_path;
    std::string backup_name;
    std::string tag;
};

std::vector<BackupConfig> readConfig(const std::string& config_file) {
    std::vector<BackupConfig> configs;
    std::ifstream config_stream(config_file);
    json j;
    config_stream >> j;

    for (auto const& entry: j) {
        BackupConfig conf;
        entry.at("backup_name").get_to(conf.backup_name);
        entry.at("root_path").get_to(conf.root_path);
        entry.at("tag").get_to(conf.tag);
        configs.push_back(conf);
    }

    return configs;
}
