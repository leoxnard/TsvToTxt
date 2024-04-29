#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

std::vector<std::vector<std::string>> read_csv(const std::string &filename, char delimiter)
{
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, delimiter))
        {
            row.push_back(value);
        }
        data.push_back(row);
    }

    return data;
}

std::vector<std::vector<std::string>> print_table(const std::vector<std::vector<std::string>> &data)
{
    std::vector<std::vector<std::string>> table;

    if (data.empty())
        return table;

    // Berechnen Sie die maximale Breite jeder Spalte
    std::vector<size_t> column_widths(data[0].size(), 0);
    for (const auto &row : data)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            if (row[i].size() > column_widths[i])
            {
                column_widths[i] = row[i].size();
            }
        }
    }

    // Drucken Sie die Daten in einer gut formatierten Tabelle
    const size_t distance = 2;
    const char fill = ' ';
    const char line = '-';
    const char corner = '+';
    const char vertical = '|';
    for (const auto &row : data)
    {
        std::vector<std::string> table_row;
        // Print a line of dashes on top of the table
        for (size_t i = 0; i < column_widths.size(); ++i)
        {
            std::stringstream ss;
            ss << std::left << corner << std::setw(column_widths[i] + distance) << std::setfill(line) << "";
            table_row.push_back(ss.str());
        }
        table_row.push_back(std::string(1, corner));
        table.push_back(table_row);

        std::vector<std::string> data_row;
        for (size_t i = 0; i < row.size(); ++i)
        {
            int padding = column_widths[i] - row[i].size() + distance; // Gesamtleerzeichen
            int left_padding = padding / 2;                            // Leerzeichen links
            int right_padding = padding - left_padding;                // Leerzeichen rechts
            std::stringstream ss;
            ss << vertical << std::string(left_padding, fill) << row[i] << std::string(right_padding, fill);
            data_row.push_back(ss.str());
        }
        data_row.push_back(std::string(1, vertical));
        table.push_back(data_row);
    }

    std::vector<std::string> table_row;
    // Print a line of dashes on top of the table
    for (size_t i = 0; i < column_widths.size(); ++i)
    {
        std::stringstream ss;
        ss << std::left << corner << std::setw(column_widths[i] + distance) << std::setfill(line) << "";
        table_row.push_back(ss.str());
    }
    table_row.push_back(std::string(1, corner));
    table.push_back(table_row);

    return table;
}

void save_txt(const std::vector<std::vector<std::string>> &data, const std::string &outputFilename)
{
    std::ofstream outputFile(outputFilename);
    if (outputFile.is_open())
    {
        for (const auto &row : data)
        {
            for (const auto &value : row)
            {
                outputFile << value;
            }
            outputFile << '\n';
        }
        outputFile.close();
        std::cout << "File saved as " << outputFilename << std::endl;
    }
    else
    {
        std::cerr << "Failed to open output file" << std::endl;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::string filename = argv[1];
    // std::string filename = "/home/mi/ls4830fu/code/TsvToTxt/data/test_data.tsv";

    char delimiter = (filename.find(".csv") != std::string::npos) ? ',' : '\t';

    auto data = read_csv(filename, delimiter);
    auto table = print_table(data);
    save_txt(table, filename.substr(0, filename.find_last_of(".")) + ".txt");
    return 0;
}
