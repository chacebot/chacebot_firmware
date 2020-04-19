FROM ros:kinetic-robot

WORKDIR /usr/chacebot_ws
COPY chacebot_ws .

RUN apt-get update && apt-get install -y \
    python3-pip \
    python3-numpy \
    python3-curtsies \ 
    python-pip \
    python-curtsies \
    #wiringpi 


RUN echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc

RUN . /opt/ros/kinetic/setup.sh && \
    catkin_make
RUN echo "source /devel/setup.bash" >> ~/.bashrc
#CMD "roscore"