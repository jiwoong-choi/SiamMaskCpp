# SiamMaskCpp
* C++ Implementation of [SiamMask](https://github.com/foolwood/SiamMask)
* Porting slogan:
    * numpy operations &rarr; cv::Mat operations
    * CNNs &rarr; torch::jit::script::Module
    * Other tensor operations &rarr; torch::Tensor operations
* Faster than the original implementation (speed increased from 22fps to 40fps when tested with a single NVIDIA GeForce GTX 1070)

# Tested environment
* OS: Ubuntu 20.04
* CUDA: 11.1
* Python: 3.8.10
* PyTorch: 1.10.2
* OpenCV: 3.4.12

# Quick Start Guide
## 1. Build OpenCV 3.4.12 from source
**Warning: OpenCV4.x doesn't work with this repository (See the issue [Build libtorch with -D_GLIBCXX_USE_CXX11_ABI=1 #14620](https://github.com/pytorch/pytorch/issues/14620#issuecomment-735236765))**
### a) Install required apt packages
```bash
sudo apt -y update
sudo apt -y upgrade
sudo apt -y install build-essential cmake pkg-config \
  libjpeg-dev libtiff5-dev libpng-dev \
  libavcodec-dev libavformat-dev libswscale-dev libavresample-dev \
  libdc1394-22-dev libxvidcore-dev libx264-dev ffmpeg \
  libxine2-dev libv4l-dev v4l-utils \
  libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
  libatlas-base-dev libeigen3-dev gfortran \
  python3-dev python3-numpy libtbb2 libtbb-dev
```
### b) Build and install OpenCV 3.4.12
```bash
cd ~
mkdir OpenCV4
cd OpenCV4
# Set the version as you'd like
git clone -b 3.4.12 https://github.com/opencv/opencv
git clone -b 3.4.12 https://github.com/opencv/opencv_contrib
cd opencv
mkdir build
cd build
# You may add or remove OpenCV Cmake options as appropriate
cmake -D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_EXAMPLES=OFF \
-D INSTALL_PYTHON_EXAMPLES=OFF \
-D WITH_TESTS=OFF \
-D WITH_PERF_TESTS=OFF \
-D BUILD_opencv_python3=ON \
-D OPENCV_ENABLE_NONFREE=ON \
-D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D WITH_TBB=OFF \
-D WITH_CUDA=ON ..
make -j`nproc`
make install
sudo ldconfig
```

## 2. Install torch
### a) [Recommended] Create a virtual environment
```bash
virtualenv -p python3 venv
source venv/bin/activate
```
### b) Install PyTorch
You may modify the version string as appropriate.
```bash
pip install torch==1.10.2+cu111 --find-links https://download.pytorch.org/whl/torch_stable.html
```

## 3. Build the demo executable
Make sure to update git submodule before building the demo
```bash
cd /path/to/SiamMaskCpp
git submodule update --init --recursive
```
Build the executable as follows:
```bash
cd /path/to/SiamMaskCpp
mkdir build
cd build
# specify -DTORCH_PATH=/path/to/lib/python3.x/site-packages/torch if cmake fails to detect PyTorch automatically
cmake ..
make
```

## 4. Download pretrained Torch scripts
You can use the models (with the refine module) trained with the original repository [foolwood/SiamMask](https://github.com/foolwood/SiamMask) for inference in C++. Just Follow the instruction in [jiwoong-choi/SiamMask](https://github.com/jiwoong-choi/SiamMask#converting-siammask-model-with-the-refine-module-to-torch-scripts) to convert your own models to Torch script files.

Alternatively, you can download pretrained Torch scripts. These files are converted from the pretrained models ([SiamMask_DAVIS.pth](http://www.robots.ox.ac.uk/~qwang/SiamMask_DAVIS.pth) and [SiamMask_VOT.pth](http://www.robots.ox.ac.uk/~qwang/SiamMask_VOT.pth)) in the original repository.

```bash
cd /path/to/SiamMaskCpp
mkdir models
cd models
wget https://github.com/nearthlab/SiamMaskCpp/releases/download/v1.0/SiamMask_DAVIS.tar.gz
wget https://github.com/nearthlab/SiamMaskCpp/releases/download/v1.0/SiamMask_VOT.tar.gz
tar -xvzf SiamMask_DAVIS.tar.gz
tar -xvzf SiamMask_VOT.tar.gz
```

Before building demo, make sure the following command prints out the correct path to torch install directory.
```bash
python3 -c "import torch; print(torch.__path__[0])"
# /path/to/lib/python3.x/site-packages/torch
```

## 5. Run the demo executable
```bash
cd SiamMaskCpp
./demo -c config_davis.json -m models/SiamMask_DAVIS tennis
./demo -c config_vot.json -m models/SiamMask_VOT tennis
```
