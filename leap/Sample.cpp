/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <iostream>
#include <cstring>
#include <unistd.h>
#include "Leap.h"
#include "../liblzr/lzr.h"


using namespace Leap;

#define LZR_SCALE_FACTOR 150.0


void* zmq_ctx;
void* zmq_pub;
lzr_interpolator* interp;
lzr_frame f;



void add_point(lzr_point p)
{
  f.points[f.n_points] = p;
  f.n_points++;  
}

void add_finger_to_frame(lzr_point p)
{
  lzr_point blanked_p = p;
  LZR_POINT_BLANK(blanked_p);

  for(int i = 0; i < 10; i++)
    add_point(blanked_p);

  for(int i = 0; i < 20; i++)
    add_point(p);

  for(int i = 0; i < 10; i++)
    add_point(blanked_p);
}


void draw_init_frame()
{
    f.points[0].x = -1.0;
    f.points[0].y = 0.0;
    f.points[0].r = 0;
    f.points[0].g = 0;
    f.points[0].b = 0;
    f.points[0].i = 0;

    //x axis
    f.points[1].x = -1.0;
    f.points[1].y = 0.0;
    f.points[1].r = 0;
    f.points[1].g = 255;
    f.points[1].b = 0;
    f.points[1].i = 255;
    //
    f.points[2].x = 1.0;
    f.points[2].y = 0.0;
    f.points[2].r = 0;
    f.points[2].g = 255;
    f.points[2].b = 0;
    f.points[2].i = 255;
    //blank
    f.points[3].x = 1.0;
    f.points[3].y = 0.0;
    f.points[3].r = 0;
    f.points[3].g = 0;
    f.points[3].b = 0;
    f.points[3].i = 0;

    f.points[4].x = 0.0;
    f.points[4].y = -1.0;
    f.points[4].r = 0;
    f.points[4].g = 0;
    f.points[4].b = 0;
    f.points[4].i = 0;

    //y-axis
    f.points[5].x = 0.0;
    f.points[5].y = -1.0;
    f.points[5].r = 0;
    f.points[5].g = 255;
    f.points[5].b = 0;
    f.points[5].i = 255;

    f.points[6].x = 0.0;
    f.points[6].y = 1.0;
    f.points[6].r = 0;
    f.points[6].g = 255;
    f.points[6].b = 0;
    f.points[6].i = 255;

    //blank
    f.points[7].x = 0.0;
    f.points[7].y = 1.0;
    f.points[7].r = 0;
    f.points[7].g = 0;
    f.points[7].b = 0;
    f.points[7].i = 0;    

    f.n_points = 8;
}



class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);

  private:
};

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  // controller.enableGesture(Gesture::TYPE_CIRCLE);
  // controller.enableGesture(Gesture::TYPE_KEY_TAP);
  // controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  // controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {

  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();

  draw_init_frame();
  // f.n_points = 0; //clear the current frame

  // std::cout << "Frame id: " << frame.id()
  //           << ", timestamp: " << frame.timestamp()
  //           << ", hands: " << frame.hands().count()
  //           << ", extended fingers: " << frame.fingers().extended().count()
  //           << ", tools: " << frame.tools().count()
  //           << ", gestures: " << frame.gestures().count() << std::endl;


  const FingerList fingers = frame.fingers();
  for(FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl)
  {
    Finger finger = *fl;
    Vector position = finger.tipPosition();
    // std::cout << "(" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;

    lzr_point p;
    p.r = 0;
    p.g = 255;
    p.b = 0;
    p.i = 255;
    p.x = position.x / -LZR_SCALE_FACTOR;
    p.y = (position.y - 250.0) / LZR_SCALE_FACTOR;

    add_finger_to_frame(p);
  }

  // Get tools
  // const ToolList tools = frame.tools();
  // for (ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl) {
  //   const Tool tool = *tl;
  //   std::cout << std::string(2, ' ') <<  "Tool, id: " << tool.id()
  //             << ", position: " << tool.tipPosition()
  //             << ", direction: " << tool.direction() << std::endl;
  // }

  //interpolate it!
  lzr_interpolator_run(interp, &f);

  std::cout << f.n_points << std::endl;
  //lase it!
  lzr_send_frame(zmq_pub, &f);
}

void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void SampleListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}

int main(int argc, char** argv) {

  //connect to LZR
  zmq_ctx = zmq_ctx_new();
  zmq_pub = lzr_frame_pub(zmq_ctx, LZR_ZMQ_ENDPOINT);
  interp = lzr_interpolator_create();

  //give the socket time to connect
  sleep(1);

  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  if (argc > 1 && strcmp(argv[1], "--bg") == 0)
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);


  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

  zmq_close(zmq_pub);
  zmq_ctx_term(zmq_ctx);
  lzr_interpolator_destroy(interp);

  return 0;
}
