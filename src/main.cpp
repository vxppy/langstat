#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <regex>
#include <ranges>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include <p-ranav/argparse.hpp>

#include "glob.h"
#include "languages.h"

const size_t TEMP_BUFF_SIZE = 1024;

struct IgnorePattern {
    std::string raw;
    std::regex pattern;
};

struct FileInfo {
    Language language = Language::Unknown;
    size_t lines = 0;
};

struct LangStatData {
    std::unordered_map<Language, size_t> working{};
    nlohmann::json data = nlohmann::json::array();

    size_t total_lines = 0;

    void finalize() {
        float sum = 0;
        for(auto& [key, lines]: working) {
            const LanguageInfo& info = language_info[static_cast<size_t>(key)];

            const float percent = total_lines > 0 ? static_cast<float>(lines) / total_lines : 0.0f;
            sum += percent;

            data.push_back({
                { "key", info.key },
                { "name", info.name },
                { "color", info.color },
                { "lines", lines },
                { "percent", percent }
            });
        }

        std::sort(data.begin(), data.end(),
            [](const auto& a, const auto& b) {
                return a["lines"] > b["lines"];
            });

        if(total_lines > 0 && !data.empty()) {
            float discrepancy = 1.0f - sum;
            data[0]["percent"] = data[0]["percent"].get<float>() + discrepancy;
        }
    }
};

inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

inline auto match_ignore(const std::string &path,
    const std::vector<IgnorePattern> &ignores) {
    return std::find_if(ignores.begin(), ignores.end(), [&] (const IgnorePattern& ignore){
        return std::regex_match(path, ignore.pattern);
    });
}

Language get_language_from_shebang(const std::string& line) {
    if(line.empty() || line.size() < 2) {
        return Language::Unknown;
    }

    if(line[0] != '#' || line[1] != '!') return Language::Unknown;

    std::string_view shebang_ext = line;
    const size_t last_split = line.rfind(' ');

    if(last_split != std::string::npos) {
        shebang_ext = std::string_view(line.data() + last_split + 1, line.size() - line.size() - 1);
    }

    return get_language_from_ext(std::filesystem::path(shebang_ext)
        .stem()
        .generic_string()
    );
};

FileInfo get_file_info(const std::filesystem::path& path) {
    std::string ext_check = (
        path.has_extension()
            ? path.extension()
            : path.stem()
        ).generic_string();

    std::transform(ext_check.begin(), ext_check.end(), ext_check.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    Language language = get_language_from_ext(ext_check);

    std::ifstream file(path);

    if(!file) return { .language = language };

    std::vector<char> temp(TEMP_BUFF_SIZE);
    file.read(temp.data(), TEMP_BUFF_SIZE);

    const auto read_count = file.gcount();

    if(std::find(temp.begin(), temp.begin() + read_count, '\0')
        != temp.begin() + read_count) {
        return { .language = language };
    }

    file.clear();
    file.seekg(0, std::ios::beg);

    // we are sure its a text file
    if(language == Language::Unknown){
        std::string line;

        if(std::getline(file, line)) {
            rtrim(line);

            std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            language = get_language_from_shebang(line);
        }
    }


    if(file.peek() == EOF) return { .language = language };

    size_t count = std::count_if(
        std::istreambuf_iterator<char>{file},
        std::istreambuf_iterator<char>{},
        [](char c) {
            return c == '\n';
        }
    );

    file.clear();
    file.seekg(-1, std::ios::end);

    if(file.get() != '\n') {
        count++;
    }

    return { .language = language, .lines = count };
}

void add_file_to_stats(const std::filesystem::path &path, LangStatData& ls_data) {
    const FileInfo file_info = get_file_info(path);

    if(file_info.language == Language::Unknown) return;

    ls_data.working[file_info.language] += file_info.lines;
    ls_data.total_lines += file_info.lines;
}

void read_files(const std::string& root,
    const std::string& path,
    const std::vector<IgnorePattern>& ignores,
    LangStatData& ls_data) {

    std::string full_path = root + path;

    if(std::filesystem::is_symlink(full_path)) return;

    if(std::filesystem::is_regular_file(full_path)) {
        add_file_to_stats(full_path, ls_data);
        return;
    }

    if(!std::filesystem::is_directory(full_path)) return;

    for(const auto& file : std::filesystem::directory_iterator(full_path)) {
        const std::string entry_file = file.path().generic_string();
        const std::string relative = entry_file.substr(full_path.size());

        const auto& ignore = match_ignore(relative, ignores);

        if(ignore != ignores.end()) {
            continue;
        }

        if(std::filesystem::is_symlink(entry_file)) {
            continue;
        }

        if(std::filesystem::is_regular_file(entry_file)) {
            add_file_to_stats(entry_file, ls_data);
        } else if(std::filesystem::is_directory(entry_file)) {
            read_files(root, relative, ignores, ls_data);
        }
    }
}

void print_stats_to_terminal(LangStatData& ls_data, const uint8_t max_width) {
    std::cout << std::fixed << std::setprecision(2) << "Language Usage Stats: " << std::endl;

    for(const auto& item: ls_data.data) {
        const uint32_t color = item["color"];

        const int r = (color >> 16) & 0xFF, g = (color >> 8) & 0xFF, b = color & 0xFF;
        const size_t filled_bar_width = std::round(item["percent"].get<float>() * max_width);

        std::cout << "\x1b[48;2;" << r << ';' << g << ';' << b << 'm';

        for(size_t i = 0; i < filled_bar_width; i++) {
            std::cout << ' ';
        }

        std::cout << "\x1b[0m";

        for(size_t i = 0; i < max_width - filled_bar_width; i++) {
            std::cout << ' ';
        }

        std::cout << std::setw(6)
            << std::fixed << std::setprecision(2)
            << item["percent"].get<float>()
            << "% \x1b[38;2;" << r << ';' << g << ';' << b << 'm'
            << item["name"].get<std::string>() << "\x1b[0m" << std::endl;
    }

    std::cout.unsetf(std::ios::fixed);
}

int main(int argc, char** argv) {
    argparse::ArgumentParser program("langstat", "1.0");

    program.add_argument("src")
        .help("all dirs you want to aggregate result of")
        .nargs(argparse::nargs_pattern::at_least_one);

    program.add_argument("--ignore")
        .help("all patterns you want to ignore")
        .nargs(argparse::nargs_pattern::any);

    bool require_json;
    program.add_argument("--bar-width")
        .help("The length of bars in default mode only")
        .default_value(static_cast<uint32_t>(20)).scan<'i', uint32_t>();

    program
        .add_argument("--json")
        .help("get output as a json stream")
        .flag().default_value(false)
        .store_into(require_json);

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    std::vector src_args(program.get<std::vector<std::string>>("src"));
    std::sort(src_args.begin(), src_args.end(), [](const std::string& a, const std::string& b) {
        return  a.length() > b.length();
    });

    std::vector ignore_args(program.get<std::vector<std::string>>("ignore"));

    std::vector<std::string> paths;
    paths.reserve(src_args.size());

    std::vector<IgnorePattern> ignores;
    ignores.reserve(ignore_args.size());

    for(const std::string& globs: ignore_args) {
        for(const auto& word: std::views::split(globs, ',')) {
            std::string glob(word.begin(), word.end());

            try {
                ignores.push_back({
                    .raw = glob,
                    .pattern = std::regex(glob::to_regex_string(glob), std::regex::optimize)
                });
            } catch(const std::exception& err) {
                std::cerr << err.what() << std::endl;
                return 1;
            }
        }
    }

    for(const std::string& path: src_args) {
        bool can_path = true;

        std::string absolute(std::filesystem::absolute(path).generic_string());
        if(!std::filesystem::exists(absolute)) {
            std::cerr << "Ignoring path '" << absolute << "' as path doesn't exist" << std::endl;
            continue;
        }

        for(const std::string& existing_path: paths) {
            if(absolute == existing_path) {
                can_path = false;
                std::cerr << "Ignoring arg '" << absolute << "' as path already accounted by '" << existing_path << "'" << std::endl;
                break;
            }

            if(absolute.starts_with(existing_path)) {
                can_path = false;
                std::cerr << "Ignoring arg '" << absolute << "' as path already accounted by '" << existing_path << "'" << std::endl;
                break;
            }
        }

        if(can_path) {
            paths.push_back(absolute);
        }
    }


    LangStatData data;
    for(const std::string& path: paths) {
        read_files(path, "", ignores, data);
    }

    data.finalize();

    if(require_json) {
        std::cout << nlohmann::json{
            { "total_lines", data.total_lines },
            { "breakdown", data.data }
        } << std::endl;
        return 0;
    }

    if(paths.empty()) return 0;

    const uint8_t bar_width = static_cast<uint8_t>(std::clamp(program.get<uint32_t>("--bar-width"), 1u, 256u));

    print_stats_to_terminal(data, bar_width);

    return 0;
}
