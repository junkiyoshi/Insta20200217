#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(255);
	ofEnableDepthTest();

	this->cap.open("D:\\video\\image51.mp4");
	this->cap_size = cv::Size(256, 144);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	this->number_of_frames = 25 * 27;
	for (int i = 0; i < this->number_of_frames; i++) {

		cv::Mat src, tmp;
		this->cap >> src;
		if (src.empty()) {

			return;
		}

		cv::resize(src, tmp, this->cap_size);
		cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

		this->frame_list.push_back(tmp);
	}

	for (int i = 0; i < 128; i++) {

		ofMesh mesh;
		mesh.addVertex(glm::vec3(-128, -72, 0));
		mesh.addVertex(glm::vec3(128, -72, 0));
		mesh.addVertex(glm::vec3(128, 72, 0));
		mesh.addVertex(glm::vec3(-128, 72, 0));

		mesh.addTexCoord(glm::vec3(0, 0, 0));
		mesh.addTexCoord(glm::vec3(256, 0, 0));
		mesh.addTexCoord(glm::vec3(256, 144, 0));
		mesh.addTexCoord(glm::vec3(0, 144, 0));

		mesh.addIndex(0); mesh.addIndex(1); mesh.addIndex(2);
		mesh.addIndex(0); mesh.addIndex(2); mesh.addIndex(3);

		this->mesh_list.push_back(mesh);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);
	ofRotateY(ofGetFrameNum() * 0.5);

	for (auto& mesh : this->mesh_list) {

		auto location = glm::vec3(ofRandom(-500, 500), ofRandom(-500, 500), ofRandom(-500, 500));
		//int n = ofMap(ofNoise(glm::vec4(location, ofGetFrameNum() * 0.005)), 0, 1, 0, this->number_of_frames);
		int n = int(ofRandom(1000) + ofGetFrameNum()) % this->number_of_frames;
		this->frame_list[n].copyTo(this->frame);
		this->image.update();

		ofPushMatrix();
		ofTranslate(location);

		this->image.bind();
		mesh.draw();
		this->image.unbind();

		ofPopMatrix();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}