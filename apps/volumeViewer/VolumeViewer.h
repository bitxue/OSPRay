// ======================================================================== //
// Copyright 2009-2014 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "QOSPRayWindow.h"
#include <QtGui>
#include <string>
#include <vector>

class TransferFunctionEditor;

class VolumeViewer : public QMainWindow {

  Q_OBJECT

  public:

  //! Constructor.
  VolumeViewer(const std::vector<std::string> &filenames, bool showFrameRate);

  //! Destructor.
  ~VolumeViewer() {};

  //! Get the OSPRay output window.
  QOSPRayWindow *getWindow() { return(osprayWindow); }

  //! Get the transfer function editor.
  TransferFunctionEditor *getTransferFunctionEditor() { return(transferFunctionEditor); }

  //! Select the model to be displayed.
  void setModel(size_t index) { ospSetObject(renderer, "model", models[index]);  ospCommit(renderer);  osprayWindow->setRenderingEnabled(true); }

  //! A string description of this class.
  std::string toString() const { return("VolumeViewer"); }

public slots:

  //! Toggle auto-rotation of the view.
  void autoRotate(bool set);

  //! Set auto-rotation rate
  void setAutoRotationRate(float rate) { autoRotationRate = rate; }

  //! Draw the model associated with the next time step.
  void nextTimeStep() { static size_t index = 0;  index = (index + 1) % models.size();  setModel(index); }

  //! Toggle animation over the time steps.
  void playTimeSteps(bool animate) { if (animate == true) playTimeStepsTimer.start(2000);  else playTimeStepsTimer.stop(); }

  //! Add a slice to the volume, optionally from file.
  void addSlice(std::string filename = std::string());

  //! Add geometry from file.
  void addGeometry(std::string filename = std::string());

  //! Re-commit all OSPRay volumes.
  void commitVolumes() { for(size_t i=0; i<volumes.size(); i++) ospCommit(volumes[i]); }

  //! Force the OSPRay window to be redrawn.
  void render() { if (osprayWindow != NULL) osprayWindow->updateGL(); }

protected:

  //! OSPRay models.
  std::vector<OSPModel> models;

  //! Model for dynamic geometry (slices); maintained separately from other geometry.
  OSPModel dynamicModel;

  //! OSPRay volumes.
  std::vector<OSPVolume> volumes;

  //! OSPRay renderer.
  OSPRenderer renderer;

  //! OSPRay transfer function.
  OSPTransferFunction transferFunction;

  //! OSPRay light.
  OSPLight light;

  //! The OSPRay output window.
  QOSPRayWindow *osprayWindow;

  //! The transfer function editor.
  TransferFunctionEditor *transferFunctionEditor;

  //! Layout for slice widgets.
  QVBoxLayout sliceWidgetsLayout;

  //! Auto-rotate button.
  QAction * autoRotateAction;

  //! Auto-rotation rate
  float autoRotationRate;

  //! Timer for use when stepping through multiple models.
  QTimer playTimeStepsTimer;

  //! Print an error message.
  void emitMessage(const std::string &kind, const std::string &message) const
  { std::cerr << "  " + toString() + "  " + kind + ": " + message + "." << std::endl; }

  //! Error checking.
  void exitOnCondition(bool condition, const std::string &message) const
  { if (!condition) return;  emitMessage("ERROR", message);  exit(1); }

  //! Load an OSPRay model from a file.
  void importObjectsFromFile(const std::string &filename);

  //! Create and configure the OSPRay state.
  void initObjects(const std::vector<std::string> &filenames);

  //! Create and configure the user interface widgets and callbacks.
  void initUserInterfaceWidgets();

};

