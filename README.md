# Voronoi Images
Creates images consisting of voronoi cells

## Usage

Build the program using
    make


Run the program using
    ./main path/to/input-image.png num_seeds scale

where:

path/to/input-image.png: the image you want to use this program on
[optional] num_seeds: how many seeds (cells) there will be; increasing this number will make it take longer; default = 500
[optional] scale: the image will be scaled down by a factor before running the algorithm on it; this will increase performance but reduce the output image's quality; default = 1
