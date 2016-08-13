#include "epipolar.hpp"

int main(){
	vector<Image> images;
	string path;
  cout << "---start---" << endl;
	Ptr<AKAZE> akaze = AKAZE::create();
  cout << "---generated akaze detector---" << endl;

	for(int i = 0; i < 3; i++){
	  path = "./img/" + to_string(i) + ".JPG";
	  images.push_back(Image(path,akaze));
	}
  
  cout << "Brute Force" << endl;
	BFMatcher matcher(NORM_L2, false);

	vector<vector<DMatch>> matches;
	vector<vector<vector<DMatch>>> matchesBetween0andOthers;

	for(int i = 0; i < images.size() - 1; i++){
    matches.clear();
    matcher.knnMatch(images[0].descriptor,images[i + 1].descriptor,matches,2);
    matchesBetween0andOthers.push_back(matches);
	}

	vector<DMatch> tmp_match;
	vector<KeyPoint> tmp_queryKps, tmp_trainKps;
  vector<vector<DMatch>> GoodMatches;
  vector<vector<KeyPoint>> GoodQueryKeys,GoodTrainKeys;

	for(int i = 0; i < matchesBetween0andOthers.size() ; i++){
    tmp_match.clear();
    tmp_queryKps.clear();
    tmp_trainKps.clear();
	  for(int j = 0; j < matchesBetween0andOthers[i].size(); j++){
      if(matchesBetween0andOthers[i][j][0].distance < 450){
        if(matchesBetween0andOthers[i][j][0].distance / matchesBetween0andOthers[i][j][1].distance < 0.8){
          tmp_match.push_back(matchesBetween0andOthers[i][j][0]);
          tmp_queryKps.push_back(images[0].kps[matchesBetween0andOthers[i][j][0].queryIdx]);
          tmp_trainKps.push_back(images[i+1].kps[matchesBetween0andOthers[i][j][0].trainIdx]);   
        }
      }
    }
    GoodMatches.push_back(tmp_match);
    GoodQueryKeys.push_back(tmp_queryKps);
    GoodTrainKeys.push_back(tmp_trainKps);
	}
	
  for(int i=0;i<GoodTrainKeys.size();i++){
    for (int j=0;j<GoodTrainKeys[i].size();j++){
      cout << GoodTrainKeys[i][j].size << endl;
    }
  }
  
  cout << GoodTrainKeys.size() << endl;

  vector<Point2f> tmp_Qkps, tmp_Tkps;
	vector<vector<Point2f>> QkpsArray, TkpsArray;

  for(int i = 0; i < matchesBetween0andOthers.size(); i++){
    tmp_Qkps.clear();
    tmp_Tkps.clear();
	  KeyPoint::convert(GoodQueryKeys[i],tmp_Qkps);
	  KeyPoint::convert(GoodTrainKeys[i],tmp_Tkps);
	  QkpsArray.push_back(tmp_Qkps);
	  TkpsArray.push_back(tmp_Tkps);
  }

  vector<MouseInfo> Information(images.size());
	vector<Mat> fundamental;
  vector<vector<Vec3f>> lines(images.size());

	for(int i = 0; i < images.size() - 1; i++){
	  Mat fund;
    fund = findFundamentalMat(QkpsArray[i],TkpsArray[i],FM_RANSAC,2);
    cout << fund << endl;
    fundamental.push_back(fund);
    Information[i+1].title = images[0].name;
    Information[i+1].F = fund;
    Information[i+1].center = images[0].image;
	}

  for(int i = 0; i < images.size(); i++){
    cout << images[i].name << endl;
    namedWindow(images[i].name,WINDOW_NORMAL);
    imshow(images[i].name,images[i].image);
    if(i!=0) {
      cout << Information[i].F << endl;
      cvSetMouseCallback((images[i].name).c_str(), on_mouse,(MouseInfo*) &Information[i]);
    }
  }
  waitKey(0);
}
