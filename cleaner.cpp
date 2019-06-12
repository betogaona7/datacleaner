#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/tracking.hpp>

#include <sys/stat.h>
#include <glob.h>

using namespace std;
using namespace cv;

void renameAll(vector <string> paths, string dst, string pattern = "", string ext = "jpg"){
    for(int i = 0; i < paths.size(); ++i){
        Mat image = imread(paths[i]);
        imwrite(dst+"/"+pattern+to_string(i)+"."+ext, image);
    }
}

void cropAll(vector <string> paths, string dst, string ext){
    Mat pivot = imread(paths[0]);
    Rect2d roi = selectROI(pivot);
    cout << roi << endl;
    for(int i = 0; i < paths.size(); ++i){
        Mat image = imread(paths[i]);
        imwrite(dst+"/"+to_string(i)+"."+ext, image(roi));
    }
}

void resizeAll(vector <string> paths, int w, int h, string dst, string ext){
    Mat new_image;
    for(int i = 0; i < paths.size(); ++i){
        resize(imread(paths[i]), new_image, Size(w, h));
        imwrite(dst+"/"+to_string(i)+"."+ext, new_image);
    }
}

bool sameSize(vector <Mat> images){
    int pw = images[0].size().width;
    int ph = images[0].size().height;
    for(int i = 0; i < images.size(); ++i){
        int nw = images[i].size().width;
        int nh = images[i].size().height;
        if(nw != pw || nh != ph){
            cout << "Some with dimensions: " << pw << " " << ph << endl;
            cout << "Other with dimensions: " << nw << " " << nh << endl;
            return false;
        }
    }
    return true;
}

vector <Mat> convert2Imgs(vector <string> paths){
    vector <Mat> imgs;
    for(int i=0; i < paths.size(); ++i){
        imgs.push_back(imread(paths[i]));
    }
    return imgs;
}

vector <string> getImagePaths(const string& pattern){
    glob_t glob_result;
    glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
    vector <string> images;
    for(unsigned int i=0; i<glob_result.gl_pathc; ++i){
        string path = string(glob_result.gl_pathv[i]);
        images.push_back(path);
    }
    globfree(&glob_result);
    return images;
}

static void show_usage_commands(string name, string action){
    cerr << "Usage: \n"
         << name << " " << action << " [options]\n\n"
         << "Options: \n"
         << " -s, --source\t\tPath to your directory with images.\n"
         << " -d, --destination\tPath where new images will be saved.\n"
         << " -p, --pattern\t\tPattern to rename all images.\n"
         << " -w, --width\t\tWidth to resize image.\n"
         << " -h, --height\t\tHeight to resize image.\n"
         << " -e, --extention\tNew image formats.\n"
         << endl;
}

static void show_usage_actions(string name){
    cerr << "Usage:\n"
         << name << " [option]\n\n"
         << "Options:\n"
         << "checksizes\tConfirms that all images have the same size (w,h). [-s]\n"
         << "resize\t\tResize all the images. [-s, -d, -w, -h]\n"
         << "crop\t\tCrop all the images by selecting a ROI. [-s, -d]\n"
         << "rename\t\tRename all the images according to pattern. [-s, -d, -p]\n"
         << "randdisplay\tDisplays a random image. [-s]\n"
         << endl;
}

int main(int argc, char* argv[])
{
    if(argc < 2){
        show_usage_actions(argv[0]);
        return 1;
    }

    string action = string(argv[1]);
    // Checking what process the program is going to follow
    if(action == "checksizes"){
        if(argc < 3){
            show_usage_commands(argv[0], argv[1]);
            return 1;
        }
        try{
            string src = argv[3];
            string ext = argv[5];
            vector <string> paths = getImagePaths(src+"/*."+ext);
            vector <Mat> images = convert2Imgs(paths);
            if(sameSize(images)){
                cout << "All images has the same size." << endl;
            }
            return 1;
        }catch(...){
            cout << "Exception occured.\n"
                 << "Remember use this formart: datacleaner checksizes -s [srcpath] -e [jpg/png/jpeg/...].\n"
                 << endl;
        }
    }else if (action == "resize"){
        if(argc < 6){
            show_usage_commands(argv[0], argv[1]);
            return 1;
        }
        try{
            string src = argv[3];
            string dst = argv[5];
            string ext = argv[7];
            int w = stoi(argv[9]);
            int h = stoi(argv[11]);

            vector <string> paths = getImagePaths(src+"/*."+ext);
            resizeAll(paths, w, h, dst, ext);
        }catch(...){
            cout << "Exception occured.\n"
                 << "Remember use this formart:\n"
                 << "datacleaner resize -s [srcpath] -d [dstpath] -e [jpg/png/jpeg/...] -w [width] -h [height].\n"
                 << endl;
        }
    }else if (action == "crop"){
        if(argc < 4){
            show_usage_commands(argv[0], argv[1]);
            return 1;
        }
        try{
            string src = argv[3];
            string dst = argv[5];
            string ext = argv[7];

            vector <string> paths = getImagePaths(src+"/*."+ext);
            vector <Mat> images = convert2Imgs(paths);

            if(!sameSize(images)){
                cerr << "Some images have different sizes, you must resize them (use resize command)" << endl;
                return 1;
            }else{
                cropAll(paths, dst, ext);
            }
        }catch(...){
            cout << "Exception occured.\n"
                 << "Remember use this formart: datacleaner crop -s [srcpath] -d [dstpath] -e [jpg/png/jpeg/...].\n"
                 << endl;
        }
    }else if (action == "rename"){
        if(argc < 5){
            show_usage_commands(argv[0], argv[1]);
            return 1;
        }
        try{
            string src = argv[3];
            string dst = argv[5];
            string ext = argv[7];
            string pattern = argv[9];
            vector <string> paths = getImagePaths(src+"/*."+ext);
            renameAll(paths, dst, pattern, ext);
        }catch(...){
            cout << "Exception occured.\n"
                 << "Remember use this formart: datacleaner rename -s [srcpath] -d [dstpath] -e [jpg/png/jpeg/...] -p [pattern].\n"
                 << endl;
        }
    }else if (action == "randdisplay"){
        if(argc < 3){
            show_usage_commands(argv[0], argv[1]);
            return 1;
        }
        cout << "Randdisplay is not implemented yet." << endl;
    }else{
        show_usage_actions(argv[0]);
        return 1;
    }
    return 0;
}