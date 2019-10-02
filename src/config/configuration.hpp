#include <optional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct ProgramConfig {
	std::string backup_path;
	std::string db_path;
};

struct BackupConfig {
    std::string root_path;
    std::string backup_name;
    std::string tag;
};

//todo: replace with std::outcome
std::optional<std::vector<BackupConfig>> readBackupsConfig(const std::string& config_file) {
    std::vector<BackupConfig> configs;
    std::ifstream config_stream(config_file);
	if (!config_stream.is_open()) {
		return {};
	}
	std::cout << "Opened the config file.\n";
    json j;

	try {
		config_stream >> j;
		std::cout << "Read the config stream.\n";
		for (auto const& entry : j) {
			std::cout << "Reading config entry.\n";
			BackupConfig conf;
			entry.at("backup_name").get_to(conf.backup_name);
			entry.at("root_path").get_to(conf.root_path);
			entry.at("tag").get_to(conf.tag);
			configs.push_back(conf);
		}

		return configs;
	}
	catch (const std::exception& ex) {
		std::cerr << "Could not parse the config: " << ex.what() << "\n";
		return {};
	}
}

std::optional<ProgramConfig> readProgramConfig(const std::string& config_file) {
    ProgramConfig config;
    std::ifstream config_stream(config_file);
	if (!config_stream.is_open()) {
		return {};
	}
	try {
		json j;
		config_stream >> j;
		j.at("backup_path").get_to(config.backup_path);
		j.at("db_path").get_to(config.db_path);
	}
	catch (const std::exception& ex) {
		std::cerr << "Could not parse the config: " << ex.what() << "\n";
		return {};
	}
    return config;
}


