# Basic Text Analysis
**Semestral work by Patrik Dvořáček for Programming in C/C++ (B6B36PJC)**

## Assignment

The input is a path to a file or a directory. The outputs are statistics with a word count, a unique word count and n-grams or a word cloud. Statistics can be generated for the input overall or per file and it will be possible to filter out selected words, for example [stop words](https://en.wikipedia.org/wiki/Stop_word).

## Usage

| Argument                | Default | Description                                                                                                                                                                                                                                   |
| ----------------------- | ------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| *First argument*        | `none`  | Path to source file or directory. Works with relative and absolute paths. Must be first.                                                                                                                                                      |
| `-h` or `--help`        | `false` | Displays the help information.                                                                                                                                                                                                                |
| `-p` or `-perFile`      | `false` | Generates statistics or word clouds per file.                                                                                                                                                                                                 |
| `-c` or `--ignorCase`   | `false` | Ignore case sensitivity.                                                                                                                                                                                                                      |
| `-t` or `--target`      | `none`  | Path of the output file or directory. If not set, statistics will be printed to the standard output and word clouds will use their defaults.                                                                                                  |
| `-n` or `--ngrams`      | `0`     | Generates n-grams of set size. Size must be at least 1. Off by default.                                                                                                                                                                       |
| `-w` or `--words`       | `true`  | Generate number of words.                                                                                                                                                                                                                     |
| `-u` or `--unique`      | `true`  | Generate number of unique words.                                                                                                                                                                                                              |
| `-f` or `--filter`      | `none`  | Sets the list of filtered words from command line. Argument must be followed by a list of words separated by `,`, for example `one,two,three,four`.                                                                                           |
| `-ff` or `--fileFilter` | `none`  | Sets the list of filtered words from a file. Argument must be followed by a path to a file with a single word on each line. Example can be found in `./examples/filter/stop_words_english.txt`.                                               |
| `-c` or `--cloud`       | `false` | Generates a word cloud(s) from loaded words into SVG files. If target path is not set, generates overall word cloud into `./word_cloud.svg` and per-file word clouds into `./word_clouds` with file paths used as names for generated clouds. |

## Implementation

The project is targeting C++ 17 and includes only single threaded implementation. Description below is top-level only and more details are available as comments alongisde the source code.

The project is structured into three distinct parts:

- Analyzer (analyzer.hpp/.cpp, statistics.hpp/.cpp)
- Command Line (cmdline.hpp/.cpp)
- Word Clouds (word_cloud.hpp/.cpp)

### Analyzer

**Analyzer** is the main component of the project. This class handles parsing and generation of all statistics as well as generation of word clouds. It receives a source path, words to be filtered out and case sensitivity flag. For each source file a Statistics class is created which then handles all interactions with it's file. After all Statistics are loaded, Analyzer generates necessary data upon request.

**Statistics** handles reading a parsing of words from a file. File text is read as UTF-8 encoded to ensure the widest possible support for different languages. Most text file formats are supported but it is possible that binary files or others will be treated as text as well, which can then pollute the results. 

An error during parsing is not treated as a fatal error. An error message is displayed on the standard error ouput but execution contious. This is due to the possibility that only one file out of multiple is locked or unavailable.

### Command Line

**CommandLine** parses the command line input into **CommandLineOptions** which are then passed on to Analyzer or used in main. Invalid arguments are rejected and cause the program to exit. The same happens in case filtered words of a file with filtered words is formatted incorrectly.

### Word Cloud

Word Cloud handles creation of a SVG file with word cloud from words supplied by Analyzer. Words are weighted by number of their occurences and then set randomly into the SVG. Words are unfortunately going to be overlaping. This is because SVG Text is rendered differently on each platform and it is thus impossible to calculate the size of text before rendering. Current overlap calculations are only approximations.

SVG is used due to it being supported by almost every possible platform and creation of simple SVG files does not require additional libraries.

## Examples

Example input data can be found inside `./examples/input`. The expected values, without using filters, are:

| File                | Words (Case Insensitive) | Unique Words (Case Insensitive) | 2-grams (All Case Insensitive)                                                                 |
| ------------------- | ------------------------ | ------------------------------- | ---------------------------------------------------------------------------------------------- |
| bacon_ipsum.txt     | 1207 (1207)              | 206 (135)                       | strip steak(11), ball tip(10), short ribs(9), pork loin(9), ground round(9)                    |
| kafka.txt           | 200 (200)                | 143 (138)                       | a little(2), - samsa(1), a collection(1), a dream(1), a fur(1)                                 |
| lorem_ipsum.txt     | 1770 (1770)              | 197 (184)                       | sit amet(18), et malesuada(4), viverra ac(3), fames ac(3), habitasse platea(3)                 |
| random_english.txt  | 342 (342)                | 170 (164)                       | stop that(8), that pigeon(8), for one(6), pigeon stop(6), around the(6)                        |
| slothman.txt        | 1024 (1024)              | 110 (62)                        | lorem exercitation(3), consectetur velit(3), cupidatat non(3), anim sint(3), incididunt non(3) |
| the_egg_czech.txt   | 1006 (1006)              | 540 (492)                       | řekl jsem(9), zeptal ses(6), já jsem(6), řekl jsi(5), ve skutečnosti(4)                        |
| the_egg_english.txt | 997 (997)                | 424 (377)                       | i said(14), you said(8), you asked(5), all the(4), for you(4)                                  |
| the_egg_spanish.txt | 924 (924)                | 476 (422)                       | lo que(5), el tiempo(4), de los(3), en el(3), es el(3)                                         |
| **Overall**         | **7470 (7470)**          | **1987 (1728)**                 | sit amet(18), i said(14), strip steak(11), ball tip(10), pork loin(9)                          |
