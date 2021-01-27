#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <regex>
#include <map>

/**
 * Struct that holds information about logical connection between long and short versions of flags.
 * Used internally by the flags.flag_map.
 */
struct flag_strings {
    std::string short_name;
    std::string long_name;

    flag_strings(const std::string& short_name, const std::string& long_name) {
        this->short_name = short_name;
        this->long_name = long_name;
    }
    bool operator==(const std::string& str) const {
        if (short_name == str || long_name == str)
            return true;
        return false;
    }

    inline bool operator<(const flag_strings& fs) const {
        return short_name < fs.short_name;
    }
};

/**
 * Struct that holds the information about logical connections between commands entered by user and their meaning.
 * Used internally by the args_handler function.
 */
struct flags {
    enum flags_enum {
        help_, file_, newlines_, digits_, numbers_, chars_, anagrams_, palindromes_, sorted_, reverse_sorted_,
        output_, input_, invalid_
    };
    std::map<flag_strings, flags_enum> flag_map = {
            { flag_strings("-f", "--file"), file_ },
            { flag_strings("-n", "--newlines"), newlines_ },
            { flag_strings("-d", "--digits"), digits_ },
            { flag_strings("-dd", "--numbers"), numbers_ },
            { flag_strings("-c", "--chars"), chars_ },
            { flag_strings("-a", "--anagrams"), anagrams_ },
            { flag_strings("-p", "--palindromes"), palindromes_ },
            { flag_strings("-s", "--sorted"), sorted_ },
            { flag_strings("-rs", "--reverse-sorted"), reverse_sorted_ },
            { flag_strings("-o", "--output"), output_ },
            { flag_strings("-i", "--input"), input_ },

    };

    flags_enum interpret(const std::string& entry) {
        for (const std::pair<flag_strings, flags_enum>& p : flag_map)
            if (p.first == entry)
                return p.second;
        return invalid_;
    }


};

/**
 * Function counting number of newline characters in a file. Invoked with -n/--newlines flags.
 * @param file Analyzed file. Supplied after -f/--file fags.
 * @return Returns a short std::string message stating the number of newline characters in the file.
 */
std::string newlines(std::fstream& file) {
    file.clear();
    file.seekg(0);
    int counter = 0;
    for (std::string line; std::getline(file, line);)
        counter++;
    return "Number of newline characters in file: " + std::to_string(counter);
}

/**
 * Function counting the number of digits in a file. Invoked with -d/--digits flags.
 * @param file Analyzed file. Supplied after -f/--file fags.
 * @return Returns a short std::string message stating the number of digits in the file.
 */
std::string count_digits(std::fstream& file) {
    file.clear();
    file.seekg(0);
    int counter = 0;

    for (std::string line; std::getline(file, line);) {
        std::stringstream stream(line);
        for (std::string word; stream >> word;) {
            std::stringstream word_stream(word);
            for (char c; word_stream >> c;)
                if (isdigit(c))
                    counter++;
        }
    }
    return "Number of digits in file: " + std::to_string(counter);
}
/**
 * Function counting the number of numbers in a file. Invoked with -dd/--digits flags.
 * @param file Analyzed file. Supplied after -f/--file fags.
 * @return Returns a short std::string message stating the number of numbers in the file.
 */
std::string count_numbers(std::fstream& file) {
    int counter = 0;
    std::string pattern = R"((/\s|^)-?(\d+)(\s|$/gm))";

    for (std::string line; std::getline(file, line);) {
        size_t pos = line.find(pattern, 0);
        while (pos != std::string::npos) {
            line.find(pattern, pos + 1);
            counter++;
        }
    }
    std::string note = "*** UNRELIABLE RESULT: THIS FEATURE IS STILL IN DEVELOPMENT, SORRY :( ***";
    return "Number of numbers in file: " + std::to_string(counter) + note;
}

/**
 * Function counting the number of characters in a file. Invoked with -c/--chars flags.
 * @param file Analyzed file. Supplied after -f/--file flags.
 * @return Returns a short std::string message stating the number of characters in the file.
 */
std::string count_chars(std::fstream& file) {
    file.clear();
    file.seekg(0);
    int counter = 0;
    for (std::string line; std::getline(file, line);) {
        std::stringstream stream(line);
        for (std::string word; stream >> word;) {
            std::vector<char> chars(word.begin(), word.end());
            for (const char& c : chars)
                counter++;
        }
    }
    return "Number of characters in file: " + std::to_string(counter);
}

/**
 * Function that creates an alphabetically sorted set of all words in a file. Invoked with -s/--sorted flags.
 * @param file Analyzed file. Supplied after -f/--file fags.
 * @return A std::string vector look-alike message containing the alphabetically sorted set of all words from the file.
 */
std::string sorted(std::fstream& file) {
    file.clear();
    file.seekg(0);
    std::set<std::string> words_set;
    for (std::string line; std::getline(file, line);) {
        std::stringstream stream(line);
        for (std::string word; stream >> word;) {
            words_set.emplace(word);
        }
    }
    std::vector<std::string> sorted_words;
    std::copy(words_set.begin(), words_set.end(), std::back_inserter(sorted_words));
    std::sort(sorted_words.begin(), sorted_words.end());
    std::string result("[");
    for (int i = 0; i < sorted_words.size() -1; ++i)
        result.append(sorted_words[i]).append(", ");
    result.append(sorted_words.back()).append("]");

    return "Sorted alphabetically:\n" + result;
}

/**
 * Function that creates an alphabetically sorted set of all words in a file. Invoked with -s/--sorted flags.
 * @param file Analyzed file. Supplied after -f/--file fags.
 * @return A std::string vector look-alike message containing the reverse-alphabetically sorted set
 * of all words from the file.
 */
std::string reverse_sorted(std::fstream& file) {
    file.clear();
    file.seekg(0);
    std::set<std::string> sorted_words_set;
    for (std::string line; std::getline(file, line);) {
        std::stringstream stream(line);
        for (std::string word; stream >> word;)
            sorted_words_set.emplace(word);
    }
    std::vector<std::string> sorted_words;
    std::copy(sorted_words_set.begin(), sorted_words_set.end(), std::back_inserter(sorted_words));
    std::sort(sorted_words.begin(), sorted_words.end(), [](const std::string& str1, const std::string& str2){
        return str1 > str2;
    });
    std::string result("[");
    for (int i = 0; i < sorted_words.size() -1; ++i)
        result.append(sorted_words[i]).append(", ");
    result.append(sorted_words.back()).append("]");

    return result;
}

/**
 * Internal helper function for palindromes() and anagrams(). Functionally identical to sorted() apart from the return
 * value type.
 * @param file Analyzed file. Supplied indirectly by invoking -f/--file flags.
 * @return std::vector<std::string> containing an alphabetically sorted set of all words from the file.
 */
std::vector<std::string> sorted_vector(std::fstream& file) {
    file.clear();
    file.seekg(0);
    std::set<std::string> words_set;
    for (std::string line; std::getline(file, line);) {
        std::stringstream stream(line);
        for (std::string word; stream >> word;) {
            words_set.emplace(word);
        }
    }
    std::vector<std::string> sorted_words;
    std::copy(words_set.begin(), words_set.end(), std::back_inserter(sorted_words));
    std::sort(sorted_words.begin(), sorted_words.end());

    return sorted_words;
}

/**
 * Function that searches for declared palindromes in a text file and returns a list of those found.
 * Invoked with -p/--palindromes flags.
 * @param file Analyzed file. Supplied after -f/--file flags.
 * @param pals std::vector<std::string> list of the palindromes that are to be searched for in the file.
 * Non-palindrome members of pals will be ignored.
 * @return A std::string vector look-alike message containing the palindromes that were both in the supplied list
 * and the supplied file.
 */
std::string palindromes(std::fstream& file, const std::vector<std::string>& pals) {
    file.clear();
    file.seekg(0);
    std::vector<std::string> words_from_file = sorted_vector(file);
    std::vector<std::string> result_vec;

    for (const std::string& pal : pals) {
        std::string lap = pal;
        std::reverse(lap.begin(), lap.end());
        if (lap != pal)
            continue;
        for (const std::string& word : words_from_file)
            if (word == pal) {
                result_vec.push_back(pal);
                break;
            }
    }
    std::string result("[");
    for (int i = 0; i < result_vec.size() -1; ++i)
        result.append(result_vec[i]).append(", ");
    result.append(result_vec.back()).append("]");

    return "Palindromes:\n" + result;
}

/**
 * Function that searches for anagrams of words supplied by the user in a file. Invoked with -a/--anagrams flags.
 * @param file Analyzed file. Supplied after -f/--file flags.
 * @param anagrams std::vector<std::string> list of the words whose anagrams are to be searched for in the file.
 * @return A std::string vector look-alike message containing the anagrams of the supplied words
 * that were found in the file.
 */
std::string anagrams(std::fstream& file, const std::vector<std::string>& anagrams) {
    std::vector<std::string> words_from_file = sorted_vector(file);
    std::vector<std::string> result_vec;


    for (std::string anagram: anagrams) {
        std::vector<char> anvec(anagram.begin(), anagram.end());
        std::sort(anvec.begin(), anvec.end());
        for (std::string word : words_from_file) {
            if (word.length() != anagram.length())
                continue;
            std::vector<char> word_chars(word.begin(), word.end());
            std::sort(word_chars.begin(), word_chars.end());
            if (std::equal(anvec.begin(), anvec.end(), word_chars.begin(), word_chars.end()) &&
            !std::count(result_vec.begin(), result_vec.end(), word))
                result_vec.push_back(word);
        }
    }
    std::string result("[");
    for (int i = 0; i < result_vec.size() -1; ++i)
        result.append(result_vec[i]).append(", ");
    result.append(result_vec.back()).append("]");
    return "Anagrams:\n" + result;
}

/**
 * Function that display information about the program. Invoked when the program is run with no flags
 * or with the --help flag.
 */
void help() {
    std::cout << "=================================================================================================\n";
    std::cout << "*** PJAText ***\n";
    std::cout << "Text analyzing program\n";
    std::cout << "Use it to:\n";
    std::cout << "* Find out how many characters, lines, digits, numbers your text file has.\n";
    std::cout << "* Sort the words from your text file in alphabetical, or, reverse-alphabetical order.\n";
    std::cout << "* Find palindromes or anagrams of words in your text file.\n";
    std::cout << "All output can be printed directly into the console or saved into an output file at your leisure.\n";
    std::cout << "-------------------------------------------------------------------------------------------------\n";
    std::cout << "Invocation flags:\n\n";
    std::cout << "[No flag | --help]\nPrints out description page (this page) into the console.\n\n";
    std::cout << "[-f | --file]\nPlace a filepath after this flag to declare the file on which the program\n"
              << "will operate. This should be the first declared flag.\n\n";
    std::cout << "[-n | --newlines]\nUse this flag to find out how many newline characters are there in your file.\n\n";
    std::cout << "[-d | --digits]\nUse this flag to find out how many digits are there in your file.\n\n";
    std::cout << "[-dd | --numbers]\nUse this flag to find out how many numbers are there in your file.\n\n";
    std::cout << "[-c | --chars]\nUse this flag to find out how many characters are there in your file.\n\n";
    std::cout << "[-a | --anagrams]\nPlace a list of words after this flag to find their anagrams in the files.\n\n";
    std::cout << "[-p | --palindromes]\nPlace a list of words after this flag to find matching palindromes in\n"
              << "the file. Non-palindromes will be ignored.\n\n";
    std::cout << "[-s | --sorted]\nUse this flag to get a set of all words in the file in alphabetical order.\n\n";
    std::cout << "[-rs | --reverse-sorted]\nUse this flag to get a set of all words in\n"
              << "the file in reverse-alphabetical order.\n\n";
    std::cout << "[-o | --output]\nUse this flag to route the output of this program to a file\n"
              << "(filepath placed after the flag).\n\n";
    std::cout << "[-i | --input]\nUse this flag with a filepath to declare a file where the instructions are held.\n"
              << "If used, this should be the only declared flag.\n\n";
    std::cout << "=================================================================================================\n";



}

/**
 * Function that, in an erroneous situation, prints adequate error description message to std::cerr
 * and exits the program.
 * @param msg Error message to be printed out to std::cerr.
 */
void error(const std::string& msg) {
    std::cerr << msg << std::endl;
    exit(1);
}

/**
 * The muscle of the program. This is where the flags, file paths and all other arguments are interpreted.
 * From here, all 'functional' methods are invoked.
 * @param comvec The command line arguments parsed into std::vector<std::string> form.
 */
void analyze_commands(const std::vector<std::string>& comvec) {
    flags flags;
    std::ofstream output_file;
    std::string output;
    bool route_output_to_file = false;

    if (comvec.size() < 2)
        error("*** ERROR: INCORRECT INVOCATION PARAMETERS ***");

    flags::flags_enum first_flag = flags.interpret(comvec[0]);
    if (first_flag != flags::file_)
        error("*** ERROR: INCORRECT INVOCATION PARAMETERS ***");

    std::fstream file(comvec[1]);

    if (file.fail())
        error("*** ERROR: INVALID ANALYZED FILE FILEPATH : " + comvec[1] + " ***");

    std::vector<flags::flags_enum> flag_list;

    flags::flags_enum second_to_last = flags.interpret(comvec[comvec.size() - 2]);
    if (second_to_last == flags::output_) {
        route_output_to_file = true;
        output_file.open(comvec.back(), std::ios::app);

        if (output_file.fail())
            error("*** ERROR: INCORRECT INVOCATION PARAMETERS ***");
    }
    int limit = route_output_to_file ? comvec.size() - 2 : comvec.size();
    bool put_to_word_vec = false;
    std::vector<std::string> word_vec;

    for (int i = 2; i < limit; ++i) {
        if (!put_to_word_vec) {
            flags::flags_enum current_flag = flags.interpret(comvec[i]);
            if (current_flag == flags::invalid_)
                error("*** ERROR: INCORRECT INVOCATION PARAMETERS ***");
            if (current_flag == flags::anagrams_ || current_flag == flags::palindromes_) {
                if (!word_vec.empty())
                    error("*** ERROR: CANNOT CALL -a/--anagrams AND -p/--palindromes FLAGS AT THE SAME TIME ***");
                put_to_word_vec = true;
            }
            flag_list.push_back(current_flag);
        } else
            word_vec.push_back(comvec[i]);
    }
    for (flags::flags_enum flag : flag_list) {
        switch (flag) {
            case flags::help_: {
                help();
                break;
            }
            case flags::newlines_: {
                output.append(newlines(file)).append("\n");
                break;
            }
            case flags::digits_: {
                output.append(count_digits(file)).append("\n");
                break;
            }
            case flags::numbers_: {
                output.append(count_numbers(file)).append("\n");
                break;
            }
            case flags::chars_: {
                output.append(count_chars(file)).append("\n");
                break;
            }
            case flags::anagrams_: {
                output.append(anagrams(file, word_vec)).append("\n");
                break;
            }
            case flags::palindromes_: {
                output.append(palindromes(file, word_vec)).append("\n");
                break;
            }
            case flags::sorted_: {
                output.append(sorted(file)).append("\n");
                break;
            }
            case flags::reverse_sorted_: {
                output.append(reverse_sorted(file)).append("\n");
                break;
            }
            default: {
                error("*** ERROR: INCORRECT INVOCATION PARAMETERS ***");
                break;
            }
        }
    }
        if (!route_output_to_file) {
            std::cout << output << std::endl;
            return;
        }
        output_file << "\n" << output << "\n";
        output_file.close();
        file.close();
        std::cout << "*** OUTPUT SUCCESSFULLY TRANSFERRED TO: " << comvec.back() << " ***\n";
}

/**
 * Internal function whose purpose is to prepare commands from a file for analyze_commands() to use.
 * Invoked when program is run with -i/--input flags.
 * @param filename Name of the input file where commands are located
 */
void analyze_from_input_file(const std::string& filename) {
    std::fstream input_file(filename);
    std::string commands;

    if (input_file.fail())
        error("*** ERROR: INVALID INPUT FILE FILEPATH: " + filename + " ***");

    for (std::string line; std::getline(input_file, line);)
        commands.append(line);
    input_file.close();
    std::vector<std::string> comvec;
    std::stringstream stream;
    std::string com;
    for (auto c: commands) {
        if (c == ' ') {
            comvec.push_back(com);
            com = "";
        } else {
            com += c;
        }
    }
    comvec.push_back(com);
    analyze_commands(comvec);


}

/**
 * Function that does primary command-line arguments analysis.
 * This function is called upon start-up of the program and will invoke
 * analyze_commands() either directly or via analyze_from_input_file()
 * depending on whether the -i/--input flags were used.
 * @param argc Number of command-line arguments (analogous to main()'s argc).
 * @param argv List of command-line arguments (analogous to main()'s argv).
 */
void args_handler(int argc , char ** argv) {
    flags flags;
    std::fstream file;
    if (argc == 1) {
        help();
        return;
    }
    if (std::strcmp(argv[1], "-i") == 0 || std::strcmp(argv[1], "--input") == 0) {
        analyze_from_input_file(argv[2]);
        return;
    }

    std::vector<std::string> commands;
    for (int i = 1; i < argc; ++i)
        commands.emplace_back(argv[i]);
    analyze_commands(commands);
}

/**
 * Typical C++ main() method.
 * Called upon start-up, it will instantly call args_handler()
 * and pass onto it the command-line arguments and its count.
 * @param argc Number of command-line arguments.
 * @param argv List of command-line arguments.
 * @return Implicitly returns 0 if program finishes successfully or 1 if an error is caught (see: error() function).
 */
int main(int argc, char **argv) {
    args_handler(argc, argv);
}
