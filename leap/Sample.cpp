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
#define LZR_NUM_BLANKS 7
#define LZR_NUM_FINGER_BLANKS 7
#define LZR_NUM_BEAMS 10

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

  for(int i = 0; i < LZR_NUM_FINGER_BLANKS; i++)
    add_point(blanked_p);

  for(int i = 0; i < LZR_NUM_BEAMS; i++)
    add_point(p);

  for(int i = 0; i < LZR_NUM_FINGER_BLANKS; i++)
    add_point(blanked_p);
}


void draw_init_frame()
{
    f.n_points = 0;

    lzr_point a;
    lzr_point b;
    lzr_point c;
    lzr_point d;

    a.x = -1.0;
    a.y = 0.0;
    a.r = 0;
    a.g = 255;
    a.b = 0;
    a.i = 255;
    //
    b.x = 1.0;
    b.y = 0.0;
    b.r = 0;
    b.g = 255;
    b.b = 0;
    b.i = 255;

    c.x = 0.0;
    c.y = -1.0;
    c.r = 0;
    c.g = 255;
    c.b = 0;
    c.i = 255;

    d.x = 0.0;
    d.y = 1.0;
    d.r = 0;
    d.g = 255;
    d.b = 0;
    d.i = 255;

    lzr_point ba = a;
    lzr_point bb = b;
    lzr_point bc = c;
    lzr_point bd = d;

    LZR_POINT_BLANK(ba);
    LZR_POINT_BLANK(bb);
    LZR_POINT_BLANK(bc);
    LZR_POINT_BLANK(bd);

    for(int i = 0; i < LZR_NUM_BLANKS; i++)
      add_point(ba);

    add_point(a);
    add_point(b);

    for(int i = 0; i < LZR_NUM_BLANKS; i++)
      add_point(bb);

    for(int i = 0; i < LZR_NUM_BLANKS; i++)
      add_point(bc);

    add_point(c);
    add_point(d);

    for(int i = 0; i < LZR_NUM_BLANKS; i++)
      add_point(bd);
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

#define LEAP_DISTANCE 200.0

bool within_frame(const lzr_point& p)
{
    return ((-1 < p.x) && (p.x < 1) && (-1 < p.y) && (p.y < 1));
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

    lzr_point p;
    p.r = 0;
    p.g = 255;
    p.b = 255;
    p.i = 255;
    p.x = position.x / -LZR_SCALE_FACTOR;
    p.y = (position.y - LEAP_DISTANCE) / LZR_SCALE_FACTOR;

    if(within_frame(p)) {
        add_finger_to_frame(p);
    }

    int posval = position.z;
    p.r = (posval < -16)*255;
    p.g = (-32 < posval && posval < 32)*255;
    p.b = (16 < posval)*255;
    p.i = 255;
    p.x = position.x / -LZR_SCALE_FACTOR;
    p.y = (position.y - LEAP_DISTANCE) / LZR_SCALE_FACTOR + 0.5;

    if(within_frame(p)) {
        add_finger_to_frame(p);
    }
    // printf("(% 5f, % 5f, % 5f)\n", position.x, position.y, position.z);

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

  //std::cout << f.n_points << std::endl;
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
