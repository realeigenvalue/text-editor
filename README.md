# text-editor

A console text-editor using a custom Document Library.

![Application Image](text-editor.png)

## Building

Prerequisites
- GCC
- Clang
- Make

```bash
sudo apt-get update && sudo apt-get install clang-5.0 libc++abi-dev libc++-dev git gdb valgrind graphviz imagemagick gnuplot
sudo apt-get install libncurses5-dev libncursesw5-dev
git clone https://github.com/realeigenvalue/text-editor.git
cd text-editor
make
./editor <filename>
p <line_number> #print a single line
p #print the whole document
w <line_number> <text> #writes text at line_number
a <line_number> <text> #appends text at line_number
d <line_number> #deletes the text at line_number
/<search-term> #search for the search-term in the document
s #saves the document
q #quits the editor
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
GNU GPLv3
