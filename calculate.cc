#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

constexpr auto sep = ',';
constexpr auto comment_symbol = "--";
constexpr auto hours_pos = 0;
constexpr auto grade_pos = 1;

auto convert_letter_grade(std::string &letter) -> double {
	const std::map<std::string, double> convert = {{"A", 4.00},  {"A-", 3.67}, {"B+", 3.33}, {"B", 3.00},
						       {"B-", 2.67}, {"C+", 2.33}, {"C", 2.00},	 {"C-", 1.67},
						       {"D+", 1.33}, {"D", 1.00},  {"D-", 0.67}, {"F", 0.00}};

	assert(convert.find(letter) != std::end(convert));
	return convert.at(letter);
}

auto get_csv(const std::filesystem::path &file_path) -> std::optional<std::vector<std::vector<std::string>>> {
	auto f = std::ifstream{file_path};
	if (f.fail()) {
		return std::nullopt;
	}

	auto csv = std::vector<std::vector<std::string>>{};

	std::string line;
	int line_count = 0;
	while (std::getline(f, line)) {
		if (line.size() == 0 || line.substr(0, 2) == comment_symbol) {
			continue;
		}

		csv.push_back({});
		std::string item;
		auto stream = std::stringstream{line};
		while (std::getline(stream, item, sep)) {
			csv.at(line_count).push_back(std::move(item));
		}

		line_count++;
	}

	return csv;
}

auto calculate_gpa(const std::vector<std::vector<std::string>> &csv) {
	auto total_hours = std::accumulate(std::cbegin(csv), std::cend(csv), 0,
					   [](int prev, auto row) { return prev + std::stoi(row.at(hours_pos)); });

	auto total_grade_points = std::accumulate(std::cbegin(csv), std::cend(csv), 0.00, [&](double prev, auto row) {
		return prev + std::stoi(row.at(hours_pos)) * convert_letter_grade(row.at(grade_pos));
	});

	return total_grade_points / static_cast<double>(total_hours);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		return 1;
	}

	const std::string &grades_csv = argv[1];

	auto csv = get_csv(grades_csv);

	if (csv == std::nullopt) {
		return 1;
	}

	std::cout << calculate_gpa(*csv) << std::endl;

	return 0;
}
