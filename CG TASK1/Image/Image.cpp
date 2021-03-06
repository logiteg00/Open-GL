#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>


// Конструктор из пути a_path
Image::Image(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, sizeof(Pixel))) != nullptr)
  {
    size = width * height * channels;
  }
  else
  {
    std::cout << "wrong read " << a_path << std::endl;
  }
  str = a_path;
}

void Image::print(Image &screen){
  for(int y = 0; y < screen.Height(); ++y)
  {
    for(int x = 0; x < screen.Width() ; ++x)
    {
      screen.PutPixel(x, y, this->GetPixel(x,y) );
    }
  }
}

// Когструктор по параметрам (размеру, каналлам)
Image::Image(int a_width, int a_height, int a_channels)
{
  data = new Pixel[a_width * a_height ]{};

  if(data != nullptr)
  {
    width = a_width;
    height = a_height;
    size = a_width * a_height * a_channels;
    channels = a_channels;
    self_allocated = true;
  }
}



int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }

  return 0;
}



int Image::rotate180deg (const std::string &a_path)
{
  Image temp_image(a_path);
  for(int y = 0; y < Height() ; ++y)
  {
    for(int x = 0; x < Width(); ++x)
    {
      PutPixel(x,y, temp_image.GetPixel( x,Height() - y));
    }
  }
  rotated = true;
  return 0;
}



Image::~Image()
{
  if(self_allocated)
    delete [] data;
  else
  {
    stbi_image_free(data);
  }
}
