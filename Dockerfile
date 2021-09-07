# Pull base image
# based on - https://larrylisky.com/2016/11/03/point-cloud-library-on-ubuntu-16-04-lts/
FROM ubuntu

# install prereqs
RUN apt-get update
RUN apt-get install -y software-properties-common
RUN apt-get update
RUN apt-get install -y git build-essential linux-libc-dev
RUN apt-get install -y cmake cmake-gui 
RUN apt-get install -y libusb-1.0-0-dev libusb-dev libudev-dev
RUN apt-get install -y mpi-default-dev openmpi-bin openmpi-common  
RUN apt-get install -y libflann-dev
RUN apt-get install -y libeigen3-dev
RUN apt-get install -y libboost-all-dev
RUN apt-get install -y libvtk7-dev
RUN apt-get install -y libqhull* libgtest-dev
RUN apt-get install -y freeglut3-dev pkg-config
RUN apt-get install -y libxmu-dev libxi-dev 
RUN apt-get install -y mono-complete
RUN apt-get install -y barrier openjdk-8-jdk openjdk-8-jre
RUN apt-get install -y openssh-client

# get pcl
#RUN add-apt-repository -y ppa:v-launchpad-jochen-sprickerhof-de/pcl
#RUN apt-get update
RUN git clone https://github.com/PointCloudLibrary/pcl.git ~/pcl

# install pcl
RUN mkdir ~/pcl/release
RUN cd ~/pcl/release && cmake -DCMAKE_BUILD_TYPE=None -DCMAKE_INSTALL_PREFIX=/usr \
           -DBUILD_GPU=ON -DBUILD_apps=ON -DBUILD_examples=ON \
           -DCMAKE_INSTALL_PREFIX=/usr ~/pcl
RUN cd ~/pcl/release && make
RUN cd ~/pcl/release && make install