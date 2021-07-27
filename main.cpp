#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <stb/stb_image_resize.h>
#include <ctime>
#include <cfloat>
#include <vector>
#include <utility>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please specify a path to the input image");
        return 1;
    }
    int num_seeds = 500;
    if (argc > 2)
        num_seeds = atoi(argv[2]);
    int scale = 1;
    if (argc > 3)
        scale = atoi(argv[3]);
    if (num_seeds == 0 || scale == 0) {
        printf("The number of seeds and the scale must not be 0");
        return 2;
    }
    srandom(time(NULL));

    int width, height, channels;
    unsigned char *img2 = stbi_load(argv[1], &width, &height, &channels, 0);
    unsigned char *img = (unsigned char*) malloc(width * height * channels / pow(scale, 2));
    stbir_resize_uint8(img2, width, height, 0, img, width / scale, height / scale, 0, channels);
    width /= scale, height /= scale;

    std::vector<std::pair<int, int>> seeds;
    std::vector<std::pair<int, int>> cells[num_seeds];
    for (int i = 0; i < num_seeds; i++)
        seeds.push_back(std::make_pair(random() % width, random() % height));

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            int closest_index = 0;
            float closest_dist = FLT_MAX;

            for (int s = 0; s < num_seeds; s++) {
                int sx = seeds[s].first;
                int sy = seeds[s].second;

                float dist = pow(sx - x, 2) + pow(sy - y, 2);
                if (dist < closest_dist) {
                    closest_index = s;
                    closest_dist = dist;
                }
            }

            cells[closest_index].push_back(std::make_pair(x, y));
        }

    for (int s = 0; s < num_seeds; s++) {
        int r = 0, g = 0, b = 0;
        for (int i = 0; i < cells[s].size(); i++) {
            int pos = (cells[s][i].second * width + cells[s][i].first) * channels;
            r += img[pos + 0];
            g += img[pos + 1];
            b += img[pos + 2];
        }

        if (cells[s].size() > 0) {
            r /= cells[s].size();
            g /= cells[s].size();
            b /= cells[s].size();
        }

        for (int i = 0; i < cells[s].size(); i++) {
            int pos = (cells[s][i].second * width + cells[s][i].first) * channels;
            img[pos + 0] = r;
            img[pos + 1] = g;
            img[pos + 2] = b;
        }
    }

    stbir_resize_uint8(img, width, height, 0, img2, width * scale, height * scale, 0, channels);
    stbi_write_png("out.png", width * scale, height * scale, channels, img2, width * channels * scale);
    stbi_image_free(img);
    stbi_image_free(img2);

    return 0;
}

