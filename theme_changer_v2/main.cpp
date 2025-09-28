#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

void redraw()
{
  cout << "\033[2J\033[H";
}

void stop()
{
  cout << "Press ENTER to continue." << endl;
  cin.get();
}

vector<pair<string, string>> getColorsC(const char* color_location)
{
  ifstream color(color_location);
  string value1;
  string value2;
  vector<pair<string, string>> colors;

  while(color >> value1)
  {
    if(value1 == "background")
    {
      color >> value2;
      colors.push_back({value1, value2});
    }
    if(value1 == "#eto_made")
    {
      colors.push_back({value1, "true"});
    }
    if(value1 == "foreground")
    {
      color >> value2;
      colors.push_back({value1, value2});
    }
    if(value1 == "cursor")
    {
      color >> value2;
      colors.push_back({value1, value2});
    }
    if(value1 == "selection_foreground")
    {
      color >> value2;
      colors.push_back({value1, value2});
    }
    if(value1 == "selection_background")
    {
      color >> value2;
      colors.push_back({value1, value2});
    }
    if(value1.rfind("color", 0) == 0)
    {
      color >> value2;
      colors.push_back({value1, value2});
    }
  }

  return colors;
}

pair<string, string> findColorC(const char* color_first, vector<pair<string, string>> colors)
{
  pair<string, string> return_value;
  for(auto &c : colors)
  {
    if(c.first == color_first)
    {
      return_value.first = c.first;
      return_value.second = c.second;
      break;
    }
  }
  return return_value;
}

vector<string> listColors(const char* directory)
{
  vector<string> listofcolors;
  for(const auto& entry : fs::directory_iterator(directory))
  {
    if(entry.path().extension() == ".conf")
    {
      listofcolors.push_back(entry.path().filename().string());
    }
  }
  return listofcolors;
}

int activate(int index_of_selected_color, const char* directory)
{
  redraw();
  vector<string> listofcolors = listColors(directory);

  string colorname = listofcolors[index_of_selected_color];
  string colorpath = string(directory) + "/" + colorname;

  fs::path filepathkitty = fs::path(getenv("HOME")) / ".config" / "kitty" / "themes" / colorname;
  fs::path folderpathkitty = fs::path(getenv("HOME")) / ".config" / "kitty" / "themes";
  vector<pair<string, string>> colorsc = getColorsC(filepathkitty.c_str());

  //check if the themes folder exists
  if(!fs::exists(folderpathkitty))
  {
    cout << "The \"themes\" folder does not exist." << endl;
    cout << "Creating one.." << endl;
    fs::create_directory(folderpathkitty);
    stop();
  }

  //check if the file exists in the theme folder
  if(fs::exists(filepathkitty))
  {
    pair<string, string> eto_made = findColorC("#eto_made", colorsc);

    //if the existing file is not made by itself (code)
    if(eto_made.second.empty())
    {
      cout << "The file already exists." << endl;
      cout << "Choose an action: (r)ename / (o)verwrite / (c)ancel: ";

      string options;
      getline(cin, options);

      if(tolower(options[0]) == 'r')
      {
        //rename currently activating file
      }
      if(tolower(options[0]) == 'o')
      {
        //overwrite (easy)
        fs::copy_file(colorpath, filepathkitty, fs::copy_options::overwrite_existing);
        cout << "overwritten." << endl;
        stop();
      }
      if(tolower(options[0]) == 'c')
      {
        cout << "cancled." << endl;
        stop();
        return 1;
      }
    }
    else
    {
      fs::copy_file(colorpath, filepathkitty, fs::copy_options::overwrite_existing);
      cout << "Updating..." << endl;
      stop();
    }
  }
  else
  {
    fs::copy_file(colorpath, filepathkitty, fs::copy_options::overwrite_existing);
    cout << "Creating..." << endl;
    stop();
  }
  return 1;
}

int menu_activate(const char* directory)
{
  redraw();
  cout << "##########################################" << endl;
  cout << "###      エト ヨシムラ Activation Menu ###" << endl;
  cout << "##########################################" << endl;
  cout << "###                                    ###" << endl;
  cout << "### Do what it says below              ###" << endl;
  cout << "###                                    ###" << endl;
  cout << "##########################################" << endl;
  cout << "###                                    ###" << endl;


  //get all of the files inside
  vector<string> listofcolors = listColors(directory);

  bool quit1 = false;
  int current_page = 1;
  double pages = ceil(static_cast<double>(listofcolors.size()) / 10);
  int contentsize = listofcolors.size();
  int selectedcolorindex = 0;
  int shouldshow = 10;
  int shouldshowstart = shouldshow - 10;

  //cout all of them but before format them
  do
  {
    redraw();
    cout << "##########################################" << endl;
    cout << "###      エト ヨシムラ Activation Menu ###" << endl;
    cout << "##########################################" << endl;
    cout << "###                                    ###" << endl;
    cout << "### Do what it says below              ###" << endl;
    cout << "###                                    ###" << endl;
    cout << "##########################################" << endl;
    cout << "###                                    ###" << endl;
    for(int i = shouldshowstart; i < shouldshow && i < listofcolors.size(); i++)
    {
      string output1;
      output1 += "### ";
      output1 += to_string((i+1)) + ". ";
      output1 += listofcolors[i];

      int calc1= 35 - output1.size();

      if(calc1 < 0)
      {
        output1.resize(35);
        output1 += "... ###";
      }
      else
      {
        for(int j = 0; j < calc1; j++)
        {
          output1 += " ";
        }
        output1 += "    ###";
      }
      cout << output1 << endl;

    }
    cout << "###                                    ###" << endl;
    cout << "##########################################" << endl;

    //ask if show next page if possible
    if (listofcolors.size() > 0)
    {
      string output2 = "Select page (" + to_string(current_page) + "/" + to_string((int)pages) + ") with n/p or the color number: ";
      cout << output2;

      string options;

      getline(cin, options);

      if(!options.empty())
      {
        if(tolower(options[0]) == 'n')
        {
          if(current_page < pages)
          {
            shouldshow = shouldshow + 10;
            shouldshowstart = shouldshow - 10;
            current_page += 1;
          }
        }
        else if(tolower(options[0]) == 'p')
        {
          if(shouldshow > 10)
          {
            shouldshow = shouldshow - 10;
            shouldshowstart = shouldshow - 10;
            current_page += -1;
          }
        }
        else if(tolower(options[0]) == 'q')
        {
          return 1;
        }
        else if(stoi(options) <= contentsize)
        {
          cout << "a color has been selected!" << endl;
          quit1 = true;
          activate((stoi(options) - 1), directory);
        }
      }
    }
    else
    {
      cout << "No options to select from" << endl;
    }
  }
  while(!quit1);

  return 1;
}

int menu_delete()
{
  //the logic
  return 1;
}

int menu(int selection)
{
  cout << "##########################################" << endl;
  cout << "###         エト ヨシムラ Menu         ###" << endl;
  cout << "##########################################" << endl;

  if(selection == 1)
  {
    cout << "###                                    ###" << endl;
    cout << "### Select Option:                     ###" << endl;
    cout << "###     1. Activate                    ###" << endl;
    cout << "###     2. Edit                        ###" << endl;
    cout << "###     3. Delete                      ###" << endl;
    cout << "###     4. Test                        ###" << endl;
    cout << "###     5. Quit                        ###" << endl;
    cout << "###                                    ###" << endl;
    cout << "##########################################" << endl;

    cout << "Enter your choice (1-5 or q): ";
    string option;

    getline(cin, option);

    if(!option.empty() && tolower(option[0]) == 'q')
    {
      return 0;
    }
    else if(!option.empty() && stoi(option) == 1)
    {
      menu_activate("colors");
    }
    else if(!option.empty() && stoi(option) == 4)
    {
      redraw();
      cout << "##########################################" << endl;
      cout << "###        エト ヨシムラ Test Menu     ###" << endl;
      cout << "##########################################" << endl;
      cout << "###                                    ###" << endl;
      cout << "### 1. Find a color from the file      ###" << endl;
      cout << "### 2. Find a color from the file      ###" << endl;
      cout << "###    (custom location and file)      ###" << endl;
      cout << "### 3. Check string lenght             ###" << endl;
      cout << "###                                    ###" << endl;
      cout << "##########################################" << endl;
      cout << "Enter your choice (1-3): ";

      getline(cin, option);

      if(!option.empty() && stoi(option) == 1)
      {
        vector<pair<string, string>> colors = getColorsC("colors/fog.conf");
        pair<string, string> color14 = findColorC("color14", colors);

        if(!color14.second.empty())
        {
          cout << color14.first << " " << color14.second << endl;
        }
      }
      else if(!option.empty() && stoi(option) == 2)
      {
        string find;
        string location;

        cout << "Enter the file location: ";

        getline(cin, location);

        cout << "Enter the color name to find: ";

        getline(cin, find);

        vector<pair<string, string>> colors = getColorsC(location.c_str());
        pair<string, string> find_color = findColorC(find.c_str(), colors);

        if(!find_color.second.empty())
        {
          cout << find_color.first << " " << find_color.second << endl;
        }
        else
        {
          cout << "not found" << endl;
        }
      }
      if(!option.empty() && stoi(option) == 3)
      {
        string size_test = "###       エト ヨシムラ Activation Menu ###";
        size_test = "##########################################";
        string twochar = "12";

        cout << "the 2 character string is " << twochar.length() << " characters long" << endl;
        cout << "the teststring is " << size_test.length() << " characters long" << endl;
        cout << "the string size is " << size_test.size() << " indicies high/long" << endl;
      }
    }
  }
  return 1;
}

int main()
{
  //start pseudo
  cout << "\033[?1049h\033[H";

  menu(1);

  //end pseudo
  cout << "\033[?1049l";
  return 0;
}
