#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/aruco.hpp>

// JSON formatting library
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace cv;

rapidjson::Value* ARUCOMarkerToJSON(int id, int dir, int confidence, float* center, std::vector<Point2f>corners, rapidjson::Document::AllocatorType& allocator) {
    rapidjson::Value* markerObj = new rapidjson::Value(rapidjson::kObjectType);
    markerObj->AddMember("id", id, allocator);
    markerObj->AddMember("dir", dir, allocator);
    markerObj->AddMember("confidence", confidence, allocator);
    markerObj->AddMember("type", "ARUCO", allocator);

    rapidjson::Value centerArray (rapidjson::kArrayType);
    centerArray.PushBack(center[0], allocator);
    centerArray.PushBack(center[1], allocator);
    markerObj->AddMember("center", centerArray, allocator);


    rapidjson::Value cornerArray (rapidjson::kArrayType);
    // WARNING: corners should be pushed in the following order:
    // top left - top right - bot right - bot left
    cornerArray.PushBack(corners[0].x, allocator);
    cornerArray.PushBack(corners[0].y, allocator);

    cornerArray.PushBack(corners[1].x, allocator);
    cornerArray.PushBack(corners[1].y, allocator);

    cornerArray.PushBack(corners[2].x, allocator);
    cornerArray.PushBack(corners[2].y, allocator);

    cornerArray.PushBack(corners[3].x, allocator);
    cornerArray.PushBack(corners[3].y, allocator);

    markerObj->AddMember("corners", cornerArray, allocator);
    return markerObj;
}

rapidjson::Value* ARUCOMarkersToJSON(std::vector<int> ids, std::vector<std::vector<Point2f>> corners, rapidjson::Document::AllocatorType& allocator) {
    rapidjson::Value* markersObj = new rapidjson::Value(rapidjson::kArrayType);
    for (int i = 0 ; i < ids.size() ; i++) {
        float* center = new float[2]; center[0] = 0; center[1] = 0;
        rapidjson::Value* markerObj = ARUCOMarkerToJSON(ids[i], 0, 100, center, corners[i], allocator);
        markersObj->PushBack(*markerObj, allocator);
        delete markerObj;
        delete[] center;
    }
    return markersObj;
}

int main(int argc, char** argv) {
    Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_ARUCO_ORIGINAL);
    std::vector<int> ids;
    std::vector<std::vector<Point2f> > corners, rejected;
    Mat image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    Mat imageCopy; image.copyTo(imageCopy);

    aruco::detectMarkers(image, dictionary, corners, ids, detectorParams, rejected);

    if (ids.size() > 0) {
        aruco::drawDetectedMarkers(imageCopy, corners, ids);
    } else {
        std::cerr << "No aruco markers found" << std::endl;
        aruco::drawDetectedMarkers(imageCopy, rejected, noArray(), Scalar(100, 0, 255));
    }

    rapidjson::Document jsonMarkers;
    jsonMarkers.SetObject();
    rapidjson::Document::AllocatorType allocator = jsonMarkers.GetAllocator();

    rapidjson::Value* markersObj = ARUCOMarkersToJSON(ids, corners, allocator);
    jsonMarkers.AddMember("markers", *markersObj, allocator);
    delete markersObj;

    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    jsonMarkers.Accept(writer);

    imshow("out", imageCopy);
    waitKey();
}
