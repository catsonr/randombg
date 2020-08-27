# randombg
a background manager written in C for GNU/linux using FEH. it randomly selects an image from the specified directory to set as the background on all monitors (feh alone sets a different random image to all backgrounds.)

# usage
clone the repo

`cd randombg`

`gcc main.c`

`./a.out`

# options

-l: loops background changing (default set to sleep for 1hr)

-t: changes sleep length (ex: -t60s, -t30m, -t1d)

-o: outputs to terminal

# misc

place in your .xinitrc and run as a background task to have it execute at start-up
