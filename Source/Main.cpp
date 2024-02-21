/**
 * Typoglycemia - Scrambles the input text, but you can still read it.
 * Copyright (C) 2024 Andor Zoltán Fülöp
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> ScrambleText(std::vector<std::string> Input);
void PrintHelp();
void PrintVersion();
bool IsArgsContain(std::vector<std::string> Args, std::string SearchedArg);
std::string CleanAndToLowerWord(std::string Word);
std::vector<std::string> GetExcludedWords(std::vector<std::string> Args);
bool IsExcludedWord(std::vector<std::string> ExcludedWords, std::string Word);

int main(int argc, char *argv[])
{
    // Store the command line arguments in a vector of strings for easier handling
    std::vector<std::string> Arguments;
    for (int i = 1; i < argc; i++)
        Arguments.push_back(argv[i]);

    // Print the help page
    if (IsArgsContain(Arguments, "-h") || IsArgsContain(Arguments, "--help"))
    {
        PrintHelp();
        return EXIT_SUCCESS;
    }

    // Print the version
    if (IsArgsContain(Arguments, "-v") || IsArgsContain(Arguments, "--version") || 1 == argc)
    {
        PrintVersion();
        return EXIT_SUCCESS;
    }
    
    // Scramble the given text
    auto ScrambledWords = ScrambleText(Arguments);

    // Print the scrambled words
    for (std::string Word : ScrambledWords)
        std::cout << Word << " ";
    std::cout << "\n";
    
    return EXIT_SUCCESS;
}

std::vector<std::string> ScrambleText(std::vector<std::string> Input)
{
    std::vector<std::string> ScrambledWords;
    std::vector<std::string> ExcludedWords = GetExcludedWords(Input);

    // Remove the excluded words from the input
    std::string ExcludedWordsString;
    for (std::string Argument : Input)
        if (Argument.find("-e=") != std::string::npos || Argument.find("--exclude=") != std::string::npos)
            ExcludedWordsString = Argument;
    
    std::vector<std::string> FilteredInput;
    for (int i = 0; i < Input.size(); i++)
        if (Input[i] != ExcludedWordsString)
            FilteredInput.push_back(Input[i]);

    for (std::string Word : FilteredInput)
    {      
        // If the word is an excluded word, store it as it is
        if (IsExcludedWord(ExcludedWords, Word))
        {
            ScrambledWords.push_back(Word);
            continue;
        }

        // Split the word into 3 parts: first letter + special characters, middle part, last letter + special characters
        std::vector<std::string> Parts;

        int FirstCharIndex = 0;
        for (char C : Word)
        {
            if (isalpha(C)) break;
            FirstCharIndex++;
        }
        Parts.push_back(Word.substr(0, FirstCharIndex + 1));

        int LastPartStartIndex = 0;
        for (int i = 0; i < Word.length() - 1; i++)
        {
            if (isalpha(Word[i]) && !isalpha(Word[i + 1])) break;
            LastPartStartIndex++;
        }
        Parts.push_back(Word.substr(FirstCharIndex + 1, LastPartStartIndex - FirstCharIndex - 1));

        Parts.push_back(Word.substr(LastPartStartIndex, Word.length() - LastPartStartIndex));

        // Scramble the middle part and store the scrambled word
        if (Parts[1].length() > 1)
        {
            bool HasSameLettersNextToEachOther = true;
            while (Parts[0] + Parts[1] + Parts[2] == Word || HasSameLettersNextToEachOther)
            {
                for (int i = 0; i < Parts[1].length() - 1; i++)
                {
                    int RandomIndex = rand() % (Parts[1].length() - 1) + 1;
                    std::swap(Parts[1][i], Parts[1][RandomIndex]);
                }

                for (int i = 0; i < Parts[1].length() - 1; i++)
                {
                    if (Parts[1][i] == Parts[1][i + 1])
                    {
                        HasSameLettersNextToEachOther = true;
                        break;
                    }
                    HasSameLettersNextToEachOther = false;
                }
            }
            ScrambledWords.push_back(Parts[0] + Parts[1] + Parts[2]);
        }
        else 
        {
            ScrambledWords.push_back(Word);
        }
    }

    return ScrambledWords;
}

void PrintHelp()
{
    std::cout << "NAME" << "\n";
    std::cout << "\t" << "Typoglycemia - Scrambles the input text, but you can still read it." << "\n";
    std::cout << "\n";

    std::cout << "SYNOPSIS" << "\n";
    std::cout << "\t" << "typoglycemia [OPTIONS] <TEXT TO BE SCRAMBLED>" << "\n";
    std::cout << "\n";

    std::cout << "DESCRIPTION" << "\n";
    std::cout << "\t" << "The phenomenon, known as typoglycemia, is the ability to understand words when the first and" << "\n";
    std::cout << "\t" << "last letters are stable, but the intermediate letters are scrambled. Your brain puts the letters" << "\n";
    std::cout << "\t" << "back into a sequence again." << "\n";
    std::cout << "\n";

    std::cout << "OPTIONS" << "\n";
    std::cout << "\t" << "-e=<WORD TO EXCLUDE>[,<WORD TO EXCLUDE>]..., --exclude=<WORD TO EXCLUDE>[,<WORD TO EXCLUDE>]... " << "\n";
    std::cout << "\t\t" << "The listed words wont be scrambled" << "\n";
    std::cout << "\n";
    std::cout << "\t" << "-h, --help" << "\n";
    std::cout << "\t\t" << "Display this help page" << "\n";
    std::cout << "\n";
    std::cout << "\t" << "-v, --version" << "\n";
    std::cout << "\t\t" << "Display the version" << "\n";
    std::cout << "\n";

    std::cout << "EXAMPLES" << "\n";
    std::cout << "\t" << "typoglycemia Hello, world!" << "\n";
    std::cout << "\t" << "typoglycemia -e=hello Hello, world!" << "\n";
    std::cout << "\t" << "typoglycemia -h" << "\n";
    std::cout << "\t" << "typoglycemia -v" << "\n";
    std::cout << "\n";

    std::cout << "REPORTING BUGS" << "\n";
    std::cout << "\t" << "You can report bugs on the project's github page." << "\n";
    std::cout << "\t" << "<https://github.com/GamerFandor/Typoglycemia/issues>" << "\n";
    std::cout << "\n";
    
    std::cout << "LICENCE" << "\n";
    std::cout << "\t" << "Typoglycemia Copyright (C) 2024 Andor Zoltan Fulop" << "\n";
    std::cout << "\t" << "This program comes with ABSOLUTELY NO WARRANTY; for details visit <https://gnu.org/licenses/gpl.html>." << "\n";
    std::cout << "\t" << "This is free software, and you are welcome to redistribute it" << "\n";
    std::cout << "\t" << "under certain conditions; visit <https://gnu.org/licenses/gpl.html> for details." << "\n";
    std::cout << "\n";

    std::cout << "AUTHOR" << "\n";
    std::cout << "\t" << "Written by Andor Zoltan Fulop." << "\n";
    std::cout << "\n";
}

void PrintVersion()
{
    std::cout << "Typoglycemia v1.0.0\n\nTypoglycemia Copyright (C) 2024 Andor Zoltan Fulop\nThis program comes with ABSOLUTELY NO WARRANTY; for details visit <https://gnu.org/licenses/gpl.html>.\nThis is free software, and you are welcome to redistribute it\nunder certain conditions; visit <https://gnu.org/licenses/gpl.html> for details.\n\nWritten by Andor Zoltan Fulop.\n\n";
}

bool IsArgsContain(std::vector<std::string> Args, std::string SearchedArg)
{
    for (std::string Argument : Args)
        if (Argument == SearchedArg)
            return true;

    return false;
}

std::string CleanAndToLowerWord(std::string Word)
{
    std::string CleanedWord;
    for (char C : Word)
        if (isalpha(C))
            CleanedWord += tolower(C);

    return CleanedWord;
}

std::vector<std::string> GetExcludedWords(std::vector<std::string> Args)
{
    for (std::string Argument : Args)
    {
        if (Argument.find("-e=") != std::string::npos || Argument.find("--exclude=") != std::string::npos)
        {
            std::string ExcludedWords = Argument.substr(Argument.find("=") + 1);
            std::vector<std::string> ExcludedWordsVector;
            std::string Word;
            for (char C : ExcludedWords)
            {
                if (C == ',')
                {
                    ExcludedWordsVector.push_back(Word);
                    Word = "";
                }
                else
                {
                    Word += C;
                }
            }
            ExcludedWordsVector.push_back(Word);
            return ExcludedWordsVector;
        }
    }

    return std::vector<std::string>();
}

bool IsExcludedWord(std::vector<std::string> ExcludedWords, std::string Word)
{
    for (std::string ExcludedWord : ExcludedWords)
        if (CleanAndToLowerWord(ExcludedWord) == CleanAndToLowerWord(Word))
            return true;

    return false;
}
