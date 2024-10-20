# SynthScript
SynthScript is a dynamically-typed scripting language designed to be read by humans.
It interfaces with the user's computer through many built-in functions like `output`, `read` and `write`.

## Installation
To install SynthScript, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/smnast/synthscript
    cd synthscript
    ```

2. Use CMake to build the project:
    ```sh
    cmake -S . -B build
    cmake --build build
    ```

3. The executable will be located at `build/sscript`

## Usage
Run from terminal `sscript <file_path>`

## Example
```sscript
count_words_in_file <- function(file_path) {
    file_text <- read(file_path)
    current_word <- ""
    word_count <- 0
    for ch in file_text {
        if ch = " " {
            if len(current_word) > 0 {
                word_count <- word_count + 1
                current_word <- ""
            }
        } else {
            current_word <- current_word + ch
        }
    }
    if len(current_word) > 0 {
        word_count <- word_count + 1
    }

    return word_count
}

output(count_words_in_file("example_file.txt"))
