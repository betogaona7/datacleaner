#include <iostream>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/stat.h>
#include <glob.h>

using namespace std;
using namespace cv;

void resizeAll(vector <string> paths, int w, int h, string dst){
    Mat new_image;
    for(int i = 0; i < paths.size(); ++i){
        resize(imread(paths[i]), new_image, Size(w, h));
        imwrite(dst+"/"+to_string(i)+".jpg", new_image);
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

void selectedEvent(int event, int x, int y, int flags, void*userdata){
    if(event == EVENT_LBUTTONDOWN){
        cout << "Left click. " << x << " " << y << endl;
    }else if (event == EVENT_RBUTTONDOWN){
        cout << "Right click. " << x << " " << y << endl;
    }else if(event == EVENT_MOUSEMOVE){
        cout << "Mouse move in the position " << x << " " << y << endl;
    }

}

int main()
{
    string src, dst;
    cin >> src;
    cin >> dst;

    // Create a folder for new data
    if(mkdir(dst.c_str(), 0777) == -1){
        cerr << "Error: " << strerror(errno) << endl;
        return -1;
    }else{
        cout << "Images will be saved in " << dst << endl;
    }

    //jpg images
    src += "/*.JPG";

    // Get all files in the src
    vector <string> paths = getImagePaths(src);
    vector <Mat> images = convert2Imgs(paths);

    if(paths.size() == 0){
        cout << "No images found." << endl;
        return -1;
    }else{
        cout << paths.size() << " images found." << endl;
    }

    for(int i=0; i < paths.size(); ++i){
        cout << paths[i] << endl;
    }

    cout << sameSize(images) << endl;
    resizeAll(paths, 224, 224, dst);

    return 0;
}