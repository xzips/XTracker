#include <iostream>



unsigned char* ReadBMP(const char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");

    if (f == NULL)
        throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    std::cout << std::endl;
    std::cout << "  Name: " << filename << std::endl;
    std::cout << " Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;

    int row_padded = (width * 3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    for (int i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for (int j = 0; j < width * 3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j + 2];
            data[j + 2] = tmp;

            std::cout << "R: " << (int)data[j] << " G: " << (int)data[j + 1] << " B: " << (int)data[j + 2] << std::endl;
        }
    }

    fclose(f);
    return data;
}


int main(int argc, char** argv) {
 


    auto img_pixels = ReadBMP("C:/Users/Aspen/Desktop/XTracker/bmp_sample.bmp");


    

    return 0;

}