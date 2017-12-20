#include "ofxMinimalButton.h"
#include "ofGraphics.h"
using namespace std;

ofxMinimalButton::ofxMinimalButton(){
	value.setSerializable(false);
}

ofxMinimalButton::ofxMinimalButton(string toggleName, float width, float height):ofxMinimalToggle(){
    if(width == 0) {
         width = getTextWidth(toggleName, height);
    }
    setup(toggleName, width, height);
}

ofxMinimalButton::~ofxMinimalButton(){
    value.removeListener(this,&ofxMinimalButton::valueChanged);
}

ofxMinimalButton* ofxMinimalButton::setup(string toggleName, float width, float height){
    if(width == 0) {
         width = getTextWidth(toggleName, height);
    }
	setName(toggleName);
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	value = false;
    checkboxRect.set(1, 1, b.width - 2, b.height - 2);

	registerMouseEvents();

    value.addListener(this,&ofxMinimalButton::valueChanged);

	return this;
}

bool ofxMinimalButton::mouseReleased(ofMouseEventArgs & args){
	bool attended = setValue(args.x, args.y, false);
	bGuiActive = false;
	if(attended){
		return true;
	}else{
		return false;
	}
}

bool ofxMinimalButton::mouseMoved(ofMouseEventArgs & args){
    return ofxMinimalToggle::mouseMoved(args);
}

bool ofxMinimalButton::mousePressed(ofMouseEventArgs & args){
    return ofxMinimalToggle::mousePressed(args);
}

bool ofxMinimalButton::mouseDragged(ofMouseEventArgs & args){
    return ofxMinimalToggle::mouseDragged(args);
}

void ofxMinimalButton::valueChanged(bool & value){
    ofxMinimalToggle::valueChanged(value);
    if(!value){
        ofNotifyEvent(triggerEvent, this);
    }
}
