gcc -std=c90 -fsanitize=undefined -Wall "$1".c -o "$1".out && ./"$1".out
