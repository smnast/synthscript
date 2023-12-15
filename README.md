# SynthScript
SynthScript is a dynamically-typed scripting language designed to be read by humans.
It interfaces with the user's computer through many built-in functions like `output`, `read` and `write`.

## Installation
Download the latest release from [releases](https://github.com/MrPersonDev/SynthScript/releases/)\
Make the file executable via `chmod +x sscript`\
Move the executable file to your favourite bin directory `mv sscript ~/bin/sscript`\

## Usage
Run from terminal `sscript <file_path>`

## Example
```sscript
function count_words_in_file(file_path) {
    file_text <- read(file_path)
    current_word <- ""
    word_count <- 0
    for (i in 0..len(file_text)-1) {
        if (file_text[i] = " ") {
            if (len(current_word) > 0) {
                word_count <- word_count + 1
                current_word <- ""
            }
        } else {
            current_word <- current_word + file_text[i]
        }
    }

    return word_count
}

output(count_words_in_file("example_file.txt"))
```
