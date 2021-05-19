/**
 * @file   main.cpp
 * @author Dennis Sitelew
 * @date   May. 18, 2021
 */

#include <Arduino.h>

#include <hw/application.h>

using namespace hw;

application *g_app;

void setup() {
   g_app = new application();
}

void loop() {
   g_app->update();
}
