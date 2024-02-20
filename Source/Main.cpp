// Copyright (c) 2024 Andor Zoltán Fülöp

#include <string>
#include <vector>
#include <iostream>

int main(int argc, char *argv[])
{
    // Store the command line arguments in a vector of strings for easier handling
    std::vector<std::string> Arguments;
    for (int i = 1; i < argc; i++)
        Arguments.push_back(argv[i]);

    // Scramble the word's letters except the first and last one
    std::vector<std::string> ScrambledWords;
    for (std::string Word : Arguments)
    {        
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

    // Print the scrambled words
    for (std::string Word : ScrambledWords)
        std::cout << Word << " ";
    std::cout << "\n";

    return EXIT_SUCCESS;
}
